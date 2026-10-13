add_library(data STATIC
    src/ring_buffer.c
    src/data_proc.c
    src/event_log.c
)

target_include_directories(data PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(data PUBLIC common)