
add_library(alert STATIC
    src/alert.c
)

target_include_directories(alert PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(alert PUBLIC
    common
    logger
    drivers
)
