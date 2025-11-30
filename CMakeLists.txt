cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(parakeyt_fw)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.2.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.2.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

# If you want debug output from USB (pass -DPICO_STDIO_USB=1) this ensures you don't lose any debug output while USB is set up
if (NOT DEFINED PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS)
    set(PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS 3000)
endif()

pico_sdk_init()

add_executable(parakeyt_fw
    src/io/io.c
    src/io/TLA2528.c
    src/io/TLC59208.c
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
    hardware_clocks
    pico_multicore
    pico_binary_info
    hardware_vreg
)
pico_add_extra_outputs(parakeyt_fw)

add_subdirectory(testing)
