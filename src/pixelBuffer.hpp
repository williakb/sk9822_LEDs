// PixelBuffer.hpp
#pragma once
#include <Arduino.h>

// Simple per-LED state
struct Pixel {
  uint8_t r, g, b;   // 0..255
  uint8_t gb;        // global brightness 0..31
};

class PixelBuffer {
public:
  explicit PixelBuffer(uint16_t n) : _n(n) {
    _buf = new Pixel[_n];
    clear();
  }
  ~PixelBuffer() { delete[] _buf; }

  uint16_t size() const { return _n; }

  // Set full pixel (clamps gb to 0..31)
  inline void set(uint16_t i, uint8_t r, uint8_t g, uint8_t b, uint8_t gb = 31) {
    if (i >= _n) return;
    _buf[i].r = r; _buf[i].g = g; _buf[i].b = b;
    _buf[i].gb = (gb > 31) ? 31 : gb;
  }

  inline void setRGB(uint16_t i, uint8_t r, uint8_t g, uint8_t b) {
    if (i >= _n) return;
    _buf[i].r = r; _buf[i].g = g; _buf[i].b = b;
  }

  inline void setGB(uint16_t i, uint8_t gb) {
    if (i >= _n) return;
    _buf[i].gb = (gb > 31) ? 31 : gb;
  }

  // Fill entire strip
  inline void fill(uint8_t r, uint8_t g, uint8_t b, uint8_t gb = 31) {
    if (gb > 31) gb = 31;
    for (uint16_t i = 0; i < _n; ++i) _buf[i] = Pixel{r,g,b,gb};
  }

  // Turn off (keep gb = 0 so it’s explicit)
  inline void clear() {
    for (uint16_t i = 0; i < _n; ++i) _buf[i] = Pixel{0,0,0,0};
  }
  
  // Set LEDs [start..end] (inclusive) to one color/brightness
  inline void setRange(uint16_t start, uint16_t end,
                       uint8_t r, uint8_t g, uint8_t b, uint8_t gb = 31) {
    if (_n == 0) return;
    if (start > end) { uint16_t t = start; start = end; end = t; }
    if (start >= _n) return;
    if (end >= _n) end = _n - 1;
    if (gb > 31) gb = 31;

    for (uint16_t i = start; i <= end; ++i) {
      _buf[i].r = r; _buf[i].g = g; _buf[i].b = b; _buf[i].gb = gb;
    }
  }

  // Only change RGB across a range, keep existing per-pixel brightness
  inline void setRangeRGB(uint16_t start, uint16_t end,
                          uint8_t r, uint8_t g, uint8_t b) {
    if (_n == 0) return;
    if (start > end) { uint16_t t = start; start = end; end = t; }
    if (start >= _n) return;
    if (end >= _n) end = _n - 1;

    for (uint16_t i = start; i <= end; ++i) {
      _buf[i].r = r; _buf[i].g = g; _buf[i].b = b;
    }
  }

  // Only change brightness across a range (0..31), keep existing RGB
  inline void setRangeGB(uint16_t start, uint16_t end, uint8_t gb) {
    if (_n == 0) return;
    if (start > end) { uint16_t t = start; start = end; end = t; }
    if (start >= _n) return;
    if (end >= _n) end = _n - 1;
    if (gb > 31) gb = 31;

    for (uint16_t i = start; i <= end; ++i) {
      _buf[i].gb = gb;
    }
  }  

  // Render using your existing APA102 instance
  template <typename APA>
  void show(APA& ledStrip) {
    ledStrip.startFrame();
    for (uint16_t i = 0; i < _n; ++i) {
      // Your APA102 library expects sendColor(R,G,B,GB)
      ledStrip.sendColor(_buf[i].r, _buf[i].g, _buf[i].b, _buf[i].gb);
    }
    ledStrip.endFrame(_n);
  }

  // Direct access if you want advanced effects
  Pixel* data() { return _buf; }
  const Pixel* data() const { return _buf; }

private:
  Pixel* _buf = nullptr;
  uint16_t _n = 0;
};