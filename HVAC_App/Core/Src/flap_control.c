/**
 * @file    flap_control.c
 * @brief   Flap control logic for HVAC flap control
 *
 * Compares the target position against the current ADC-derived position,
 * commands the motor in the correct direction, and stops when the precise
 * stop window is reached.  Updates LEDs after every control cycle.
 *
 * Position numbering and ADC relationship (from env-setup):
 *   Position 0 (closed) → highest ADC (~4060)
 *   Position 5 (open)   → lowest  ADC (~325)
 *
 * Therefore:
 *   target > current  → ADC must decrease → motor FORWARD (open)
 *   target < current  → ADC must increase → motor REVERSE (close)
 *
 * Traces to: SWE-REQ-016..023, SWE-REQ-050, SWE-REQ-053
 */

#include "flap_control.h"
#include "feedback_processor.h"
#include "motor_driver.h"
#include "led_status.h"

/* ---------------------------------------------------------------------------
 * Module-static data
 * --------------------------------------------------------------------------- */
static uint8_t target_position;
static uint8_t movement_active_flag;

/* ---------------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------------- */

void flap_control_init(void)
{
    target_position      = 0U;
    movement_active_flag = 0U;
}

void flap_control_set_target(uint8_t position)
{
    if (position < FLAP_POSITION_COUNT)
    {
        target_position = position;
    }
}

void flap_control_process(void)
{
    uint8_t current_position;

    /* ---- Fault guard: invalid feedback → stop immediately ---- */
    if (feedback_processor_is_valid() == 0U)
    {
        if (movement_active_flag != 0U)
        {
            motor_driver_stop();
            movement_active_flag = 0U;
        }
        /* Update LEDs even on fault (deactivates position LEDs) */
        led_status_update(0U, 0U);
        return;
    }

    current_position = feedback_processor_get_position();

    /* ---- Check precise stop window during active movement ---- */
    if (movement_active_flag != 0U)
    {
        if (feedback_processor_in_stop_window(target_position) != 0U)
        {
            /* Target reached — stop motor */
            motor_driver_stop();
            movement_active_flag = 0U;

            /* Refresh current position after stop */
            current_position = feedback_processor_get_position();
            led_status_update(current_position, 1U);
            return;
        }
    }

    /* ---- Determine movement requirement ---- */
    if (target_position == current_position)
    {
        /* Already at target — ensure motor is stopped */
        if (movement_active_flag != 0U)
        {
            motor_driver_stop();
            movement_active_flag = 0U;
        }
    }
    else if (target_position > current_position)
    {
        /*
         * Need to move to a higher position number (more open).
         * Higher position = lower ADC value → motor FORWARD.
         */
        motor_driver_set_direction(MOTOR_DIR_FORWARD);
        motor_driver_enable();
        movement_active_flag = 1U;
    }
    else /* target_position < current_position */
    {
        /*
         * Need to move to a lower position number (more closed).
         * Lower position = higher ADC value → motor REVERSE.
         */
        motor_driver_set_direction(MOTOR_DIR_REVERSE);
        motor_driver_enable();
        movement_active_flag = 1U;
    }

    /* ---- Update LED status ---- */
    led_status_update(current_position, 1U);
}
