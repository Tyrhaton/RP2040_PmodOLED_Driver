cmake_minimum_required(VERSION 3.13)
include(~/pico-sdk/pico_sdk_init.cmake)
project(main C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_SDK_FETCH_FROM_GIT on)
pico_sdk_init()
include_directories(libs)
add_executable(main
example.c
oled.c
font.c
oled.h
)

target_link_libraries(main pico_stdlib hardware_adc hardware_uart hardware_gpio hardware_irq hardware_spi m)
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)
pico_add_extra_outputs(main)
