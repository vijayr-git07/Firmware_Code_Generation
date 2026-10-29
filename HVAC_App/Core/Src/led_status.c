/**
 * @file    led_status.c
 * @brief   LED Status Handler module implementation for HVAC flap control.
 *
 * LED mapping:
 *   PC0 -- Power LED (always ON during runtime)
 *   PC1 -- Position 1 LED
 *   PC2 -- Position 2 LED
 *   PC3 -- Position 3 LED
 *   PC4 -- Position 4 LED
 *   PC5 -- Position 5 LED
 *
 * Position 0 (fully closed) has no dedicated LED.
 */

#include "led_status.h"
#include "main.h"

/* ---------------------------------------------------------------------------
 * Module constants
 * -----------------------------------------------------------------------*/
#define POWER_LED_PORT      GPIOC
#define POWER_LED_PIN       GPIO_PIN_0

#define POS_LED_PORT        GPIOC

#define NUM_POSITION_LEDS   5U

/* Position LED pins: index 0 = position 1 LED (PC1), ... index 4 = position 5 LED (PC5) */
static const uint16_t position_led_pins[NUM_POSITION_LEDS] = {
    GPIO_PIN_1,  /* Position 1 */
    GPIO_PIN_2,  /* Position 2 */
    GPIO_PIN_3,  /* Position 3 */
    GPIO_PIN_4,  /* Position 4 */
    GPIO_PIN_5,  /* Position 5 */
};

/* ---------------------------------------------------------------------------
 * Module-static variables
 * -----------------------------------------------------------------------*/
static uint8_t current_led_state = 0U;

/* ---------------------------------------------------------------------------
 * Private helpers
 * -----------------------------------------------------------------------*/

/**
 * @brief  Turn off all position LEDs (PC1-PC5).
 */
static void turn_off_all_position_leds(void)
{
    uint8_t i;

    for (i = 0U; i < NUM_POSITION_LEDS; i++)
    {
        HAL_GPIO_WritePin(POS_LED_PORT, position_led_pins[i], GPIO_PIN_RESET);
    }
}

/* ---------------------------------------------------------------------------
 * Public functions
 * -----------------------------------------------------------------------*/

void led_status_init(void)
{
    /* Turn on power LED (SWE-REQ-035) */
    HAL_GPIO_WritePin(POWER_LED_PORT, POWER_LED_PIN, GPIO_PIN_SET);

    /* Turn off all position LEDs */
    turn_off_all_position_leds();

    current_led_state = 0U;
}

void led_status_update(uint8_t position)
{
    /* Validate input range */
    if (position > 5U)
    {
        return;
    }

    /* Only update if position has changed (SWE-REQ-038) */
    if (position == current_led_state)
    {
        return;
    }

    /* Turn off all position LEDs first (one-active rule, SWE-REQ-037) */
    turn_off_all_position_leds();

    if ((position >= 1U) && (position <= 5U))
    {
        /* Activate the LED for the given position.
         * Position 1 -> index 0 (PC1), Position 5 -> index 4 (PC5) */
        HAL_GPIO_WritePin(POS_LED_PORT, position_led_pins[position - 1U], GPIO_PIN_SET);
    }
    /* Position 0: no dedicated LED -- all position LEDs remain off */

    current_led_state = position;
}
