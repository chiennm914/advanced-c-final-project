#include "unity.h"
#include "ring_buffer.h"
#include "data_proc.h"
#include "event_log.h"

void setUp(void)
{
    /* No setup required */
}

void tearDown(void)
{
    /* No cleanup required */
}
static void test_ring_fifo(void)
{
    ring_buffer_t rb;
    int16_t value = 0;

    TEST_ASSERT_EQUAL(STATUS_OK, ring_init(&rb));

    TEST_ASSERT_EQUAL(STATUS_OK, ring_push(&rb, 100));
    TEST_ASSERT_EQUAL(STATUS_OK, ring_push(&rb, 200));
    TEST_ASSERT_EQUAL(STATUS_OK, ring_push(&rb, 300));

    TEST_ASSERT_EQUAL(STATUS_OK, ring_pop(&rb, &value));
    TEST_ASSERT_EQUAL_INT16(100, value);

    TEST_ASSERT_EQUAL(STATUS_OK, ring_pop(&rb, &value));
    TEST_ASSERT_EQUAL_INT16(200, value);

    TEST_ASSERT_EQUAL(STATUS_OK, ring_pop(&rb, &value));
    TEST_ASSERT_EQUAL_INT16(300, value);

    TEST_ASSERT_TRUE(ring_is_empty(&rb));
}

static void test_ring_empty(void)
{
    ring_buffer_t rb;
    int16_t value = 0;

    ring_init(&rb);

    TEST_ASSERT_TRUE(ring_is_empty(&rb));
    TEST_ASSERT_EQUAL(STATUS_ERR_EMPTY, ring_pop(&rb, &value));
    TEST_ASSERT_EQUAL(0U, ring_count(&rb));
}

static void test_ring_full(void)
{
    ring_buffer_t rb;
    ring_init(&rb);

    for (uint32_t i = 0U; i < RING_BUFFER_CAPACITY; i++) {
        TEST_ASSERT_EQUAL(STATUS_OK, ring_push(&rb, (int16_t)i));
    }

    TEST_ASSERT_TRUE(ring_is_full(&rb));
    TEST_ASSERT_EQUAL(STATUS_ERR_FULL, ring_push(&rb, 999));
    TEST_ASSERT_EQUAL(RING_BUFFER_CAPACITY, ring_count(&rb));
}

static void test_ring_wrap_around(void) {
    ring_buffer_t rb;
    int16_t value = 0;
    ring_init(&rb);
    for (uint32_t i = 0U; i < RING_BUFFER_CAPACITY; i++) {
        TEST_ASSERT_EQUAL(STATUS_OK, ring_push(&rb, (int16_t)i));
    }
    TEST_ASSERT_TRUE(ring_is_full(&rb));
    TEST_ASSERT_EQUAL(STATUS_OK, ring_pop(&rb, &value));
    TEST_ASSERT_EQUAL_INT16(0, value);

    TEST_ASSERT_EQUAL(STATUS_OK, ring_pop(&rb, &value));
    TEST_ASSERT_EQUAL_INT16(1, value);
    TEST_ASSERT_EQUAL(2U, rb.tail);
    TEST_ASSERT_EQUAL(14U, ring_count(&rb));
    TEST_ASSERT_EQUAL(STATUS_OK, ring_push(&rb, 100));
    TEST_ASSERT_EQUAL(STATUS_OK, ring_push(&rb, 200));
    TEST_ASSERT_EQUAL(2U, rb.head);
    TEST_ASSERT_EQUAL(16U, ring_count(&rb));
    TEST_ASSERT_TRUE(ring_is_full(&rb));
}

static void test_data_proc_startup_phase(void)
{
    data_proc_t proc;

    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_init(&proc, 4U));

    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 100));
    TEST_ASSERT_EQUAL_INT16(100, data_proc_get_average(&proc));

    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 200));
    TEST_ASSERT_EQUAL_INT16(150, data_proc_get_average(&proc));

    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 300));
    TEST_ASSERT_EQUAL_INT16(200, data_proc_get_average(&proc));
}


static void test_data_proc_steady_state(void)
{
    data_proc_t proc;

    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_init(&proc, 4U));

    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 100));
    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 200));
    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 300));
    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 400));

    TEST_ASSERT_EQUAL_INT16(250, data_proc_get_average(&proc));

    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 500));

    TEST_ASSERT_EQUAL_INT16(350, data_proc_get_average(&proc));
}

static void test_data_proc_spike_smoothing(void)
{
    data_proc_t proc;

    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_init(&proc, 4U));

    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 100));
    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 100));
    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 100));
    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 100));

    TEST_ASSERT_EQUAL_INT16(100, data_proc_get_average(&proc));

    TEST_ASSERT_EQUAL(STATUS_OK, data_proc_add_sample(&proc, 500));

    TEST_ASSERT_EQUAL_INT16(200, data_proc_get_average(&proc));
}

static void test_event_log_sorted_insert(void)
{
    event_log_init();

    event_log_add(EVENT_SYSTEM_RESET, 100U, 10);
    event_log_add(EVENT_SENSOR_ERROR, 300U, 30);
    event_log_add(EVENT_ALERT_TEMP_HIGH, 200U, 20);

    TEST_ASSERT_EQUAL_UINT32(3U, event_log_get_count());

    TEST_ASSERT_EQUAL_UINT32(300U, event_log_get_at(0U)->timestamp);
    TEST_ASSERT_EQUAL_UINT32(200U, event_log_get_at(1U)->timestamp);
    TEST_ASSERT_EQUAL_UINT32(100U, event_log_get_at(2U)->timestamp);
}

static void test_event_log_bounded_capacity(void)
{
    event_log_init();

    for (uint32_t i = 1U; i <= EVENT_LOG_MAX_ENTRIES; i++)
    {
        event_log_add(EVENT_SENSOR_ERROR, i, 0);
    }

    TEST_ASSERT_EQUAL_UINT32(EVENT_LOG_MAX_ENTRIES,
                             event_log_get_count());

    event_log_add(EVENT_ALERT_TEMP_HIGH, 100U, 45);

    TEST_ASSERT_EQUAL_UINT32(EVENT_LOG_MAX_ENTRIES,
                             event_log_get_count());

    TEST_ASSERT_EQUAL_UINT32(100U, event_log_get_at(0U)->timestamp);
    TEST_ASSERT_EQUAL_UINT32(2U,
                             event_log_get_at(EVENT_LOG_MAX_ENTRIES - 1U)->timestamp);

    TEST_ASSERT_NULL(event_log_get_at(EVENT_LOG_MAX_ENTRIES));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_ring_fifo);
    RUN_TEST(test_ring_empty);
    RUN_TEST(test_ring_full);
    RUN_TEST(test_ring_wrap_around);

    RUN_TEST(test_data_proc_startup_phase);
    RUN_TEST(test_data_proc_steady_state);
    RUN_TEST(test_data_proc_spike_smoothing);

    RUN_TEST(test_event_log_sorted_insert);
    RUN_TEST(test_event_log_bounded_capacity);
    return UNITY_END();
}
