/**
 * @file    system_init.c
 * @brief   System initialization manager for HVAC flap control
 *
 * Initializes all application-layer modules in the correct order,
 * reads the initial flap position, and sets the initial LED state.
 *
 * Traces to: SWE-REQ-046..049
 */

#include "system_init.h"
#include "command_parser.h"
#include "feedback_processor.h"
#include "motor_driver.h"
#include "flap_control.h"
#include "led_status.h"

void system_init(void)
{
    /* 1. Initialize command parser */
    command_parser_init();

    /* 2. Initialize feedback processor */
    feedback_processor_init();

    /* 3. Initialize motor driver (ensures safe OFF state) */
    motor_driver_init();

    /* 4. Initialize flap control logic */
    flap_control_init();

    /* 5. Initialize LED handler (power LED ON, position LEDs OFF) */
    led_status_init();

    /* 6. Read initial feedback position */
    feedback_processor_update();

    /* 7. Update initial LED state based on feedback */
    if (feedback_processor_is_valid() != 0U)
    {
        uint8_t initial_pos = feedback_processor_get_position();
        flap_control_set_target(initial_pos);  /* Target = current at startup */
        led_status_update(initial_pos, 1U);
    }
    else
    {
        led_status_update(0U, 0U);
    }
}
