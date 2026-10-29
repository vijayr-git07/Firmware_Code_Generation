/**
 * @file    flap_control.c
 * @brief   HVAC Flap Control - Flap Control Logic implementation
 *
 * Movement direction logic (env-setup §13):
 *   Higher position number => lower ADC value => FORWARD (open)
 *   Lower  position number => higher ADC value => REVERSE (close)
 *
 * SWE-REQ: 016, 017, 018, 019, 020, 021, 022, 023, 044, 050, 054
 */

#include "flap_control.h"
#include "feedback_processor.h"
#include "motor_driver.h"
#include "led_status.h"
#include "command_parser.h"

/* ---- Module-static data ------------------------------------------ */
static uint8_t target_position;
static uint8_t movement_active_flag;

/* ---- Public API -------------------------------------------------- */

void flap_control_init(void)
{
    target_position      = CMD_INVALID;  /* no target yet */
    movement_active_flag = 0U;
}

void flap_control_set_target(uint8_t target)
{
    if (target <= CMD_POSITION_MAX)
    {
        target_position = target;
    }
}

void flap_control_process(void)
{
    uint8_t current_pos;
    uint8_t fb_valid;
    uint8_t cmd;

    /* ---- 1. Acquire latest command ------------------------------- */
    cmd = command_parser_get_latest();
    if (cmd != CMD_INVALID)
    {
        flap_control_set_target(cmd);
    }

    /* ---- 2. Check feedback validity ------------------------------ */
    fb_valid    = feedback_processor_is_valid();
    current_pos = feedback_processor_get_position();

    if (fb_valid == 0U)
    {
        /* Invalid feedback — stop motor immediately (SWE-REQ-021). */
        if (movement_active_flag != 0U)
        {
            motor_driver_stop();
            movement_active_flag = 0U;
        }
        /* Preserve LED state during invalid feedback (SWE-REQ-039). */
        return;
    }

    /* ---- 3. No target set yet — idle ----------------------------- */
    if (target_position == CMD_INVALID)
    {
        /* Update LEDs to reflect current position even when idle. */
        led_status_update(current_pos);
        return;
    }

    /* ---- 4. Check precise stop window ---------------------------- */
    if (feedback_processor_target_reached(target_position) != 0U)
    {
        /* Target reached — stop motor (SWE-REQ-019). */
        if (movement_active_flag != 0U)
        {
            motor_driver_stop();
            movement_active_flag = 0U;
        }
        led_status_update(target_position);
        return;
    }

    /* ---- 5. Determine direction and move ------------------------- */
    if (target_position == current_pos)
    {
        /*
         * Coarse position matches but precise window not yet reached.
         * Keep moving in the same direction until precise stop.
         * If not already moving, start based on ADC comparison.
         */
        if (movement_active_flag == 0U)
        {
            /* Use raw ADC to decide micro-direction. */
            uint16_t raw = feedback_processor_get_raw_adc();
            /* Precise table midpoints decrease with increasing position. */
            /* For the target, if raw > midpoint we need to go forward
               (toward lower ADC = higher position number direction),
               if raw < midpoint we need to go reverse. */
            /* Simple heuristic: compare against precise window bounds. */
            /* If we are above the precise window, move forward (lower ADC). */
            /* If below, move reverse (higher ADC). */
            /* Note: "forward" = open = higher position = lower ADC. */
            static const uint16_t precise_mid[6] = {
                4060U, 3847U, 3578U, 3319U, 2032U, 325U
            };
            if (raw > precise_mid[target_position])
            {
                motor_driver_set_direction(MOTOR_DIR_FORWARD);
            }
            else
            {
                motor_driver_set_direction(MOTOR_DIR_REVERSE);
            }
            motor_driver_enable();
            movement_active_flag = 1U;
        }
        /* else: already moving — keep going until precise stop. */
    }
    else if (target_position > current_pos)
    {
        /* Need to move to higher position number => lower ADC => FORWARD. */
        motor_driver_set_direction(MOTOR_DIR_FORWARD);
        motor_driver_enable();
        movement_active_flag = 1U;
    }
    else /* target_position < current_pos */
    {
        /* Need to move to lower position number => higher ADC => REVERSE. */
        motor_driver_set_direction(MOTOR_DIR_REVERSE);
        motor_driver_enable();
        movement_active_flag = 1U;
    }

    /* Update LEDs to reflect current coarse position during movement. */
    led_status_update(current_pos);
}
