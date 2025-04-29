add_library(pico_trinamic INTERFACE)

target_sources(pico_trinamic INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/trinamic/common.c
        ${CMAKE_CURRENT_LIST_DIR}/trinamic/tmc_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/trinamic/tmc2130.c
        ${CMAKE_CURRENT_LIST_DIR}/trinamic/tmc2130.c
        ${CMAKE_CURRENT_LIST_DIR}/trinamic/tmc2209.c
        ${CMAKE_CURRENT_LIST_DIR}/trinamic/tmc5160.c
        ${CMAKE_CURRENT_LIST_DIR}/tmc_uart.c
)

target_include_directories(pico_trinamic INTERFACE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/trinamic)

# Add the used pico libraries to the library
target_link_libraries(pico_trinamic INTERFACE
        pico_stdlib
        hardware_uart
        )
