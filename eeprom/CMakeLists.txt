add_library(kitten-eeprom INTERFACE)

target_sources(kitten-eeprom INTERFACE
    eeprom.cpp
)

target_link_libraries(kitten-eeprom INTERFACE
    hardware_i2c
)


target_include_directories(kitten-eeprom INTERFACE ${CMAKE_CURRENT_LIST_DIR})