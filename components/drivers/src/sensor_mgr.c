#include "sensor_mgr.h"
#include "contract.h"
#include "humi_drv.h"
#include "sensor_intf.h"
#include "temp_drv.h"

#include <stddef.h>

static const sensor_intf_t *s_sensors[SENSOR_NUM_TYPES];

status_t sensor_mgr_init(void) {
  s_sensors[SENSOR_TEMPERATURE] = temp_drv_get_interface();
  s_sensors[SENSOR_HUMIDITY] = humi_drv_get_interface();

  for (int i = 0U; i < SENSOR_NUM_TYPES; i++) {
    status_t status = s_sensors[i]->init();

    if (status != STATUS_OK) {
      return status;
    }
  }

  return STATUS_OK;
}

status_t sensor_mgr_read_all(sensor_data_t *p_data) {
  REQUIRE(p_data != NULL);
  // 1. Duyệt qua mảng s_sensors
  for (int i = 0U; i < SENSOR_NUM_TYPES; i++) {
    if (s_sensors[i] == NULL) {
      return STATUS_ERR_EMPTY;
    }
    int16_t *p_value = NULL;
    if (i == SENSOR_TEMPERATURE) {
      p_value = &p_data->temperature;
    } else {
      p_value = &p_data->humidity;
    }

    status_t status = s_sensors[i]->read(p_value);
    if (status != STATUS_OK) {
      return status;
    }
  }
  return STATUS_OK;
}