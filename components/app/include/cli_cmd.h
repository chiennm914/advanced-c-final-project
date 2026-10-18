#ifndef CLI_CMD_H
#define CLI_CMD_H

#include "common_types.h"
#include "app_fsm.h"
#include <stdbool.h>
#include "alert.h"

/**
 * @file cli_cmd.h
 * @brief CLI command dispatcher interface.
 */

/**
 * @brief CLI command entry.
 */
typedef struct
{
    const char *p_name;
    void (*handler)(void *p_ctx);
} cli_command_t;

typedef struct
{
    system_state_t current_state;
    bool quit_requested;
} app_context_t;

/**
 * @brief Dispatch a CLI command.
 *
 * @param[in] p_input Input command string.
 * @param[in] p_ctx Application context.
 *
 * @return STATUS_OK if command is recognized,
 *         STATUS_ERR_NULL_PTR if an argument is NULL,
 *         STATUS_ERR_INVALID_PARAM if command is unknown.
 */
status_t cli_dispatch(const char *p_input, void *p_ctx);



#endif /* CLI_CMD_H */