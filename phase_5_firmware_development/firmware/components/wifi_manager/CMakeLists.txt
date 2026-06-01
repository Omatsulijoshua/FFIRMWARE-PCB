idf_component_register(
    SRCS "wifi_manager.c" "wifi_protocol.c"
    INCLUDE_DIRS "include"
    REQUIRES common esp_wifi esp_event storage_manager
)
