#include "local_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app_config.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "ota_manager.h"
#include "state_machine.h"
#include "wifi_manager.h"
#include "wifi_protocol.h"

static const char *TAG = "local_api";
static httpd_handle_t s_server;

static const char *json_bool(bool value)
{
    return value ? "true" : "false";
}

static esp_err_t send_json(httpd_req_t *req, const char *json)
{
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_sendstr(req, json);
}

static esp_err_t status_handler(httpd_req_t *req)
{
    sdhs_status_t st = state_machine_get_status();
    char body[512];
    snprintf(body, sizeof(body),
             "{"
             "\"state\":\"%s\","
             "\"program\":\"%s\","
             "\"fault\":\"%s\","
             "\"elapsedSeconds\":%lu,"
             "\"remainingSeconds\":%lu,"
             "\"outletTempC\":%.2f,"
             "\"humidityRh\":%.2f,"
             "\"fanPercent\":%u,"
             "\"heaterOn\":%s,"
             "\"completionReason\":\"%s\""
             "}",
             sdhs_state_to_string(st.state),
             sdhs_program_to_string(st.program),
             sdhs_fault_to_string(st.fault),
             (unsigned long)st.elapsed_seconds,
             (unsigned long)st.remaining_seconds,
             st.outlet_temp_c,
             st.humidity_rh,
             st.fan_percent,
             json_bool(st.heater_on),
             sdhs_completion_to_string(st.completion_reason));
    return send_json(req, body);
}

static sdhs_program_t parse_program_from_body(const char *body)
{
    const char *key = "\"program\"";
    const char *p = strstr(body, key);
    if (p == NULL) {
        return SDHS_PROGRAM_NONE;
    }
    p = strchr(p, ':');
    if (p == NULL) {
        return SDHS_PROGRAM_NONE;
    }
    p = strchr(p, '"');
    if (p == NULL) {
        return SDHS_PROGRAM_NONE;
    }
    p++;

    char program[32] = {0};
    size_t i = 0;
    while (*p != '\0' && *p != '"' && i < sizeof(program) - 1) {
        program[i++] = *p++;
    }
    return wifi_protocol_program_from_string(program);
}

static uint32_t parse_duration_from_body(const char *body)
{
    const char *key = "\"durationSeconds\"";
    const char *p = strstr(body, key);
    if (p == NULL) {
        return 0;
    }
    p = strchr(p, ':');
    if (p == NULL) {
        return 0;
    }
    return (uint32_t)strtoul(p + 1, NULL, 10);
}

static esp_err_t read_body(httpd_req_t *req, char *body, size_t body_len)
{
    if (body_len == 0) {
        return ESP_ERR_INVALID_ARG;
    }
    int remaining = req->content_len;
    if (remaining <= 0 || remaining >= (int)body_len) {
        return ESP_ERR_INVALID_SIZE;
    }
    int received = httpd_req_recv(req, body, remaining);
    if (received <= 0) {
        return ESP_FAIL;
    }
    body[received] = '\0';
    return ESP_OK;
}

static esp_err_t start_handler(httpd_req_t *req)
{
    char body[256];
    if (read_body(req, body, sizeof(body)) != ESP_OK) {
        httpd_resp_set_status(req, "400 Bad Request");
        return send_json(req, "{\"ok\":false,\"error\":\"invalid_body\"}");
    }

    sdhs_program_t program = parse_program_from_body(body);
    if (program == SDHS_PROGRAM_NONE) {
        httpd_resp_set_status(req, "400 Bad Request");
        return send_json(req, "{\"ok\":false,\"error\":\"invalid_program\"}");
    }

    esp_err_t err = state_machine_start_program(program, parse_duration_from_body(body));
    if (err != ESP_OK) {
        httpd_resp_set_status(req, "409 Conflict");
        return send_json(req, "{\"ok\":false,\"error\":\"state_rejected\"}");
    }
    return send_json(req, "{\"ok\":true}");
}

static esp_err_t stop_handler(httpd_req_t *req)
{
    state_machine_stop();
    return send_json(req, "{\"ok\":true}");
}

static esp_err_t clear_fault_handler(httpd_req_t *req)
{
    state_machine_clear_fault();
    return send_json(req, "{\"ok\":true}");
}

static esp_err_t device_handler(httpd_req_t *req)
{
    char body[256];
    snprintf(body, sizeof(body),
             "{"
             "\"name\":\"SDHS\","
             "\"firmwareVersion\":\"%s\","
             "\"hardwareRevision\":\"%s\","
             "\"wifiConnected\":%s,"
             "\"wifiState\":\"%s\","
             "\"rssi\":%d"
             "}",
             SDHS_FIRMWARE_VERSION,
             SDHS_HARDWARE_REVISION,
             json_bool(wifi_manager_is_connected()),
             wifi_protocol_state_to_string(wifi_manager_get_state()),
             wifi_manager_get_rssi());
    return send_json(req, body);
}

static esp_err_t ota_status_handler(httpd_req_t *req)
{
    sdhs_ota_status_t ota = ota_manager_get_status();
    char body[256];
    snprintf(body, sizeof(body),
             "{"
             "\"state\":\"%s\","
             "\"error\":\"%s\","
             "\"currentVersion\":\"%s\","
             "\"targetVersion\":\"%s\","
             "\"progress\":%u"
             "}",
             ota_state_to_string(ota.state),
             ota_error_to_string(ota.error),
             ota.current_version,
             ota.target_version,
             ota.progress_percent);
    return send_json(req, body);
}

static const char *parse_url_from_body(char *body)
{
    const char *key = "\"url\"";
    char *p = strstr(body, key);
    if (p == NULL) {
        return NULL;
    }
    p = strchr(p, ':');
    if (p == NULL) {
        return NULL;
    }
    p = strchr(p, '"');
    if (p == NULL) {
        return NULL;
    }
    p++;
    char *end = strchr(p, '"');
    if (end == NULL) {
        return NULL;
    }
    *end = '\0';
    return p;
}

static esp_err_t ota_start_handler(httpd_req_t *req)
{
    sdhs_status_t status = state_machine_get_status();
    if (status.state != SDHS_STATE_IDLE && status.state != SDHS_STATE_COMPLETE) {
        httpd_resp_set_status(req, "409 Conflict");
        return send_json(req, "{\"ok\":false,\"error\":\"cycle_active\"}");
    }

    char body[256];
    if (read_body(req, body, sizeof(body)) != ESP_OK) {
        httpd_resp_set_status(req, "400 Bad Request");
        return send_json(req, "{\"ok\":false,\"error\":\"invalid_body\"}");
    }

    const char *url = parse_url_from_body(body);
    if (url == NULL) {
        httpd_resp_set_status(req, "400 Bad Request");
        return send_json(req, "{\"ok\":false,\"error\":\"missing_url\"}");
    }

    esp_err_t err = ota_manager_start_update(url);
    if (err != ESP_OK) {
        httpd_resp_set_status(req, "409 Conflict");
        return send_json(req, "{\"ok\":false,\"error\":\"ota_rejected\"}");
    }
    return send_json(req, "{\"ok\":true}");
}

static void register_uri(httpd_handle_t server, const char *uri, httpd_method_t method, esp_err_t (*handler)(httpd_req_t *))
{
    httpd_uri_t config = {
        .uri = uri,
        .method = method,
        .handler = handler,
        .user_ctx = NULL,
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &config));
}

esp_err_t local_api_init(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    esp_err_t err = httpd_start(&s_server, &config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "failed to start HTTP server: %s", esp_err_to_name(err));
        return err;
    }

    register_uri(s_server, "/api/v1/status", HTTP_GET, status_handler);
    register_uri(s_server, "/api/v1/cycle/start", HTTP_POST, start_handler);
    register_uri(s_server, "/api/v1/cycle/stop", HTTP_POST, stop_handler);
    register_uri(s_server, "/api/v1/fault/clear", HTTP_POST, clear_fault_handler);
    register_uri(s_server, "/api/v1/device", HTTP_GET, device_handler);
    register_uri(s_server, "/api/v1/ota/status", HTTP_GET, ota_status_handler);
    register_uri(s_server, "/api/v1/ota/start", HTTP_POST, ota_start_handler);

    ESP_LOGI(TAG, "local API server started on port %u", config.server_port);
    return ESP_OK;
}
