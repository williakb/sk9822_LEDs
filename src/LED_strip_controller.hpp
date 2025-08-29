/*----------------------------------------------------------------------------
 * Description  : LED (sk9822 based) controller
 * Author       : Kirk Williams
 * Copyright (c) 2025 Ingenuity Electronics Design Pty Ltd
 *---------------------------------------------------------------------------*/
#pragma once

/** Includes **/
/* Project Headers */

/* System Headers */
#include <stdint.h>

/* External Library Headers */

/** Definitions **/

/** Public Functions **/
#ifdef __cplusplus /* for compatibility with C++ environments  */
extern "C" {
#endif

void LED_strip_controller_init(void);

#ifdef __cplusplus /* for compatibility with C++ environments  */
}
#endif
