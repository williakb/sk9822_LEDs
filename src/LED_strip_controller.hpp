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
// Define a simple RGB struct
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color_t;

// Enum for symbolic names
typedef enum {
    CLEAR,
    RED,
    GREEN,
    BLUE,
    ORANGE,
    COLORS_COUNT     // <-- handy to know array size
} LED_colors_t;

// Lookup table mapping enum → RGB values
const Color_t LED_ColorTable[COLORS_COUNT] = {
    {0, 0, 0},       // CLEAR
    {255, 0, 0},     // RED
    {0, 255, 0},     // GREEN
    {0, 0, 255},     // BLUE
    {255, 64, 0}     // ORANGE-ISH
};

/** Public Functions **/
#ifdef __cplusplus /* for compatibility with C++ environments  */
extern "C" {
#endif

void LED_strip_init(void);
void LED_strip_controller_set_pixels_in_range(uint16_t start, uint16_t end, LED_colors_t color, uint8_t brightness);
void LED_strip_controller_clear_pixels_in_range(uint16_t start, uint16_t end);

#ifdef __cplusplus /* for compatibility with C++ environments  */
}
#endif
