idf_component_register(
    SRCS "local_api.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_http_server common state_machine wifi_manager ota_manager
)

