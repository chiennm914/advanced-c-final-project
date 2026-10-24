
#include "alert.h"

#include <stddef.h>

#include "logger.h"

static alert_config_t s_alert_config;
static bool s_alert_active = false;
static bool s_alert_initialized = false;

void alert_init(const alert_config_t *p_config) {
  if (p_config == NULL) {
    LOG_ERROR("Alert configuration is NULL");
    return;
  }

  if ((p_config->temp_min > p_config->temp_max) ||
      (p_config->humi_min > p_config->humi_max)) {
    LOG_ERROR("Invalid alert thresholds");
    return;
  }

  s_alert_config = *p_config;
  s_alert_active = false;
  s_alert_initialized = true;
}

bool alert_check(const sensor_data_t *p_data) {
  if ((p_data == NULL) || (s_alert_initialized == false)) {
    return false;
  }

  bool exceeded = (p_data->temperature > s_alert_config.temp_max) ||
                  (p_data->temperature < s_alert_config.temp_min) ||
                  (p_data->humidity > s_alert_config.humi_max) ||
                  (p_data->humidity < s_alert_config.humi_min);

  if (exceeded) {
    if (s_alert_active == false) {
      LOG_WARN("Sensor threshold exceeded");
    }

    s_alert_active = true;
  }

  return exceeded;
}

void alert_acknowledge(void) { s_alert_active = false; }

bool alert_is_active(void) { return s_alert_active; }

bool alert_get_config(alert_config_t *p_config) {
  if ((p_config == NULL) || (s_alert_initialized == false)) {
    return false;
  }

  *p_config = s_alert_config;

  return true;
}
