include(FetchContent)
FetchContent_Declare(unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.6.0)
FetchContent_MakeAvailable(unity)

# Disable clang-tidy for third-party Unity sources
set_target_properties(unity PROPERTIES C_CLANG_TIDY "")

# Helper function: add a test linking to specific components
function(add_iot_test TEST_NAME SOURCE_FILE)
    add_executable(${TEST_NAME} ${SOURCE_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE ${ARGN} unity)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

# Test targets — each links ONLY the component(s) it tests
add_iot_test(test_hal  test_hal_sim.c  hal)
# Add more as you implement them:
add_iot_test(test_logger    test_logger.c           logger)       # Task 2
# add_iot_test(test_sensors   test_sensor_drv.c       drivers)      # Task 3
# add_iot_test(test_data      test_data_structures.c  data)         # Task 4
# add_iot_test(test_telemetry test_telemetry.c        telemetry)    # Task 5
# add_iot_test(test_app       test_app_logic.c        app alert)    # Task 6
# add_iot_test(test_config    test_config.c           config logger)# Task 7