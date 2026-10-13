#ifndef DATA_PROC_H
#define DATA_PROC_H

#include <stdint.h>
#include "common_types.h"
#include "ring_buffer.h"

#define DATA_PROC_DEFAULT_WINDOW 10U

typedef struct {
    ring_buffer_t buffer;
    uint8_t window_size;
    int32_t running_sum;
} data_proc_t;

/**
 * @brief Initialize the moving average filter.
 * @param p_proc Pointer to filter instance.
 * @param window_size Number of samples in the window.
 * @return STATUS_OK on success, error status otherwise.
 */
status_t data_proc_init(data_proc_t *p_proc, uint8_t window_size);

/**
 * @brief Add a sample to the moving average filter.
 */
status_t data_proc_add_sample(data_proc_t *p_proc, int16_t sample);

/**
 * @brief Get the current moving average.
 */
int16_t data_proc_get_average(const data_proc_t *p_proc);

#endif /* DATA_PROC_H */