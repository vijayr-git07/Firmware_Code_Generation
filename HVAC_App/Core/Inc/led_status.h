/**
 * @file    led_status.h
 * @brief   LED Status Handler module interface for HVAC flap control.
 *
 * Controls the power LED (PC0) and position indicator LEDs (PC1-PC5).
 * Only one position LED is active at a time.
 */

#ifndef LED_STATUS_H
#define LED_STATUS_H

#include <stdint.h>

/**
 * @brief  Initialize the LED status module.
 *         Turns on the power LED and turns off all position LEDs.
 */
void led_status_init(void);

/**
 * @brief  Update position LED indication.
 *         Activates the LED corresponding to the given position.
 *         Position 0 has no dedicated LED -- all position LEDs are off.
 *         Only one position LED is active at a time.
 * @param  position  Logical flap position (0-5).
 */
void led_status_update(uint8_t position);

#endif /* LED_STATUS_H */
