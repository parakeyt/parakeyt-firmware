cmake_minimum_required(VERSION 3.13)

# i2c bus scanner
add_executable(bus_scan
    bus_scan.c
)

pico_enable_stdio_usb(bus_scan 1)
pico_enable_stdio_uart(bus_scan 0)

target_link_libraries(bus_scan
    pico_stdlib
    pico_stdio
    hardware_i2c
    pico_binary_info
    hardware_vreg
)
pico_add_extra_outputs(bus_scan)

# matrix printer
add_executable(printer printer.c)
target_sources(printer PUBLIC
    ../src/io/io.c
    ../src/io/TLA2528.c
    ../src/io/TLC59208.c
    ../src/io/MCP23008.c
    ../src/util.c
)

# target_include_directories(printer PUBLIC
#     ../include)

target_link_libraries(printer
    pico_unique_id
    pico_stdlib
    pico_time
    pico_rand
    pico_stdio
    pico_sync
    hardware_i2c
    hardware_pio
    hardware_gpio
    hardware_adc
    pico_multicore
    pico_binary_info
)
pico_enable_stdio_usb(printer 1)
pico_enable_stdio_uart(printer 0)
pico_generate_pio_header(printer ${CMAKE_CURRENT_LIST_DIR}/../src/io/ws2812.pio)
pico_add_extra_outputs(printer)
