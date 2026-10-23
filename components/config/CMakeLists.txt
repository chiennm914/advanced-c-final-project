add_library(config STATIC
    src/config.c
)

target_include_directories(config PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(config PUBLIC
    common
    logger
    alert
)