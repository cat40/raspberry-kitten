add_library(kitten-matrix INTERFACE)

target_sources(kitten-matrix INTERFACE
    button_matrix.cpp
)

target_link_libraries(kitten-matrix INTERFACE
    kitten-utilities
)


target_include_directories(kitten-utilities INTERFACE ${CMAKE_CURRENT_LIST_DIR})