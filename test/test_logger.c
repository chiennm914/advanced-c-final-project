#include "logger.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

static void test_logger_filters_below_min_level(void){
    FILE *p_file;
    char buffer[256] = {0};

    logger_init(LOG_LEVEL_WARN, "test_logger.log");

    LOG_INFO("This should not appear");
    LOG_WARN("This should appear");

    logger_close();

    p_file = fopen("test_logger.log","r");
    TEST_ASSERT_NOT_NULL(p_file);

    if (p_file != NULL) {
        (void)fgets(buffer, sizeof(buffer), p_file);
        (void)fclose(p_file);
    }

    TEST_ASSERT_NULL(strstr(buffer,"This should not appear"));
    TEST_ASSERT_NOT_NULL(strstr(buffer,"This should appear"));
}

static void test_logger_formats_message(void)
{
    FILE *p_file;
    char buffer[256] = {0};

    logger_init(LOG_LEVEL_DEBUG, "test_logger.log");

    logger_log(LOG_LEVEL_INFO,
               "test_file.c",
               123U,
               "test_function",
               "Temperature = %d",
               30);

    logger_close();

    p_file = fopen("test_logger.log", "r");
    TEST_ASSERT_NOT_NULL(p_file);

    if (p_file != NULL) {
        (void)fgets(buffer, sizeof(buffer), p_file);
        (void)fclose(p_file);
    }

    TEST_ASSERT_NOT_NULL(
        strstr(buffer,
               "[INFO] test_file.c:123 (test_function): Temperature = 30"));
}

static void test_logger_writes_to_file(void)
{
    FILE *p_file;
    char buffer[256] = {0};

    logger_init(LOG_LEVEL_DEBUG, "test_logger.log");

    LOG_ERROR("Sensor failure");

    logger_close();

    p_file = fopen("test_logger.log", "r");
    TEST_ASSERT_NOT_NULL(p_file);

    if (p_file != NULL) {
        (void)fgets(buffer, sizeof(buffer), p_file);
        (void)fclose(p_file);
    }

    TEST_ASSERT_NOT_NULL(strstr(buffer, "Sensor failure"));
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_logger_filters_below_min_level);
    RUN_TEST(test_logger_formats_message);
    RUN_TEST(test_logger_writes_to_file);
    return UNITY_END();
}