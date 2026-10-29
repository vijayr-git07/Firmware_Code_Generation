/**
 * @file    flap_control.h
 * @brief   Flap control logic interface for HVAC flap control
 */

#ifndef FLAP_CONTROL_H
#define FLAP_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief  Initialize the flap control module.
 */
void flap_control_init(void);

/**
 * @brief  Execute one cycle of flap control logic. Call periodically.
 */
void flap_control_process(void);

/**
 * @brief  Set a new target flap position.
 * @param  position  Target position (0-5).
 */
void flap_control_set_target(uint8_t position);

#ifdef __cplusplus
}
#endif

#endif /* FLAP_CONTROL_H */
