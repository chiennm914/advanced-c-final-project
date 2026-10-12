#ifndef SENSOR_MGR_H
#define SENSOR_MGR_H

#include "common_types.h"
#include <stdint.h>

/**
 * @brief Sensor readings in fixed-point format (x10).
 */
typedef struct {
    int16_t temperature;
    int16_t humidity;
} sensor_data_t;

/**
 * @brief Initialize and register all sensor drivers.
 *
 * @return STATUS_OK on success, error status otherwise.
 */
status_t sensor_mgr_init(void);

/**
 * @brief Read values from all registered sensors.
 *
 * @param[out] p_data Pointer to sensor data output.
 * @return STATUS_OK on success, error status otherwise.
 */
status_t sensor_mgr_read_all(sensor_data_t *p_data);

#endif /* SENSOR_MGR_H */