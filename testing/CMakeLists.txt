cmake_minimum_required(VERSION 3.13)

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

