cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(parakeyt_fw)

set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")

pico_sdk_init()

add_compile_options(-Wall)

add_executable(parakeyt_fw
    src/io/io.c
    src/io/mcp23008.c
    src/main.c
    src/util.c
)

pico_enable_stdio_usb(parakeyt_fw 1)
pico_enable_stdio_uart(parakeyt_fw 0)

target_link_libraries(parakeyt_fw
    pico_stdlib
    pico_time
    pico_rand
    pico_stdio
    pico_sync
    hardware_i2c
    hardware_gpio
    hardware_adc
    pico_multicore
    pico_binary_info
    hardware_vreg
)
pico_add_extra_outputs(parakeyt_fw)

