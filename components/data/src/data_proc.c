#include "data_proc.h"
#include "contract.h"

#include <stddef.h>

status_t data_proc_init(data_proc_t *p_proc, uint8_t window_size)
{
    REQUIRE(p_proc != NULL);

    if (window_size == 0U) {
        window_size = DATA_PROC_DEFAULT_WINDOW;
    }

    if (window_size > RING_BUFFER_CAPACITY) {
        return STATUS_ERR_INVALID_PARAM;
    }

    status_t status = ring_init(&p_proc->buffer);

    if (status != STATUS_OK) {
        return status;
    }

    p_proc->window_size = window_size;
    p_proc->running_sum = 0;

    return STATUS_OK;
}

status_t data_proc_add_sample(data_proc_t *p_proc, int16_t sample)
{
    REQUIRE(p_proc != NULL);
    REQUIRE(p_proc->window_size > 0U);
    REQUIRE(p_proc->window_size <= RING_BUFFER_CAPACITY);

    int16_t oldest = 0;
    status_t status;

    if (ring_count(&p_proc->buffer) >= p_proc->window_size) {
        status = ring_pop(&p_proc->buffer, &oldest);

        if (status != STATUS_OK) {
            return status;
        }

        p_proc->running_sum -= oldest;
    }

    status = ring_push(&p_proc->buffer, sample);

    if (status != STATUS_OK) {
        return status;
    }

    p_proc->running_sum += sample;

    return STATUS_OK;
}

int16_t data_proc_get_average(const data_proc_t *p_proc)
{
    REQUIRE(p_proc != NULL);

    uint32_t count = ring_count(&p_proc->buffer);

    if (count == 0U) {
        return 0;
    }

    return (int16_t)(p_proc->running_sum / (int32_t)count);
}

