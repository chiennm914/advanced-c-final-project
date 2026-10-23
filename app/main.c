
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "alert.h"
#include "app_fsm.h"
#include "cli_cmd.h"
#include "common_types.h"
#include "config.h"
#include "data_proc.h"
#include "hal_sim.h"
#include "hal_timer.h"
#include "logger.h"
#include "sensor_mgr.h"
#include "telemetry.h"

#define CLI_INPUT_SIZE 128U
#define TELEMETRY_BUFFER_SIZE 32U

static status_t app_monitor_sample(data_proc_t *p_temp_filter,
                                   data_proc_t *p_humi_filter,
                                   app_context_t *p_app) {
  sensor_data_t data = {0};
  uint8_t packet[TELEMETRY_BUFFER_SIZE] = {0};
  uint32_t written = 0U;

  hal_sim_update();
  hal_timer_tick();

  if (sensor_mgr_read_all(&data) != STATUS_OK) {
    p_app->current_state = fsm_run(p_app->current_state, EVT_SENSOR_FAIL);

    LOG_ERROR("Sensor read failed");
    return STATUS_ERR_IO;
  }

  if ((data_proc_add_sample(p_temp_filter, data.temperature) != STATUS_OK) ||
      (data_proc_add_sample(p_humi_filter, data.humidity) != STATUS_OK)) {
    LOG_ERROR("Data processing failed");
    p_app->current_state = fsm_run(p_app->current_state, EVT_SENSOR_FAIL);
    return STATUS_ERR_IO;
  }

  if (alert_check(&data)) {
    p_app->current_state = fsm_run(p_app->current_state, EVT_ALERT_TRIGGERED);
  }

  if (telemetry_serialize(&data, packet, (uint32_t)sizeof(packet), &written) !=
      STATUS_OK) {
    LOG_ERROR("Telemetry serialization failed");
    return STATUS_ERR_IO;
  }

  LOG_INFO("Temp=%d.%dC Humi=%d.%d%% tick=%lu", (int)(data.temperature / 10),
           (int)((data.temperature % 10 < 0) ? -(data.temperature % 10)
                                             : (data.temperature % 10)),
           (int)(data.humidity / 10), (int)(data.humidity % 10),
           (unsigned long)hal_timer_get_tick());

  telemetry_print_hex(packet, written);

  return STATUS_OK;
}

int main(void) {
  app_context_t app = {.current_state = SYS_INIT, .quit_requested = false};

  alert_config_t config = {0};
  data_proc_t temp_filter = {0};
  data_proc_t humi_filter = {0};
  char input[CLI_INPUT_SIZE];

  logger_init(LOG_LEVEL_INFO, "iot_monitor.log");
  hal_sim_init(42U);
  hal_timer_init();

  if (config_load("config.txt", &config) != STATUS_OK) {
    LOG_ERROR("Config load failed");
    logger_close();
    return 1;
  }

  alert_init(&config);

  if (sensor_mgr_init() != STATUS_OK) {
    LOG_ERROR("Sensor initialization failed");
    logger_close();
    return 1;
  }

  if ((data_proc_init(&temp_filter, DATA_PROC_DEFAULT_WINDOW) != STATUS_OK) ||
      (data_proc_init(&humi_filter, DATA_PROC_DEFAULT_WINDOW) != STATUS_OK)) {
    LOG_ERROR("Data processing initialization failed");
    logger_close();
    return 1;
  }

  app.current_state = fsm_run(SYS_INIT, EVT_INIT_OK);

  printf("=== IoT Environmental Monitoring System v1.0.0 ===\n");
  printf("Type 'help' to see available commands.\n");

  while (!app.quit_requested) {
    printf("IoT> ");

    if (fflush(stdout) == EOF) {
      LOG_ERROR("Failed to flush stdout");
      break;
    }

    if (fgets(input, sizeof(input), stdin) == NULL) {
      break;
    }

    if ((strchr(input, '\n') == NULL) &&
        (strlen(input) == sizeof(input) - 1U))
    {
        int ch;

        do
        {
            ch = getchar();
        } while ((ch != '\n') && (ch != EOF));
    }

    (void)cli_dispatch(input, &app);

    if (app.current_state == SYS_MONITORING) {
      (void)app_monitor_sample(&temp_filter, &humi_filter, &app);
    }
  }

  LOG_INFO("Shutting down");
  logger_close();

  printf("Goodbye!\n");
  return 0;
}
