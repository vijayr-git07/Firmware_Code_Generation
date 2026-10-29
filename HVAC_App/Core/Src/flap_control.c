/**
 * @file    flap_control.c
 * @brief   Flap control logic implementation.
 */

#include "flap_control.h"
#include "feedback_processor.h"
#include "motor_driver.h"
#include "led_status.h"

/* --------------- precise stop detection table -------------------------- */
#define NUM_POSITIONS  6U

typedef struct
{
    uint16_t adc_min;
    uint16_t adc_max;
} stop_window_t;

/* Tight hardware-calibrated ADC windows per position */
static const stop_window_t stop_table[NUM_POSITIONS] =
{
    { 4055U, 4065U },   /* position 0 */
    { 3837U, 3857U },   /* position 1 */
    { 3563U, 3593U },   /* position 2 */
    { 3309U, 3329U },   /* position 3 */
    { 2022U, 2042U },   /* position 4 */
    {  315U,  335U }    /* position 5 */
};

/* --------------- module-static state ----------------------------------- */
static uint8_t target_position;
static uint8_t current_position;
static uint8_t movement_active_flag;

/* ======================================================================= */
void flap_control_init(void)
{
    target_position      = 0U;
    current_position     = 0U;
    movement_active_flag = 0U;
}

/* ======================================================================= */
void flap_control_set_target(uint8_t position)
{
    if (position <= 5U)
    {
        target_position = position;
    }
}

/* ======================================================================= */
void flap_control_process(void)
{
    uint8_t  fb_valid;
    uint8_t  fb_position;
    uint16_t raw_adc;

    /* --- read current feedback ----------------------------------------- */
    fb_valid   = feedback_processor_is_valid();
    fb_position = feedback_processor_get_position();
    raw_adc    = feedback_processor_get_raw_adc();

    /* --- fault: invalid feedback --------------------------------------- */
    if (fb_valid == 0U)
    {
        if (movement_active_flag != 0U)
        {
            motor_driver_stop();
            movement_active_flag = 0U;
        }
        /* Preserve LED state during invalid feedback */
        return;
    }

    /* Update tracked current position from valid feedback */
    current_position = fb_position;

    /* --- check precise stop window during active movement -------------- */
    if (movement_active_flag != 0U)
    {
        if ((raw_adc >= stop_table[target_position].adc_min) &&
            (raw_adc <= stop_table[target_position].adc_max))
        {
            /* Target reached – stop motor */
            motor_driver_stop();
            movement_active_flag = 0U;

            /* Update LED to target position */
            led_status_update(target_position);
            return;
        }
    }

    /* --- determine movement requirement -------------------------------- */
    if (target_position == current_position)
    {
        /* Already at target coarse position */
        if (movement_active_flag != 0U)
        {
            /* Still moving but coarse match – check precise window */
            if ((raw_adc >= stop_table[target_position].adc_min) &&
                (raw_adc <= stop_table[target_position].adc_max))
            {
                motor_driver_stop();
                movement_active_flag = 0U;
            }
            /* else: keep moving until precise window is hit */
        }
        /* Update LED to current position */
        led_status_update(current_position);
        return;
    }

    /* --- movement needed ----------------------------------------------- */
    /*
     * Position 0 has the highest ADC value (>4000) and position 5 the
     * lowest (<1000).  Moving to a LOWER position number means moving
     * toward HIGHER ADC values.
     *
     * Forward (open)  = toward lower position numbers = higher ADC.
     *   Actually: "Forward" in the truth table means IN1=RESET, IN2=SET.
     *   Per the env-setup, Forward = open direction.
     *
     * The ADC decreases as the flap opens (pos 0 = closed = high ADC,
     * pos 5 = open = low ADC).  So:
     *   target > current  =>  opening  =>  ADC must decrease  =>  FORWARD
     *   target < current  =>  closing  =>  ADC must increase  =>  REVERSE
     */
    if (target_position > current_position)
    {
        /* Need to open further (lower ADC) */
        motor_driver_set_direction(MOTOR_DIR_FORWARD);
    }
    else
    {
        /* Need to close (higher ADC) */
        motor_driver_set_direction(MOTOR_DIR_REVERSE);
    }

    motor_driver_enable();
    movement_active_flag = 1U;
}
