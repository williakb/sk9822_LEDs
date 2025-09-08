#include <Arduino.h>
#include <APA102.h>
#include "pixelBuffer.hpp"
#include "LED_strip_controller.hpp"
/*
 * Arduino Mega (ATmega2560) — Timer1 PWM on pin 11 (OC1A)
 * Pattern:
 *   - Start at 1%, wait 10s
 *   - Ramp up to 50%
 *   - Ramp down to 1%
 *   - Wait 10s
 *   - Repeat
 */
//PURLIN -> approx 2ms to 10ms...

#define PWM_PIN   11       // OC1A on Mega
#define PWM_FREQ  60       // x Hz PWM ~ 16.667ms

const float DUTY_MIN = 12.0f;       // %~2ms
const float DUTY_MAX = 48.0f;       // %~8ms

const unsigned long WAIT_MS      = 5000; // 10 seconds at ends
const unsigned long RAMP_STEP_MS = 1000;   // time between duty updates during ramps
const float         RAMP_STEP_PCT = 5.0f; // change per step during ramps

// --- State machine ---
enum class PwmState { WAIT_AT_MIN, RAMP_UP, RAMP_DOWN };
PwmState state = PwmState::WAIT_AT_MIN;

unsigned long lastTick = 0;
float duty = DUTY_MIN;

void updatePWMDuty(uint8_t pin, float dutyCyclePercent);

// ---------- PWM helpers (Timer1: pins 11 = OC1A, 12 = OC1B) ----------
void setPWM(uint8_t pin, uint32_t frequency, float dutyCyclePercent) {
  if (dutyCyclePercent < 0) dutyCyclePercent = 0;
  if (dutyCyclePercent > 100) dutyCyclePercent = 100;

  pinMode(pin, OUTPUT);

  // Stop Timer1
  TCCR1A = 0;
  TCCR1B = 0;

  // Compute TOP and choose prescaler
  uint32_t prescaler = 1;
  uint32_t top = (F_CPU / (prescaler * frequency)) - 1;
  if (top > 65535) { prescaler = 8;   top = (F_CPU / (prescaler * frequency)) - 1; }
  if (top > 65535) { prescaler = 64;  top = (F_CPU / (prescaler * frequency)) - 1; }
  if (top > 65535) { prescaler = 256; top = (F_CPU / (prescaler * frequency)) - 1; }
  if (top > 65535) { prescaler = 1024;top = (F_CPU / (prescaler * frequency)) - 1; }
  if (top > 65535)  top = 65535; // clamp

  // Fast PWM with ICR1 as TOP
  TCCR1A = (1 << WGM11);
  TCCR1B = (1 << WGM13) | (1 << WGM12);

  // Prescaler bits
  switch (prescaler) {
    case 1:    TCCR1B |= (1 << CS10); break;
    case 8:    TCCR1B |= (1 << CS11); break;
    case 64:   TCCR1B |= (1 << CS11) | (1 << CS10); break;
    case 256:  TCCR1B |= (1 << CS12); break;
    case 1024: TCCR1B |= (1 << CS12) | (1 << CS10); break;
  }

  ICR1 = top;

  // Enable output channel
  if (pin == 11) {
    TCCR1A |= (1 << COM1A1); // non-inverting on OC1A
  } else if (pin == 12) {
    TCCR1A |= (1 << COM1B1); // non-inverting on OC1B
  }

  // Initial duty
  updatePWMDuty(pin, dutyCyclePercent);
}

void updatePWMDuty(uint8_t pin, float dutyCyclePercent) {
  if (dutyCyclePercent < 0) dutyCyclePercent = 0;
  if (dutyCyclePercent > 100) dutyCyclePercent = 100;

  uint16_t top = ICR1;
  uint16_t val = (uint16_t)((dutyCyclePercent / 100.0f) * (top + 1));

  if (pin == 11) {
    OCR1A = val;
  } else if (pin == 12) {
    OCR1B = val;
  }
}

// ------------------- Sketch -------------------
void setup() {
  setPWM(PWM_PIN, PWM_FREQ, DUTY_MIN);
  lastTick = millis();  // start the initial 10s wait
  state = PwmState::WAIT_AT_MIN;
}

void loop() {
  unsigned long now = millis();

  switch (state) {
    case PwmState::WAIT_AT_MIN:
      if (now - lastTick >= WAIT_MS) {
        state = PwmState::RAMP_UP;
        lastTick = now;
      }
      break;

    case PwmState::RAMP_UP:
      if (now - lastTick >= RAMP_STEP_MS) {
        lastTick = now;
        duty += RAMP_STEP_PCT;
        if (duty >= DUTY_MAX) {
          duty = DUTY_MAX;
          state = PwmState::RAMP_DOWN; // immediately start down
        }
        updatePWMDuty(PWM_PIN, duty);
      }
      break;

    case PwmState::RAMP_DOWN:
      if (now - lastTick >= RAMP_STEP_MS) {
        lastTick = now;
        duty -= RAMP_STEP_PCT;
        if (duty <= DUTY_MIN) {
          duty = DUTY_MIN;
          updatePWMDuty(PWM_PIN, duty);
          state = PwmState::WAIT_AT_MIN; // wait 10s at the bottom
          lastTick = now;                // (reset wait timer)
          break;
        }
        updatePWMDuty(PWM_PIN, duty);
      }
      break;
  }
}





// /*
//  * Blink
//  * Turns on an LED on for one second,
//  * then off for one second, repeatedly.
//  */

// #include <Arduino.h>
// #include <APA102.h>
// #include "pixelBuffer.hpp"
// #include "LED_strip_controller.hpp"

// #define PWM_PIN 11
// #define DC_INCREASE_VALUE .01
// #define DC_UPDATE_INTERVAL 3000

// void init_pwm(); 
// void setPWM(uint8_t pin, uint32_t frequency, float dutyCyclePercent);
// void loopUpdateDC();

// unsigned long lastUpdate = 0;
// float duty = 0.01;   // start at 0.01%
// bool increasing = true;

// void setup()
// {
//   // initialize LED digital pin as an output.
//   pinMode(LED_BUILTIN, OUTPUT);

//   Serial.begin(115200); // Initialize serial communication at 9600 baud

//   //LED_strip_init();

//   //init_pwm();
//   setPWM(PWM_PIN, 60, .01);
// }

// void loop()
// {
//   // // turn the LED on (HIGH is the voltage level)
//   // digitalWrite(LED_BUILTIN, HIGH);
//   // // wait for a second
//   // delay(100);
//   // // turn the LED off by making the voltage LOW
//   // digitalWrite(LED_BUILTIN, LOW);
//   //  // wait for a second
//   // delay(100);

//   // // Sending data
//   // //Serial.println("Hello from Kirks Arduino Mega!");
//   // //delay(100); // Wait for 1 second

//   // Receiving data (optional)
//   if (Serial.available() > 0) {
//     String receivedData = Serial.readStringUntil('\n');
//     Serial.print("Received: ");
//     Serial.println(receivedData);

//     if (receivedData[0] == 'a')
//       LED_strip_controller_clear_pixels_in_range(3,5);
//   }

//   loopUpdateDC();
// }

// // Example: 1 kHz PWM at 25% duty on pin 11 (OC1A)
// void init_pwm() {
//   pinMode(PWM_PIN, OUTPUT);

//   // Stop Timer1
//   TCCR1A = 0;
//   TCCR1B = 0;

//   // Set Fast PWM mode with ICR1 as TOP
//   TCCR1A = (1 << COM1A1);                 // non-inverting mode on OC1A
//   TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10); // Fast PWM, no prescale
//   TCCR1A |= (1 << WGM11);

//   ICR1 = 15999;    // TOP value → sets frequency
//                    // f_PWM = 16MHz / (N * (1 + TOP))
//                    // here: 16MHz / (1 * 16000) = 1 kHz

//   OCR1A = 4000;    // duty = OCR1A / ICR1 = 4000/16000 = 25%
// }

// /*
//  * Simple PWM helper for Arduino Mega
//  * Uses Timer1 (pins 11 = OC1A, 12 = OC1B)
//  */

// void setPWM(uint8_t pin, uint32_t frequency, float dutyCyclePercent) {
//   // constrain dutyCycle between 0–100%
//   if (dutyCyclePercent < 0) dutyCyclePercent = 0;
//   if (dutyCyclePercent > 100) dutyCyclePercent = 100;

//   // Set pin as output
//   pinMode(pin, OUTPUT);

//   // Stop Timer1
//   TCCR1A = 0;
//   TCCR1B = 0;

//   // Calculate TOP value for desired frequency
//   // f_PWM = 16MHz / (prescaler * (1 + TOP))
//   uint32_t prescaler = 1;
//   uint32_t top = (F_CPU / (prescaler * frequency)) - 1;

//   // If TOP too large for 16-bit, increase prescaler
//   if (top > 65535) { prescaler = 8;  top = (F_CPU / (prescaler * frequency)) - 1; }
//   if (top > 65535) { prescaler = 64; top = (F_CPU / (prescaler * frequency)) - 1; }
//   if (top > 65535) { prescaler = 256;top = (F_CPU / (prescaler * frequency)) - 1; }
//   if (top > 65535) { prescaler = 1024;top = (F_CPU / (prescaler * frequency)) - 1; }

//   // Set Fast PWM mode with ICR1 as TOP
//   TCCR1A = (1 << WGM11);
//   TCCR1B = (1 << WGM13) | (1 << WGM12);

//   // Configure prescaler bits
//   switch (prescaler) {
//     case 1:    TCCR1B |= (1 << CS10); break;
//     case 8:    TCCR1B |= (1 << CS11); break;
//     case 64:   TCCR1B |= (1 << CS11) | (1 << CS10); break;
//     case 256:  TCCR1B |= (1 << CS12); break;
//     case 1024: TCCR1B |= (1 << CS12) | (1 << CS10); break;
//   }

//   // Set TOP value
//   ICR1 = top;

//   // Set duty cycle
//   uint16_t duty = (uint16_t)((dutyCyclePercent / 100.0) * (top + 1));

//   if (pin == 11) {
//     TCCR1A |= (1 << COM1A1);  // non-inverting on OC1A
//     OCR1A = duty;
//   } else if (pin == 12) {
//     TCCR1A |= (1 << COM1B1);  // non-inverting on OC1B
//     OCR1B = duty;
//   }
// }

// void updatePWMDuty(uint8_t pin, float dutyCyclePercent) {
//   if (dutyCyclePercent < 0) dutyCyclePercent = 0;
//   if (dutyCyclePercent > 100) dutyCyclePercent = 100;

//   uint16_t duty = (uint16_t)((dutyCyclePercent / 100.0) * (ICR1 + 1));

//   if (pin == 11) {
//     OCR1A = duty;
//   } else if (pin == 12) {
//     OCR1B = duty;
//   }
// }

// void loopUpdateDC() {
//   unsigned long now = millis();

//   if (now - lastUpdate >= DC_UPDATE_INTERVAL) {  // 10 seconds passed
//     lastUpdate = now;

//     // Change duty cycle
//     if (increasing) {
//       duty += DC_INCREASE_VALUE;  // increase by 10%
//       if (duty >= 100) {
//         duty = 100;
//         increasing = false;
//       }
//     } else {
//       duty -= DC_INCREASE_VALUE;  // decrease by 10%
//       if (duty <= 0) {
//         duty = 0;
//         increasing = true;
//       }
//     }

//     // Update the duty cycle
//     updatePWMDuty(11, duty);
//   }
// }