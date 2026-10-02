#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include <stdint.h>

/**
 * @brief Initialize the simulated timer.
 *
 * Resets the tick counter to zero.
 */
void hal_timer_init(void);

/**
 * @brief Get the current timer tick.
 *
 * @return Current tick counter value.
 */
uint32_t hal_timer_get_tick(void);

/**
 * @brief Increment the timer tick counter by one.
 */
void hal_timer_tick(void);

#endif /* HAL_TIMER_H */