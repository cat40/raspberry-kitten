add_library(kitten-pcf8523 INTERFACE)

target_sources(kitten-pcf8523 INTERFACE
    pcf8523.cpp
)

target_link_libraries(kitten-pcf8523 INTERFACE
    hardware_i2c
)


target_include_directories(kitten-pcf8523 INTERFACE ${CMAKE_CURRENT_LIST_DIR})