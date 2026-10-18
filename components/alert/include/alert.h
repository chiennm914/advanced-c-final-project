
#ifndef ALERT_H
#define ALERT_H

#include <stdbool.h>
#include <stdint.h>

#include "sensor_mgr.h"

/**
 * @file alert.h
 * @brief Sensor threshold alert interface.
 */

/**
 * @brief Alert threshold configuration.
 */
typedef struct
{
    int16_t temp_max;
    int16_t temp_min;
    int16_t humi_max;
    int16_t humi_min;
} alert_config_t;

/**
 * @brief Initialize alert thresholds.
 *
 * @param[in] p_config Pointer to threshold configuration.
 */
void alert_init(const alert_config_t *p_config);

/**
 * @brief Check sensor readings against thresholds.
 *
 * @param[in] p_data Pointer to sensor readings.
 * @return true if an alert condition is detected.
 */
bool alert_check(const sensor_data_t *p_data);

/**
 * @brief Acknowledge and clear the active alert.
 */
void alert_acknowledge(void);

/**
 * @brief Check whether an alert is active.
 *
 * @return true if an alert is currently active.
 */
bool alert_is_active(void);
bool alert_get_config(alert_config_t *p_config);

#endif /* ALERT_H */
