
#include <stdio.h>
#include <stdbool.h>

#include "app_fsm.h"
#include "cli_cmd.h"

#define CLI_INPUT_SIZE 128U

int main(void)
{
    app_context_t app = {
        .current_state = SYS_INIT,
        .quit_requested = false
    };

    char input[CLI_INPUT_SIZE];

    app.current_state = fsm_run(app.current_state, EVT_INIT_OK);

    printf("=== IoT Environmental Monitoring ===\n");
    printf("Type 'help' to see available commands.\n");

    while (!app.quit_requested)
    {
        printf("> ");
        if (fflush(stdout) == EOF)
        {
            return 1;
        }

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        cli_dispatch(input, &app);
    }

    printf("Goodbye!\n");

    return 0;
}
