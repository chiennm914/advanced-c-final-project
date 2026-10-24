
#ifndef APP_FSM_H
#define APP_FSM_H

#include "common_types.h"

/* System events */
typedef enum {
  EVT_INIT_OK = 0,
  EVT_INIT_FAIL,
  EVT_CMD_START,
  EVT_CMD_STOP,
  EVT_CMD_RESET,
  EVT_CMD_QUIT,
  EVT_ALERT_TRIGGERED,
  EVT_ALERT_ACK,
  EVT_SENSOR_FAIL,
  EVT_NUM_EVENTS
} system_event_t;

/* Function pointer type for state handlers */
typedef system_state_t (*state_handler_t)(system_event_t event);

/**
 * @brief Process an event in the current FSM state.
 *
 * @param current_state Current system state.
 * @param event Event to process.
 * @return Next system state.
 */
system_state_t fsm_run(system_state_t current_state, system_event_t event);

#endif /* APP_FSM_H */
