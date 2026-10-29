/**
 * @file    led_status.h
 * @brief   LED status handler interface for HVAC flap control
 */

#ifndef LED_STATUS_H
#define LED_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief  Initialize the LED status handler.
 *         Turns on the power LED and clears all position LEDs.
 */
void led_status_init(void);

/**
 * @brief  Update position LEDs based on current flap position.
 * @param  position       Current flap position (0-5).
 * @param  feedback_valid 1 if feedback is valid, 0 if invalid.
 *
 * Only one position LED is active at a time.
 * Position 0 has no dedicated LED; all position LEDs are OFF.
 * If feedback is invalid, all position LEDs are turned OFF.
 */
void led_status_update(uint8_t position, uint8_t feedback_valid);

#ifdef __cplusplus
}
#endif

#endif /* LED_STATUS_H */
