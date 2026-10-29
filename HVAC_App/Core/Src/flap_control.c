/**
 * @file    flap_control.c
 * @brief   Flap Control Logic module implementation for HVAC flap control.
 *
 * Coordinates motor movement and LED updates based on the difference
 * between the commanded target position and the current ADC feedback.
 *
 * Uses two tables:
 *   - Coarse table (in feedback_processor) for current position detection
 *   - Precise stop table (here) for target-reached detection
 *
 * Movement direction (higher ADC = lower position number):
 *   Target position < current position => ADC must increase => REVERSE
 *   Target position > current position => ADC must decrease => FORWARD
 */

#include "flap_control.h"
#include "feedback_processor.h"
#include "motor_driver.h"
#include "led_status.h"
#include "main.h"

/* ---------------------------------------------------------------------------
 * Module constants -- Precise Stop Detection Table
 * -----------------------------------------------------------------------*/
#define NUM_POSITIONS  6U

typedef struct {
    uint16_t adc_min;
    uint16_t adc_max;
} stop_window_t;

static const stop_window_t precise_stop_table[NUM_POSITIONS] = {
    { 4055U, 4065U },  /* Position 0 */
    { 3837U, 3857U },  /* Position 1 */
    { 3563U, 3593U },  /* Position 2 */
    { 3309U, 3329U },  /* Position 3 */
    { 2022U, 2042U },  /* Position 4 */
    {  315U,  335U },  /* Position 5 */
};

/* ---------------------------------------------------------------------------
 * Module-static variables
 * -----------------------------------------------------------------------*/
static uint8_t target_position       = 0U;
static uint8_t movement_active_flag  = 0U;

/* ---------------------------------------------------------------------------
 * Private helpers
 * -----------------------------------------------------------------------*/

/**
 * @brief  Check whether the current raw ADC value is within the precise
 *         stop window for the given target position.
 * @param  adc_val  Current raw ADC reading.
 * @param  target   Target position (0-5).
 * @return 1 if within the stop window, 0 otherwise.
 */
static uint8_t is_target_reached(uint16_t adc_val, uint8_t target)
{
    uint8_t reached = 0U;

    if (target < NUM_POSITIONS)
    {
        if ((adc_val >= precise_stop_table[target].adc_min) &&
            (adc_val <= precise_stop_table[target].adc_max))
        {
            reached = 1U;
        }
    }

    return reached;
}

/**
 * @brief  Determine the required motor direction to move from the current
 *         raw ADC value toward the precise stop window of the target position.
 * @param  raw_adc  Current raw ADC reading.
 * @param  target   Target position (0-5).
 * @return MOTOR_DIR_FORWARD or MOTOR_DIR_REVERSE.
 */
static uint8_t determine_direction(uint16_t raw_adc, uint8_t target)
{
    /* The midpoint of the target precise stop window */
    uint16_t target_mid;

    if (target < NUM_POSITIONS)
    {
        target_mid = (precise_stop_table[target].adc_min +
                      precise_stop_table[target].adc_max) / 2U;
    }
    else
    {
        target_mid = raw_adc;  /* safety: no movement */
    }

    if (raw_adc < target_mid)
    {
        /* ADC must increase => REVERSE (toward higher ADC / lower position) */
        return MOTOR_DIR_REVERSE;
    }
    else
    {
        /* ADC must decrease => FORWARD (toward lower ADC / higher position) */
        return MOTOR_DIR_FORWARD;
    }
}

/* ---------------------------------------------------------------------------
 * Public functions
 * -----------------------------------------------------------------------*/

void flap_control_init(void)
{
    target_position      = 0U;
    movement_active_flag = 0U;
}

void flap_control_set_target(uint8_t position)
{
    if (position < NUM_POSITIONS)
    {
        target_position = position;
    }
}

void flap_control_process(void)
{
    uint8_t  current_pos;
    uint16_t raw_adc;
    uint8_t  fb_valid;
    uint8_t  dir;

    /* Get current feedback state */
    fb_valid    = feedback_processor_is_valid();
    current_pos = feedback_processor_get_position();
    raw_adc     = feedback_processor_get_raw_adc();

    /* --- Fault handling: invalid feedback => stop motor immediately --- */
    if (fb_valid == 0U)
    {
        if (movement_active_flag != 0U)
        {
            motor_driver_stop();
            movement_active_flag = 0U;
        }
        /* Preserve LED state during invalid feedback (SWE-REQ-039) */
        return;
    }

    /* --- Check precise stop window for the target position --- */
    if (is_target_reached(raw_adc, target_position) != 0U)
    {
        /* Target reached: stop motor and update LED */
        if (movement_active_flag != 0U)
        {
            motor_driver_stop();
            movement_active_flag = 0U;
        }
        led_status_update(target_position);
        return;
    }

    /* --- Movement is needed: not yet in precise stop window --- */

    /* Determine direction based on raw ADC vs target stop window midpoint */
    dir = determine_direction(raw_adc, target_position);
    motor_driver_set_direction(dir);

    if (movement_active_flag == 0U)
    {
        motor_driver_enable();
        movement_active_flag = 1U;
    }

    /* Update LED to reflect current coarse position during movement */
    led_status_update(current_pos);
}
