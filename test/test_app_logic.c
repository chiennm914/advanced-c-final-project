
#include "unity.h"

#include "app_fsm.h"
#include "cli_cmd.h"
#include "alert.h"

#include <stdbool.h>
#include <stdint.h>

void setUp(void)
{
}

void tearDown(void)
{
}

/* ================= FSM TESTS ================= */

static void test_fsm_init_to_idle(void)
{
    system_state_t result = fsm_run(SYS_INIT, EVT_INIT_OK);

    TEST_ASSERT_EQUAL(SYS_IDLE, result);
}

static void test_fsm_init_fail_to_error(void)
{
    system_state_t result = fsm_run(SYS_INIT, EVT_INIT_FAIL);

    TEST_ASSERT_EQUAL(SYS_ERROR, result);
}

static void test_fsm_idle_to_monitoring(void)
{
    system_state_t result = fsm_run(SYS_IDLE, EVT_CMD_START);

    TEST_ASSERT_EQUAL(SYS_MONITORING, result);
}

static void test_fsm_monitoring_to_alert(void)
{
    system_state_t result =
        fsm_run(SYS_MONITORING, EVT_ALERT_TRIGGERED);

    TEST_ASSERT_EQUAL(SYS_ALERT, result);
}

static void test_fsm_alert_to_monitoring(void)
{
    system_state_t result = fsm_run(SYS_ALERT, EVT_ALERT_ACK);

    TEST_ASSERT_EQUAL(SYS_MONITORING, result);
}

static void test_fsm_invalid_state(void)
{
    system_state_t result =
        fsm_run((system_state_t)SYS_NUM_STATES, EVT_CMD_START);

    TEST_ASSERT_EQUAL(SYS_ERROR, result);
}

/* ================= CLI TESTS ================= */

static void test_cli_known_command(void)
{
    app_context_t app = {
        .current_state = SYS_IDLE,
        .quit_requested = false
    };

    status_t result = cli_dispatch("start", &app);

    TEST_ASSERT_EQUAL(STATUS_OK, result);
    TEST_ASSERT_EQUAL(SYS_MONITORING, app.current_state);
}

static void test_cli_unknown_command(void)
{
    app_context_t app = {
        .current_state = SYS_IDLE,
        .quit_requested = false
    };

    status_t result = cli_dispatch("invalid_command", &app);

    TEST_ASSERT_EQUAL(STATUS_ERR_INVALID_PARAM, result);
    TEST_ASSERT_EQUAL(SYS_IDLE, app.current_state);
}

/* ================= ALERT TESTS ================= */

static void test_alert_threshold(void)
{
    alert_config_t config = {
        .temp_max = 350,
        .temp_min = 150,
        .humi_max = 800,
        .humi_min = 300
    };

    sensor_data_t data = {0};

    alert_init(&config);

    data.temperature = 400;
    data.humidity = 500;

    bool result = alert_check(&data);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(alert_is_active());
}

static void test_alert_acknowledge(void)
{
    alert_config_t config = {
        .temp_max = 350,
        .temp_min = 150,
        .humi_max = 800,
        .humi_min = 300
    };

    sensor_data_t data = {0};

    alert_init(&config);

    data.temperature = 400;
    data.humidity = 500;

    (void)alert_check(&data);

    TEST_ASSERT_TRUE(alert_is_active());

    alert_acknowledge();

    TEST_ASSERT_FALSE(alert_is_active());
}

/* ================= UNITY ENTRY ================= */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_fsm_init_to_idle);
    RUN_TEST(test_fsm_init_fail_to_error);
    RUN_TEST(test_fsm_idle_to_monitoring);
    RUN_TEST(test_fsm_monitoring_to_alert);
    RUN_TEST(test_fsm_alert_to_monitoring);
    RUN_TEST(test_fsm_invalid_state);

    RUN_TEST(test_cli_known_command);
    RUN_TEST(test_cli_unknown_command);

    RUN_TEST(test_alert_threshold);
    RUN_TEST(test_alert_acknowledge);

    return UNITY_END();
}
