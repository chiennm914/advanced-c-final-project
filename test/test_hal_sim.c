#include "unity.h"
#include <stdint.h>
#include "hal_sim.h"
#include "hal_timer.h"

void setUp(void)
{
}

void tearDown(void)
{
}

static void test_hal_register_read_write(void)
{
    hal_sim_init(1234U);

    hal_sim_write_register(HAL_REG_TEMP_RAW, 250U);

    TEST_ASSERT_EQUAL_UINT16(250U, hal_sim_read_register(HAL_REG_TEMP_RAW));

}

static void test_hal_timer_increment(void)
{
    hal_timer_init();

    TEST_ASSERT_EQUAL_UINT32(0U, hal_timer_get_tick());

    hal_timer_tick();
    TEST_ASSERT_EQUAL_UINT32(1U, hal_timer_get_tick());

    hal_timer_tick();
    TEST_ASSERT_EQUAL_UINT32(2U, hal_timer_get_tick());
}

static void test_hal_init_seed(void)
{
    uint16_t first_temp;
    uint16_t second_temp;

    hal_sim_init(1234U);
    hal_sim_update();
    first_temp = hal_sim_read_register(HAL_REG_TEMP_RAW);

    hal_sim_init(1234U);
    hal_sim_update();
    second_temp = hal_sim_read_register(HAL_REG_TEMP_RAW);

    TEST_ASSERT_EQUAL_UINT16(first_temp, second_temp);
}

static void test_hal_sim_update_changes_values(void)
{
    uint16_t temp_before;
    uint16_t humi_before;
    uint16_t temp_after;
    uint16_t humi_after;

    hal_sim_init(42U);

    temp_before = hal_sim_read_register(HAL_REG_TEMP_RAW);
    humi_before = hal_sim_read_register(HAL_REG_HUMI_RAW);

    hal_sim_update();

    temp_after = hal_sim_read_register(HAL_REG_TEMP_RAW);
    humi_after = hal_sim_read_register(HAL_REG_HUMI_RAW);

    TEST_ASSERT_TRUE(
        (temp_after != temp_before) ||
        (humi_after != humi_before)
    );
}

static void test_hal_timer_init_resets(void)
{
    hal_timer_init();

    hal_timer_tick();
    hal_timer_tick();

    TEST_ASSERT_EQUAL_UINT32(2U, hal_timer_get_tick());

    hal_timer_init();

    TEST_ASSERT_EQUAL_UINT32(0U, hal_timer_get_tick());
}

int main(void)
{
    UNITY_BEGIN(); /* NOLINT(misc-include-cleaner) */

    RUN_TEST(test_hal_register_read_write); /* NOLINT(misc-include-cleaner) */
    RUN_TEST(test_hal_timer_increment);
    RUN_TEST(test_hal_init_seed);
    RUN_TEST(test_hal_sim_update_changes_values);
    RUN_TEST(test_hal_timer_init_resets);
    return UNITY_END(); /* NOLINT(misc-include-cleaner) */
}