/**
 * @file    hvac_app.c
 * @brief   HVAC application integration – init and runtime coordination.
 */

#include "hvac_app.h"
#include "command_parser.h"
#include "feedback_processor.h"
#include "flap_control.h"
#include "motor_driver.h"
#include "led_status.h"

/* ======================================================================= */
void hvac_app_init(void)
{
    /* 1. Initialize command parser */
    command_parser_init();

    /* 2. Initialize feedback processor */
    feedback_processor_init();

    /* 3. Initialize motor driver (ensures safe-off state) */
    motor_driver_init();

    /* 4. Initialize LED handler (power LED ON, position LEDs OFF) */
    led_status_init();

    /* 5. Initialize flap control logic */
    flap_control_init();

    /* 6. Read initial flap feedback position */
    feedback_processor_update();

    /* 7. Update initial LED state from feedback */
    if (feedback_processor_is_valid() != 0U)
    {
        uint8_t initial_pos = feedback_processor_get_position();
        led_status_update(initial_pos);
        flap_control_set_target(initial_pos);
    }
}

/* ======================================================================= */
void hvac_app_task(void)
{
    uint8_t new_target;

    /* 1. Poll and validate UART command */
    command_parser_process();

    /* 2. If a new valid command is available, update the target */
    if (command_parser_get_latest(&new_target) != 0U)
    {
        flap_control_set_target(new_target);
    }

    /* 3. Acquire ADC feedback and update position state */
    feedback_processor_update();

    /* 4. Execute flap control logic (motor + LED coordination) */
    flap_control_process();
}
