idf_component_register(
    SRCS "heater_manager.c"
    INCLUDE_DIRS "include"
    REQUIRES common driver safety_manager
)

