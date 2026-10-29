/**
 * @file    hvac_app.c
 * @brief   HVAC application integration glue
 *
 * Coordinates all application-layer modules in the superloop:
 *   1. Process UART commands
 *   2. Read ADC feedback
 *   3. Execute flap control logic (includes motor + LED updates)
 *
 * Traces to: SWE-REQ-007..011
 */

#include "hvac_app.h"
#include "system_init.h"
#include "command_parser.h"
#include "feedback_processor.h"
#include "flap_control.h"

void hvac_app_init(void)
{
    system_init();
}

void hvac_app_task(void)
{
    uint8_t new_target;

    /* 1. Poll and process UART command input */
    command_parser_process();

    /* 2. If a new valid command is available, update the target */
    if (command_parser_get_latest(&new_target) != 0U)
    {
        flap_control_set_target(new_target);
    }

    /* 3. Read ADC feedback and update position state */
    feedback_processor_update();

    /* 4. Execute flap control logic (motor + LED updates inside) */
    flap_control_process();
}
