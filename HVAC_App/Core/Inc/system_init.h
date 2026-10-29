/**
 * @file    system_init.h
 * @brief   System initialization manager interface for HVAC flap control
 */

#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize all application-layer modules.
 *
 * Shall be called once after CubeMX peripheral initialization completes.
 * Ensures motor safe state, reads initial feedback, and sets initial LED state.
 */
void system_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_INIT_H */
