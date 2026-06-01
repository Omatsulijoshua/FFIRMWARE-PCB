idf_component_register(
    SRCS "ble_manager.c" "ble_protocol.c"
    INCLUDE_DIRS "include"
    REQUIRES common state_machine bt
)
