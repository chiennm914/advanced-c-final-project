add_executable(iot_monitor
    main.c
)

target_link_libraries(iot_monitor PRIVATE
    app_logic
)