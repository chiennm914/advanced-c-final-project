#ifndef HAL_SIM_H
#define HAL_SIM_H

#include <stdint.h>

/** @brief Simulated register addresses. */
#define HAL_REG_TEMP_RAW    (0x00U)
#define HAL_REG_HUMI_RAW    (0x01U)
#define HAL_REG_STATUS      (0x02U)
#define HAL_REG_COUNT        (3U)

/**
 * @brief Initialize the HAL simulation.
 *
 * @param[in] seed Seed used for pseudo-random sensor generation.
 */
void hal_sim_init(uint32_t seed);

/**
 * @brief Read a simulated HAL register.
 *
 * @param[in] reg_addr Register address to read.
 * @return Value stored in the register.
 */
uint16_t hal_sim_read_register(uint8_t reg_addr);

/**
 * @brief Write a value to a simulated HAL register.
 *
 * @param[in] reg_addr Register address to write.
 * @param[in] value Value to store in the register.
 */
void hal_sim_write_register(uint8_t reg_addr, uint16_t value);

/**
 * @brief Update the simulated sensor register values.
 */
void hal_sim_update(void);

#endif /* HAL_SIM_H */

