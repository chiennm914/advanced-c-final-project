#include "unity.h"
#include "hal_sim.h"
#include "temp_drv.h"
#include "humi_drv.h"
#include "sensor_mgr.h"

void setUp(void)
{
    hal_sim_init(123U);
}

void tearDown(void)
{
}

static void test_temp_driver_init(void) {
    const sensor_intf_t *sensor = temp_drv_get_interface();
    if (sensor == NULL) {
        TEST_FAIL_MESSAGE("Temperature interface is NULL");
        return;
    }
    TEST_ASSERT_EQUAL(STATUS_OK, sensor->init());
}

static void test_humi_driver_init(void)
{
    const sensor_intf_t *sensor = humi_drv_get_interface();

    if (sensor == NULL) {
        TEST_FAIL_MESSAGE("Humidity interface is NULL");
        return;
    }
    TEST_ASSERT_EQUAL(STATUS_OK, sensor->init());
}

static void test_temp_driver_read(void)
{
    const sensor_intf_t *sensor = temp_drv_get_interface();
    int16_t temperature = 0;

    hal_sim_write_register(HAL_REG_TEMP_RAW, 253U);

    TEST_ASSERT_EQUAL(STATUS_OK, sensor->init());
    TEST_ASSERT_EQUAL(STATUS_OK, sensor->read(&temperature));

    TEST_ASSERT_EQUAL_INT16(253, temperature);
}

static void test_humi_driver_read(void)
{
    const sensor_intf_t *sensor = humi_drv_get_interface();
    int16_t humidity = 0;

    hal_sim_write_register(HAL_REG_HUMI_RAW, 601U);

    TEST_ASSERT_EQUAL(STATUS_OK, sensor->init());
    TEST_ASSERT_EQUAL(STATUS_OK, sensor->read(&humidity));

    TEST_ASSERT_EQUAL_INT16(601, humidity);
}

static void test_sensor_manager_read_all(void)
{
    sensor_data_t data = {0};

    hal_sim_write_register(HAL_REG_TEMP_RAW, 253U);
    hal_sim_write_register(HAL_REG_HUMI_RAW, 601U);

    TEST_ASSERT_EQUAL(STATUS_OK, sensor_mgr_init());
    TEST_ASSERT_EQUAL(STATUS_OK, sensor_mgr_read_all(&data));

    TEST_ASSERT_EQUAL_INT16(253, data.temperature);
    TEST_ASSERT_EQUAL_INT16(601, data.humidity);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_temp_driver_init);
    RUN_TEST(test_humi_driver_init);
    RUN_TEST(test_temp_driver_read);
    RUN_TEST(test_humi_driver_read);
    RUN_TEST(test_sensor_manager_read_all);

    return UNITY_END();
}