/**
 * @file    system_init.c
 * @brief   HVAC Flap Control - System Initialization Manager implementation
 *
 * SWE-REQ: 046, 047, 048, 049, 054
 */

#include "system_init.h"
#include "command_parser.h"
#include "feedback_processor.h"
#include "motor_driver.h"
#include "led_status.h"
#include "flap_control.h"

void system_init(void)
{
    /* 1. Initialise command parser. */
    command_parser_init();

    /* 2. Initialise feedback processor. */
    feedback_processor_init();

    /* 3. Initialise motor driver (ensures safe OFF state, SWE-REQ-047). */
    motor_driver_init();

    /* 4. Initialise LED handler (power LED ON, position LEDs OFF). */
    led_status_init();

    /* 5. Initialise flap control logic. */
    flap_control_init();

    /* 6. Read initial flap feedback position (SWE-REQ-048). */
    feedback_processor_update();

    /* 7. Update initial LED state (SWE-REQ-049). */
    {
        uint8_t pos = feedback_processor_get_position();
        led_status_update(pos);
    }
}
