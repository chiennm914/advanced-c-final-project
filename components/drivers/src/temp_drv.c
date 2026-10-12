#include "temp_drv.h"

#include "hal_sim.h"
#include "contract.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Internal temperature driver configuration.
 */
typedef struct {
    bool is_initialized;
    int16_t offset_cal;
} temp_drv_config_t;

static temp_drv_config_t s_config = {
    .is_initialized = false,
    .offset_cal = 0
};

static status_t temp_init(void)
{
    s_config.is_initialized = true;
    return STATUS_OK;
}

static status_t temp_read(int16_t *p_value)
{
    REQUIRE(p_value != NULL);

    // 1. Kiểm tra driver đã khởi tạo chưa
    if (s_config.is_initialized == false) {
        return STATUS_ERR_EMPTY;
    }
    // 2. Đọc dữ liệu từ HAL_REG_TEMP_RAW
    uint16_t raw = hal_sim_read_register(HAL_REG_TEMP_RAW);
    // 3. Chuyển dữ liệu sang fixed-point x10
    //    và cộng offset_cal
    int32_t calibrated = (int32_t)raw + (int32_t)s_config.offset_cal;
    // 4. Gán kết quả vào *p_value
    if ((calibrated < INT16_MIN) || (calibrated > INT16_MAX)) {
        return STATUS_ERR_INVALID_PARAM;
    }
    *p_value = (int16_t)calibrated;
    // 5. Trả về STATUS_OK
    return STATUS_OK;
}

static const char *temp_get_name(void){
    return "Temperature";
}

static const sensor_intf_t s_temp_intf = {
    .init = temp_init,
    .read = temp_read,
    .get_name = temp_get_name
};

const sensor_intf_t *temp_drv_get_interface(void)
{
    return &s_temp_intf;
}