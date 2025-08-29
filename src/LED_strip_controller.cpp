#include <Arduino.h>
#include <APA102.h>
#include "pixelBuffer.hpp"
#include "LED_strip_controller.hpp"

/* By default, the APA102 library uses pinMode and digitalWrite
 * to write to the LEDs, which works on all Arduino-compatible
 * boards but might be slow.  If you have a board supported by
 * the FastGPIO library and want faster LED updates, then install
 * the FastGPIO library and uncomment the next two lines: */
//  #include <FastGPIO.h>
//  #define APA102_USE_FAST_GPIO

  // Define which pins to use.
const uint8_t dataPin = 51;//MOSI
const uint8_t clockPin = 52;//SCLK

// Create an object for writing to the LED strip.
APA102<dataPin, clockPin> ledStrip;

constexpr uint16_t LED_COUNT = 60;
PixelBuffer pixels(LED_COUNT);

void test_led_strip (){
    // Set some individual LEDs
    pixels.set(0, 255, 0, 0, 31);  // LED 0 red @ max
    pixels.set(1, 0, 255, 0, 16);  // LED 1 green @ mid
    pixels.set(2, 0, 0, 255, 4);   // LED 2 blue @ low

    // Push the whole strip at once
    //pixels.show(ledStrip);

    //delay(1000);

    // Fill then update
    //pixels.fill(255, 255, 255, 8);
    //pixels.show(ledStrip);

    // Set LEDs 4..8 to blue @ max brightness
    pixels.setRange(4, 8, /*r*/0, /*g*/0, /*b*/255, /*gb*/31);
    pixels.show(ledStrip);

    delay(3000);
    // Later: dim that section without changing color
    pixels.setRangeGB(4, 8, 8);
    pixels.show(ledStrip);

    delay(3000);

    pixels.clear();
    pixels.show(ledStrip);
}

void LED_strip_controller_init(void)
{
    test_led_strip ();
}