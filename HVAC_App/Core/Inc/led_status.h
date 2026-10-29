/**
 * @file    led_status.h
 * @brief   HVAC Flap Control - LED Status Handler interface
 *
 * Controls:
 *   Power LED  : PC0 — always ON during runtime
 *   Position LEDs: PC1 (pos 1) .. PC5 (pos 5)
 *   Position 0 has no dedicated LED; all position LEDs are OFF.
 *
 * SWE-REQ: 035, 036, 037, 038, 039
 */

#ifndef LED_STATUS_H
#define LED_STATUS_H

#include <stdint.h>

/* ---------------- Public API -------------------------------------- */

/**
 * @brief  Initialise LED status handler.
 *
 * Turns the power LED ON and all position LEDs OFF.
 */
void led_status_init(void);

/**
 * @brief  Update position LEDs to reflect the given flap position.
 * @param  position  0 .. 5 (valid), or 0xFF (invalid/fault).
 *
 * - Position 0 or invalid: all position LEDs OFF.
 * - Position 1..5: only the corresponding LED is ON.
 * - Power LED remains ON regardless.
 */
void led_status_update(uint8_t position);

#endif /* LED_STATUS_H */
