#include "telemetry.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

static void test_telemetry_round_trip(void) {
  sensor_data_t original = {.temperature = 253, .humidity = 601};

  sensor_data_t decoded = {0};
  uint8_t buffer[14] = {0U};
  uint32_t written = 0U;

  status_t status =
      telemetry_serialize(&original, buffer, sizeof(buffer), &written);

  TEST_ASSERT_EQUAL(STATUS_OK, status);
  TEST_ASSERT_EQUAL_UINT32(14U, written);

  status = telemetry_deserialize(buffer, written, &decoded);

  TEST_ASSERT_EQUAL(STATUS_OK, status);
  TEST_ASSERT_EQUAL_INT16(original.temperature, decoded.temperature);
  TEST_ASSERT_EQUAL_INT16(original.humidity, decoded.humidity);
}

static void test_telemetry_corrupt_sync(void) {
  sensor_data_t original = {.temperature = 253, .humidity = 601};

  sensor_data_t decoded = {0};
  uint8_t buffer[14] = {0U};
  uint32_t written = 0U;

  status_t status =
      telemetry_serialize(&original, buffer, sizeof(buffer), &written);

  TEST_ASSERT_EQUAL(STATUS_OK, status);

  /* Corrupt the SYNC byte */
  buffer[0] = 0xBBU;

  status = telemetry_deserialize(buffer, written, &decoded);

  TEST_ASSERT_EQUAL(STATUS_ERR_INVALID_PARAM, status);
}

static void test_telemetry_corrupt_crc(void) {
  sensor_data_t original = {.temperature = 253, .humidity = 601};

  sensor_data_t decoded = {0};
  uint8_t buffer[14] = {0U};
  uint32_t written = 0U;

  status_t status =
      telemetry_serialize(&original, buffer, sizeof(buffer), &written);

  TEST_ASSERT_EQUAL(STATUS_OK, status);

  /* Corrupt temperature checksum */
  buffer[6] ^= 0x01U;

  status = telemetry_deserialize(buffer, written, &decoded);

  TEST_ASSERT_EQUAL(STATUS_ERR_INVALID_PARAM, status);
}

static void test_telemetry_buffer_too_small(void) {
  sensor_data_t original = {.temperature = 253, .humidity = 601};

  uint8_t buffer[10] = {0U};
  uint32_t written = 100U;

  status_t status =
      telemetry_serialize(&original, buffer, sizeof(buffer), &written);

  TEST_ASSERT_EQUAL(STATUS_ERR_INVALID_PARAM, status);
  TEST_ASSERT_EQUAL_UINT32(0U, written);
}

static void test_telemetry_null_pointers(void) {
  sensor_data_t data = {.temperature = 253, .humidity = 601};

  uint8_t buffer[14] = {0U};
  uint32_t written = 0U;

  /* NULL sensor data */
  TEST_ASSERT_EQUAL(
      STATUS_ERR_NULL_PTR,
      telemetry_serialize(NULL, buffer, sizeof(buffer), &written));

  /* NULL output buffer */
  TEST_ASSERT_EQUAL(STATUS_ERR_NULL_PTR,
                    telemetry_serialize(&data, NULL, sizeof(buffer), &written));

  /* NULL written pointer */
  TEST_ASSERT_EQUAL(STATUS_ERR_NULL_PTR,
                    telemetry_serialize(&data, buffer, sizeof(buffer), NULL));

  /* NULL input buffer */
  TEST_ASSERT_EQUAL(STATUS_ERR_NULL_PTR,
                    telemetry_deserialize(NULL, sizeof(buffer), &data));

  /* NULL decoded data */
  TEST_ASSERT_EQUAL(STATUS_ERR_NULL_PTR,
                    telemetry_deserialize(buffer, sizeof(buffer), NULL));
}

static void test_telemetry_oversized_data(void) {
  sensor_data_t original = {.temperature = 253, .humidity = 601};

  sensor_data_t decoded = {0};
  uint8_t buffer[20] = {0U};
  uint32_t written = 0U;

  status_t status =
      telemetry_serialize(&original, buffer, sizeof(buffer), &written);

  TEST_ASSERT_EQUAL(STATUS_OK, status);
  TEST_ASSERT_EQUAL_UINT32(14U, written);

  /* Pass an oversized packet length */
  status = telemetry_deserialize(buffer, sizeof(buffer), &decoded);

  TEST_ASSERT_EQUAL(STATUS_ERR_INVALID_PARAM, status);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_telemetry_round_trip);
  RUN_TEST(test_telemetry_corrupt_sync);
  RUN_TEST(test_telemetry_corrupt_crc);
  RUN_TEST(test_telemetry_buffer_too_small);
  RUN_TEST(test_telemetry_null_pointers);
  RUN_TEST(test_telemetry_oversized_data);

  return UNITY_END();
}