/**
 * @file    hvac_app.c
 * @brief   HVAC Flap Control - Application integration implementation
 *
 * Coordinates the superloop runtime sequence defined in SWE.3 §2.0:
 *   1. Poll UART command input
 *   2. Read ADC feedback
 *   3. Execute flap control logic (includes motor + LED updates)
 *
 * SWE-REQ: 007, 008, 009, 011, 046
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
    /* Step 1: Poll UART for new commands. */
    command_parser_process();

    /* Step 2: Acquire and validate ADC feedback. */
    feedback_processor_update();

    /* Step 3: Execute flap control logic (motor + LED coordination). */
    flap_control_process();
}
