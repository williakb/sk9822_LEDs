#include <Arduino.h>
#include <APA102.h>
#include "pixelBuffer.hpp"
#include "LED_strip_controller.hpp"

typedef enum
{
    DRILL_OFF = 0,
    DRILL_READY,
    DRILL_ACTIVE,
    DRILL_ERROR
} DrillLEDstate_t;

void set_drill_LED_state(uint8_t drillNum, DrillLEDstate_t state){

}

void set_error_LED_state(void){

}