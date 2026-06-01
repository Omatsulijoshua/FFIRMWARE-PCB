idf_component_register(
    SRCS "state_machine.c" "program_profiles.c"
    INCLUDE_DIRS "include"
    REQUIRES common sensor_manager fan_manager heater_manager safety_manager esp_timer
)
