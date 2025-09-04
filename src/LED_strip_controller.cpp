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

void test_led_strip ();

void LED_strip_init(void)
{
  /* LED Array comes up in unknown state/LED pattern */
  pixels.clear();
  pixels.show(ledStrip);

  test_led_strip();
}

void LED_strip_controller_set_pixels_in_range(uint16_t start, uint16_t end, LED_colors_t color, uint8_t brightness) {
  Color_t c = LED_ColorTable[color];
  pixels.setRange(start, end, c.r, c.g, c.b, brightness);
  pixels.show(ledStrip);
  //Serial.printf("Setting LED to R=%d G=%d B=%d\n", c.r, c.g, c.b);
}

void LED_strip_controller_clear_pixels_in_range(uint16_t start, uint16_t end) {
  pixels.setRange(start, end, 0,0,0);
  pixels.show(ledStrip);
  //Serial.printf("Clearing LED to R=%d G=%d B=%d\n", c.r, c.g, c.b);
}

void LED_strip_controller_blink_pixels_in_range(uint16_t start, uint16_t end, LED_colors_t color, uint8_t brightness) {
  //TBD....
}

void test_led_strip (){
    // Set some individual LEDs
  LED_strip_controller_set_pixels_in_range(0, 3, RED, 25);
  LED_strip_controller_set_pixels_in_range(4, 8, GREEN, 25);
  LED_strip_controller_set_pixels_in_range(9, 11, BLUE, 25);
}

