#include <Arduino.h>
#include <APA102.h>
#include "pixelBuffer.hpp"
#include "LED_strip_controller.hpp"

// #define DRILL_SEGMENT_LENGTH 5
// #define ERROR_SEGMENT_LENGTH 2

// // Represents one contiguous range of LEDs
// // Each drill has two LED sections
// enum class Drill_LED_Section : uint8_t {
//     Drill,   // main drill LEDs
//     Error,   // error LEDs
//     COUNT
// };

// // All 12 drills (6 front, 6 back)
// enum class Drill_LED : uint8_t {
//     Drill0_LED_Front,
//     Drill1_LED_Front,
//     Drill2_LED_Front,
//     Drill3_LED_Front,
//     Drill4_LED_Front,
//     Drill5_LED_Front,

//     Drill0_LED_Back,
//     Drill1_LED_Back,
//     Drill2_LED_Back,
//     Drill3_LED_Back,
//     Drill4_LED_Back,
//     Drill5_LED_Back,

//     COUNT
// };

// struct GroupRange {
//     uint16_t start;
//     uint16_t end;   // inclusive
// };

// struct DrillLayout {
//     GroupRange sections[static_cast<uint8_t>(Drill_LED_Section::COUNT)];
// };

// // Build the layout sequentially
// constexpr DrillLayout LAYOUTS[static_cast<uint8_t>(Drill_LED::COUNT)] = {
//     /* Front side */
//     { { { 0,  2}, { 3,  4} } },  // Drill0_LED_Front
//     { { { 5,  7}, { 8,  9} } },  // Drill1_LED_Front
//     { { {10, 12}, {13, 14} } },  // Drill2_LED_Front
//     { { {15, 17}, {18, 19} } },  // Drill3_LED_Front
//     { { {20, 22}, {23, 24} } },  // Drill4_LED_Front
//     { { {25, 27}, {28, 29} } },  // Drill5_LED_Front

//     /* Back side */
//     { { {30, 32}, {33, 34} } },  // Drill6_LED_Back
//     { { {35, 37}, {38, 39} } },  // Drill7_LED_Back
//     { { {40, 42}, {43, 44} } },  // Drill8_LED_Back
//     { { {45, 47}, {48, 49} } },  // Drill9_LED_Back
//     { { {50, 52}, {53, 54} } },  // Drill10_LED_Back
//     { { {55, 57}, {58, 59} } },  // Drill11_LED_Back
// };
// typedef enum
// {
//     DRILL_OFF = 0,
//     DRILL_READY,
//     DRILL_ACTIVE,
//     DRILL_ERROR
// } DrillLEDstate_t;

// void set_drill_LED_state(uint8_t drillNum, DrillLEDstate_t state){
//     // Set Drill2 front "Drill" LEDs to green
//     fillDrill(Drill_LED::Drill2_LED_Front, Drill_LED_Section::Drill, ColorId::Green, mySetPixel);

//     // Set Drill7 back "Error" LEDs to orange
//     fillDrill(Drill_LED::Drill7_LED_Back, Drill_LED_Section::Error, ColorId::DrillError, mySetPixel);
// }

// void set_error_LED_state(void){

// }

// using SetPixelFn = void(*)(uint16_t index, Color c);

// inline void fillDrill(Drill_LED drill, Drill_LED_Section section, ColorId cid, SetPixelFn setPixel) {
//     const Color c = COLOR_TABLE[static_cast<uint8_t>(cid)];
//     const DrillLayout& dl = LAYOUTS[static_cast<uint8_t>(drill)];
//     const GroupRange& gr = dl.sections[static_cast<uint8_t>(section)];

//     for (uint16_t i = gr.start; i <= gr.end; ++i) {
//         setPixel(i, c);
//     }
// }