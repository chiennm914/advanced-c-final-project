
#include "config.h"
#include "unity.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TEST_CONFIG_FILE "test_config_temp.txt"

void setUp(void) { (void)remove(TEST_CONFIG_FILE); }

void tearDown(void) { (void)remove(TEST_CONFIG_FILE); }

static void write_test_file(const char *p_content) {
  FILE *p_file = fopen(TEST_CONFIG_FILE, "w");

  if (p_file == NULL) {
    TEST_FAIL_MESSAGE("Cannot open test config file");
    return;
  }

  size_t length = strlen(p_content);

  size_t written = fwrite(p_content, 1U, length, p_file);

  int close_result = fclose(p_file);

  TEST_ASSERT_EQUAL_size_t(length, written);
  TEST_ASSERT_EQUAL_INT(0, close_result);
}

static void test_config_save_load_round_trip(void) {
  alert_config_t original = {
      .temp_max = 400, .temp_min = 100, .humi_max = 900, .humi_min = 300};

  alert_config_t loaded = {0};

  TEST_ASSERT_EQUAL(STATUS_OK, config_save(TEST_CONFIG_FILE, &original));

  TEST_ASSERT_EQUAL(STATUS_OK, config_load(TEST_CONFIG_FILE, &loaded));

  TEST_ASSERT_EQUAL_INT16(original.temp_max, loaded.temp_max);
  TEST_ASSERT_EQUAL_INT16(original.temp_min, loaded.temp_min);
  TEST_ASSERT_EQUAL_INT16(original.humi_max, loaded.humi_max);
  TEST_ASSERT_EQUAL_INT16(original.humi_min, loaded.humi_min);
}

static void test_config_missing_file_uses_defaults(void) {
  alert_config_t config = {0};

  TEST_ASSERT_EQUAL(STATUS_OK, config_load(TEST_CONFIG_FILE, &config));

  TEST_ASSERT_EQUAL_INT16(350, config.temp_max);
  TEST_ASSERT_EQUAL_INT16(50, config.temp_min);
  TEST_ASSERT_EQUAL_INT16(800, config.humi_max);
  TEST_ASSERT_EQUAL_INT16(200, config.humi_min);
}

static void test_config_malformed_line_skipped(void) {
  write_test_file("temp_max=420\n"
                  "invalid_line\n"
                  "temp_min=abc\n"
                  "humi_max=850\n");

  alert_config_t config = {0};

  TEST_ASSERT_EQUAL(STATUS_OK, config_load(TEST_CONFIG_FILE, &config));

  TEST_ASSERT_EQUAL_INT16(420, config.temp_max);
  TEST_ASSERT_EQUAL_INT16(50, config.temp_min);
  TEST_ASSERT_EQUAL_INT16(850, config.humi_max);
}

static void test_config_out_of_range_clamped(void) {
  write_test_file("temp_max=50000\n"
                  "temp_min=-50000\n");

  alert_config_t config = {0};

  TEST_ASSERT_EQUAL(STATUS_OK, config_load(TEST_CONFIG_FILE, &config));

  TEST_ASSERT_EQUAL_INT16(INT16_MAX, config.temp_max);
  TEST_ASSERT_EQUAL_INT16(INT16_MIN, config.temp_min);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_config_save_load_round_trip);
  RUN_TEST(test_config_missing_file_uses_defaults);
  RUN_TEST(test_config_malformed_line_skipped);
  RUN_TEST(test_config_out_of_range_clamped);

  return UNITY_END();
}
