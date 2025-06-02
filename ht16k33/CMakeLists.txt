add_library(kitten-ht16k33 INTERFACE)

target_sources(kitten-ht16k33 INTERFACE
    ht16k33.cpp
)

target_include_directories(kitten-ht16k33 INTERFACE ${CMAKE_CURRENT_LIST_DIR})