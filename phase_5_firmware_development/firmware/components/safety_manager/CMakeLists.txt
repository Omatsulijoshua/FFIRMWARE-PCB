idf_component_register(
    SRCS "safety_manager.c"
    INCLUDE_DIRS "include"
    REQUIRES common sensor_manager fan_manager diagnostics_manager
)

