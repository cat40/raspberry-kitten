add_library(kitten-utilities INTERFACE)

target_sources(kitten-utilities INTERFACE
    utilities.c
)

# target_link_libraries(kitten-utilities INTERFACE
#     hardware_pwm
# )


target_include_directories(kitten-utilities INTERFACE ${CMAKE_CURRENT_LIST_DIR})