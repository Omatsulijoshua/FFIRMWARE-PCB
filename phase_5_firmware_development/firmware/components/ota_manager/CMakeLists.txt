idf_component_register(
    SRCS "ota_manager.c" "ota_types.c"
    INCLUDE_DIRS "include"
    REQUIRES common esp_https_ota app_update esp_http_client json wifi_manager state_machine safety_manager
)
