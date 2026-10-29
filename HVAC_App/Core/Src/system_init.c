/**
 * @file    system_init.c
 * @brief   System Initialization Manager implementation for HVAC flap control.
 *
 * Initializes all application-layer modules in the documented order:
 *   1. Command Parser
 *   2. Feedback Processor
 *   3. Motor Driver (ensures safe stopped state)
 *   4. LED Status Handler (power LED ON)
 *   5. Flap Control Logic
 *   6. Read initial feedback position
 *   7. Update initial LED state
 */

#include "system_init.h"
#include "command_parser.h"
#include "feedback_processor.h"
#include "motor_driver.h"
#include "led_status.h"
#include "flap_control.h"

void system_init(void)
{
    /* 1. Initialize command parser */
    command_parser_init();

    /* 2. Initialize feedback processor */
    feedback_processor_init();

    /* 3. Initialize motor driver -- ensures safe stopped state (SWE-REQ-047) */
    motor_driver_init();

    /* 4. Initialize LED status handler -- power LED ON, position LEDs OFF */
    led_status_init();

    /* 5. Initialize flap control logic */
    flap_control_init();

    /* 6. Read initial feedback position (SWE-REQ-048) */
    feedback_processor_update();

    /* 7. Update initial LED state based on current position (SWE-REQ-049) */
    if (feedback_processor_is_valid() != 0U)
    {
        led_status_update(feedback_processor_get_position());
    }
}
