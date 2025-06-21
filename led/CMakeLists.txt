add_library(kitten-led INTERFACE)

target_sources(kitten-led INTERFACE
    led.cpp
)

target_link_libraries(kitten-led INTERFACE
    hardware_pwm
)


target_include_directories(kitten-led INTERFACE ${CMAKE_CURRENT_LIST_DIR})