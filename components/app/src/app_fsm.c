#include "app_fsm.h"
#include <stddef.h>
#include "logger.h"


static system_state_t state_init(system_event_t event);
static system_state_t state_idle(system_event_t event);
static system_state_t state_monitoring(system_event_t event);
static system_state_t state_alert(system_event_t event);
static system_state_t state_error(system_event_t event);

static const state_handler_t s_state_table[SYS_NUM_STATES] = {
    [SYS_INIT]       = state_init,
    [SYS_IDLE]       = state_idle,
    [SYS_MONITORING] = state_monitoring,
    [SYS_ALERT]      = state_alert,
    [SYS_ERROR]      = state_error
};

static const char *const s_state_names[SYS_NUM_STATES] = {
    "SYS_INIT",
    "SYS_IDLE",
    "SYS_MONITORING",
    "SYS_ALERT",
    "SYS_ERROR"
};

static const char *const s_event_names[EVT_NUM_EVENTS] = {
    "EVT_INIT_OK",
    "EVT_INIT_FAIL",
    "EVT_CMD_START",
    "EVT_CMD_STOP",
    "EVT_CMD_RESET",
    "EVT_CMD_QUIT",
    "EVT_ALERT_TRIGGERED",
    "EVT_ALERT_ACK",
    "EVT_SENSOR_FAIL"
};

static system_state_t state_init(system_event_t event)
{
    system_state_t next_state = SYS_INIT;

    if (event == EVT_INIT_OK)
    {
        next_state = SYS_IDLE;
    }
    else if (event == EVT_INIT_FAIL)
    {
        next_state = SYS_ERROR;
    }
    else
    {
        return SYS_INIT;
    }

    return next_state;
}

static system_state_t state_idle(system_event_t event)
{
    system_state_t next_state = SYS_IDLE;

    if (event == EVT_CMD_START)
    {
        next_state = SYS_MONITORING;
    }
    else
    {
        return SYS_IDLE;
    }

    return next_state;
}

static system_state_t state_monitoring(system_event_t event)
{
    system_state_t next_state = SYS_MONITORING;

    if (event == EVT_ALERT_TRIGGERED)
    {
        next_state = SYS_ALERT;
    }
    else if (event == EVT_SENSOR_FAIL)
    {
        next_state = SYS_ERROR;
    }
    else if (event == EVT_CMD_STOP)
    {
        next_state = SYS_IDLE;
    }
    else
    {
        return SYS_MONITORING;
    }

    return next_state;
}

static system_state_t state_alert(system_event_t event)
{
    system_state_t next_state = SYS_ALERT;

    if (event == EVT_ALERT_ACK)
    {
        next_state = SYS_MONITORING;
    }
    else if (event == EVT_SENSOR_FAIL)
    {
        next_state = SYS_ERROR;
    }
    else
    {
        return SYS_ALERT;
    }

    return next_state;
}

static system_state_t state_error(system_event_t event)
{
    system_state_t next_state = SYS_ERROR;

    if (event == EVT_CMD_RESET)
    {
        next_state = SYS_INIT;
    }
    else
    {
        return SYS_ERROR;
    }

    return next_state;
}

system_state_t fsm_run(system_state_t current_state,
                       system_event_t event)
{
    if ((unsigned int)current_state >= (unsigned int)SYS_NUM_STATES)
    {
        return SYS_ERROR;
    }

    if ((unsigned int)event >= (unsigned int)EVT_NUM_EVENTS)
    {
        return SYS_ERROR;
    }

    if (s_state_table[current_state] == NULL)
    {
        return SYS_ERROR;
    }

    system_state_t next_state = s_state_table[current_state](event);

    if (next_state != current_state)
    {
        LOG_INFO("FSM: %s -> %s (%s)",
                s_state_names[current_state],
                s_state_names[next_state],
                s_event_names[event]);
    }

    return next_state;
}