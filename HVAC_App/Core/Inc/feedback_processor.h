/**
 * @file    feedback_processor.h
 * @brief   HVAC Flap Control - Feedback Processor interface
 *
 * Acquires ADC samples from the potentiometer, validates them,
 * and maps them to logical flap positions using coarse and
 * precise stop-detection tables.
 *
 * SWE-REQ: 024, 025, 026, 027, 028, 029
 */

#ifndef FEEDBACK_PROCESSOR_H
#define FEEDBACK_PROCESSOR_H

#include <stdint.h>

/* ---------------- Public Constants -------------------------------- */

/** Total number of logical flap positions. */
#define FLAP_POSITION_COUNT   6U

/** Sentinel for invalid / unknown position. */
#define FLAP_POSITION_INVALID ((uint8_t)0xFFU)

/* ---------------- Public API -------------------------------------- */

/**
 * @brief  Initialise the feedback processor module.
 */
void feedback_processor_init(void);

/**
 * @brief  Perform one ADC acquisition cycle and update internal state.
 *
 * Starts a single ADC conversion, waits for completion, reads the
 * value, validates it against the coarse table, and maps it to a
 * logical position.
 *
 * Must be called once per superloop iteration.
 */
void feedback_processor_update(void);

/**
 * @brief  Return the current coarse logical flap position.
 * @return 0 .. 5 on success, FLAP_POSITION_INVALID on fault.
 */
uint8_t feedback_processor_get_position(void);

/**
 * @brief  Check whether the latest feedback is valid.
 * @return 1 if valid, 0 if invalid / fault.
 */
uint8_t feedback_processor_is_valid(void);

/**
 * @brief  Return the latest raw ADC value.
 * @return 12-bit ADC reading (0 .. 4095).
 */
uint16_t feedback_processor_get_raw_adc(void);

/**
 * @brief  Check whether the raw ADC value is within the precise
 *         stop window for the given target position.
 * @param  target_position  0 .. 5
 * @return 1 if within window, 0 otherwise.
 */
uint8_t feedback_processor_target_reached(uint8_t target_position);

#endif /* FEEDBACK_PROCESSOR_H */
