idf_component_register(
    SRCS "app_main.c"
    INCLUDE_DIRS "."
    REQUIRES
        common
        storage_manager
        diagnostics_manager
        sensor_manager
        fan_manager
        heater_manager
        safety_manager
        state_machine
        ble_manager
        wifi_manager
        local_api
        ota_manager
)
