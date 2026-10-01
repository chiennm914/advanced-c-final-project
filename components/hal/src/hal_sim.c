#include "hal_sim.h"
#include <stdint.h>
#include <stdlib.h>

static volatile uint16_t s_registers[HAL_REG_COUNT];

void hal_sim_init(uint32_t seed)
{
    srand((unsigned int)seed);

    s_registers[HAL_REG_TEMP_RAW] = 0U;
    s_registers[HAL_REG_HUMI_RAW] = 0U;
    s_registers[HAL_REG_STATUS] = 0U;
}

uint16_t hal_sim_read_register(uint8_t reg_addr)
{
    if (reg_addr >= HAL_REG_COUNT) {
        return 0U;
    }

    return s_registers[reg_addr];
}

void hal_sim_write_register(uint8_t reg_addr, uint16_t value)
{
    if (reg_addr < HAL_REG_COUNT) {
        s_registers[reg_addr] = value;
    }
}

void hal_sim_update(void)
{
    uint16_t temp_raw;
    uint16_t humi_raw;

    temp_raw = (uint16_t)(200U + ((unsigned int)rand() % 151U)); /* NOLINT(cert-msc30-c,cert-msc50-cpp,misc-predictable-rand) */
    humi_raw = (uint16_t)(400U + ((unsigned int)rand() % 401U)); /* NOLINT(cert-msc30-c,cert-msc50-cpp,misc-predictable-rand) */

    s_registers[HAL_REG_TEMP_RAW] = temp_raw;
    s_registers[HAL_REG_HUMI_RAW] = humi_raw;
    s_registers[HAL_REG_STATUS] = 1U;
}