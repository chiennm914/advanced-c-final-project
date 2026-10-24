#include "cli_cmd.h"
#include "alert.h"
#include "app_fsm.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static void cmd_start(void *p_ctx);
static void cmd_stop(void *p_ctx);
static void cmd_status(void *p_ctx);
static void cmd_config(void *p_ctx);
static void cmd_alert_ack(void *p_ctx);
static void cmd_reset(void *p_ctx);
static void cmd_help(void *p_ctx);
static void cmd_quit(void *p_ctx);

static const cli_command_t s_cmd_table[] = {
    {"start", cmd_start},         {"stop", cmd_stop},
    {"status", cmd_status},       {"config", cmd_config},
    {"alert_ack", cmd_alert_ack}, {"reset", cmd_reset},
    {"help", cmd_help},           {"quit", cmd_quit}};

status_t cli_dispatch(const char *p_input, void *p_ctx) {
  if (p_input == NULL) {
    return STATUS_ERR_NULL_PTR;
  }

  char command[64];
  size_t len = strcspn(p_input, "\r\n");

  if (len >= sizeof(command)) {
    return STATUS_ERR_INVALID_PARAM;
  }

  memcpy(command, p_input, len);
  command[len] = '\0';

  for (size_t i = 0U; i < ARRAY_SIZE(s_cmd_table); i++) {
    if (strcmp(command, s_cmd_table[i].p_name) == 0) {
      if (s_cmd_table[i].handler == NULL) {
        return STATUS_ERR_INVALID_PARAM;
      }

      s_cmd_table[i].handler(p_ctx);
      return STATUS_OK;
    }
  }

  (void)printf("Unknown command\n");
  return STATUS_ERR_INVALID_PARAM;
}

static void cmd_start(void *p_ctx) {
  if (p_ctx == NULL) {
    return;
  }

  app_context_t *p_app = (app_context_t *)p_ctx;

  p_app->current_state = fsm_run(p_app->current_state, EVT_CMD_START);
}

static void cmd_stop(void *p_ctx) {
  if (p_ctx == NULL) {
    return;
  }

  app_context_t *p_app = (app_context_t *)p_ctx;

  p_app->current_state = fsm_run(p_app->current_state, EVT_CMD_STOP);
}

static void cmd_reset(void *p_ctx) {
  if (p_ctx == NULL) {
    return;
  }

  app_context_t *p_app = (app_context_t *)p_ctx;

  p_app->current_state = fsm_run(p_app->current_state, EVT_CMD_RESET);
}
// cppcheck-suppress constParameterCallback
static void cmd_status(void *p_ctx) {
  if (p_ctx == NULL) {
    return;
  }

  const app_context_t *p_app = (const app_context_t *)p_ctx;

  switch (p_app->current_state) {
  case SYS_INIT:
    (void)printf("State: INIT\n");
    break;

  case SYS_IDLE:
    (void)printf("State: IDLE\n");
    break;

  case SYS_MONITORING:
    (void)printf("State: MONITORING\n");
    break;

  case SYS_ALERT:
    (void)printf("State: ALERT\n");
    break;

  case SYS_ERROR:
    (void)printf("State: ERROR\n");
    break;

  default:
    (void)printf("State: UNKNOWN\n");
    break;
  }
}

static void cmd_alert_ack(void *p_ctx) {
  if (p_ctx == NULL) {
    return;
  }

  app_context_t *p_app = (app_context_t *)p_ctx;

  if (p_app->current_state == SYS_ALERT) {
    alert_acknowledge();

    p_app->current_state = fsm_run(p_app->current_state, EVT_ALERT_ACK);
  }
}

static void cmd_config(void *p_ctx) {
  (void)p_ctx;

  alert_config_t config;

  if (alert_get_config(&config) == false) {
    (void)printf("Alert configuration unavailable.\n");
    return;
  }

  (void)printf("Alert thresholds:\n");
  (void)printf("  Temp min: %d\n", (int)config.temp_min);
  (void)printf("  Temp max: %d\n", (int)config.temp_max);
  (void)printf("  Humi min: %d\n", (int)config.humi_min);
  (void)printf("  Humi max: %d\n", (int)config.humi_max);
}

static void cmd_help(void *p_ctx) {
  (void)p_ctx;

  (void)printf("Available commands:\n");

  for (size_t i = 0U; i < ARRAY_SIZE(s_cmd_table); i++) {
    (void)printf("  %s\n", s_cmd_table[i].p_name);
  }
}

static void cmd_quit(void *p_ctx) {
  if (p_ctx == NULL) {
    return;
  }

  app_context_t *p_app = (app_context_t *)p_ctx;

  if (p_app->current_state == SYS_IDLE) {
    p_app->quit_requested = true;
    (void)printf("Shutting down...\n");
  }
}