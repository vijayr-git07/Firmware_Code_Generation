/**
 * @file    hvac_app.c
 * @brief   HVAC Application integration implementation.
 *
 * Provides the fixed application entry points:
 *   - hvac_app_init()  : delegates to system_init()
 *   - hvac_app_task()  : executes one superloop cycle
 *
 * Runtime sequence per cycle:
 *   1. Poll and validate UART commands
 *   2. Acquire and map ADC feedback
 *   3. Apply new target position if a valid command was received
 *   4. Execute flap control logic (motor + LED coordination)
 */

#include "hvac_app.h"
#include "system_init.h"
#include "command_parser.h"
#include "feedback_processor.h"
#include "flap_control.h"

void hvac_app_init(void)
{
    /* Initialize all application-layer modules via system init manager */
    system_init();
}

void hvac_app_task(void)
{
    uint8_t target_pos = 0U;

    /* 1. Poll UART for new commands (SWE-REQ-015) */
    command_parser_process();

    /* 2. Acquire ADC feedback and update position (SWE-REQ-024) */
    feedback_processor_update();

    /* 3. If a valid command is available, set the new target (SWE-REQ-016) */
    if (command_parser_get_latest(&target_pos) != 0U)
    {
        flap_control_set_target(target_pos);
    }

    /* 4. Execute flap control logic (motor direction, stop, LED update) */
    flap_control_process();
}
