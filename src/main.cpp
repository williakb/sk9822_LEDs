/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <APA102.h>
#include "pixelBuffer.hpp"
#include "LED_strip_controller.hpp"

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200); // Initialize serial communication at 9600 baud

  LED_strip_controller_init();
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(100);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
   // wait for a second
  delay(100);

  // Sending data
  //Serial.println("Hello from Kirks Arduino Mega!");
  //delay(100); // Wait for 1 second

  // Receiving data (optional)
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(receivedData);
  }
}