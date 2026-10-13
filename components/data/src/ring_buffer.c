#include "ring_buffer.h"
#include "contract.h"
#include <stddef.h>

status_t ring_init(ring_buffer_t *p_rb)
{
    REQUIRE(p_rb != NULL);

    p_rb->head = 0U;
    p_rb->tail = 0U;
    p_rb->count = 0U;
    p_rb->capacity = RING_BUFFER_CAPACITY;

    return STATUS_OK;
}

status_t ring_push(ring_buffer_t *p_rb, int16_t value) {
    REQUIRE(p_rb != NULL);
    INVARIANT(p_rb->count <= p_rb->capacity);
    if (p_rb->count == p_rb->capacity) {
        return STATUS_ERR_FULL;
    }
    p_rb->data[p_rb->head] = value;
    p_rb->head = (p_rb->head + 1U) % p_rb->capacity;
    p_rb->count++;
    INVARIANT(p_rb->count <= p_rb ->capacity);

    return STATUS_OK;
}

status_t ring_pop(ring_buffer_t *p_rb, int16_t *p_value) {
    REQUIRE(p_rb != NULL);
    REQUIRE(p_value != NULL);
    INVARIANT(p_rb->count <= p_rb->capacity);

    if (p_rb->count == 0) {
        return STATUS_ERR_EMPTY;
    }
    *p_value = p_rb->data[p_rb->tail];
    p_rb->tail = (p_rb->tail +1U) % p_rb->capacity;

    p_rb->count--;
    return STATUS_OK;
}

bool ring_is_empty(const ring_buffer_t *p_rb) {
    REQUIRE(p_rb != NULL);
    return (p_rb->count == 0U);
}

bool ring_is_full(const ring_buffer_t *p_rb) {
    REQUIRE(p_rb != NULL);
    return (p_rb->count == p_rb->capacity);
}

status_t ring_peek(const ring_buffer_t *p_rb, int16_t *p_value)
{
    REQUIRE(p_rb != NULL);
    REQUIRE(p_value != NULL);

    if (p_rb->count == 0U) {
        return STATUS_ERR_EMPTY;
    }

    *p_value = p_rb->data[p_rb->tail];

    return STATUS_OK;
}

uint32_t ring_count(const ring_buffer_t *p_rb)
{
    REQUIRE(p_rb != NULL);

    return p_rb->count;
}



