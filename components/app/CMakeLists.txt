
add_library(app_logic STATIC
    src/app_fsm.c
    src/cli_cmd.c
)

target_include_directories(app_logic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(app_logic PUBLIC
    common
    logger
    alert
)
