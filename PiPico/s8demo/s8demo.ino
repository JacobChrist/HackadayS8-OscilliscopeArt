#include <SPI.h>  //use the SPI library
#include "dac.h"

void setup() {
  // These instruction inform the Pico Pi SPI library which
  // pins to use for SPI communications.
  //SPI.setRX(0);    // Ignored: we are not receiving any data
  //SPI.setCS(1);    // Ignored: when software controlled CS
  SPI1.setSCK(14);  // Serial Clock
  SPI1.setTX(15);   // MOSI Serial Data

  dac_init();
  SPI1.begin(false);  // false means CS in software controlled
  pinMode(dac_cs, OUTPUT);

  while(!BOOTSEL)
  {
    jolly_wrencher(0, 0);
  }
}

const float TAU = 6.283185307179586;

void loop() {
  static uint16_t x = 0, y = 0;
  const int CYCLES = 500;

  // Circle
  for (int32_t i = 0; i < CYCLES; i++) {
    circle(128, 120.0 * ((float)i/CYCLES), 0, 0);
  }
  for (int32_t i = 0; i < CYCLES; i++) {
    circle(128, 120.0, TAU * ((float)i/CYCLES), 0);
  }
  for (int32_t i = CYCLES; i > 0; i--) {
    circle(128, 120.0 * ((float)i/CYCLES), 0, 0);
  }

  for (int32_t i = 0; i < 500; i++) {
    jolly_wrencher(0, 0);
  }

  // growing amplitude
  for (int32_t i = 0; i < CYCLES; i++) {
    for (uint16_t x = 0; x < 256; x++) {
      uint8_t y = (uint8_t)((120 * ((float)i / CYCLES) * sin((float)x / 255.0 * TAU * ((float)5))) + 128.0);
      dac_xy(x, y);
    }
  }

  for (int32_t i = 0; i < 500; i++) {
    jolly_wrencher(x, y);
    x++;
    y++;
  }

  // Lissajous
  for (int32_t i = 0; i < CYCLES; i++) {
    for (uint16_t t = 0; t < 512; t++) {
      uint8_t x = (uint8_t)((120.0 * cos((float)t / 255.0 * TAU * ((float)i / CYCLES * 1))) + 128.0);
      uint8_t y = (uint8_t)((120.0 * sin((float)t / 255.0 * TAU * ((float)i / CYCLES * 3) + ((float)i / CYCLES * 10))) + 128.0);
      dac_xy(y, x);
    }
  }

  int dir = 1;
  y = 0;
  for (int32_t i = 0; i < 500; i++) {

    supercon8(0, y);
    y += dir;
    if (y > 175) dir = -1;
    if (y < 1) dir = 1;
  }

  for (int32_t i = 0; i < CYCLES; i++) {
    g_of_sin((float)i / CYCLES * 10);
  }

  for (int32_t i = 0; i < 500; i++) {
    logic_gates();
  }

  for (int32_t i = 0; i < CYCLES; i++) {
    f_of_sin((float)i / CYCLES * 10);
  }

  for (int32_t i = 0; i < 500; i++) {
    jolly_wrencher(-x/3, 0);
    supercon8(x, y);
    x++;
    y++;
  }
}

void circle(uint16_t points, float amplitude, float x_phase, float y_phase) {
  for (float t = 0; t < TAU; t += TAU / points) {
    uint8_t x = (uint8_t)((amplitude * cos(t + x_phase)) + 128.0);
    uint8_t y = (uint8_t)((amplitude * sin(t + y_phase)) + 128.0);
    dac_xy(y, x);
  }
}

void g_of_sin(float freq) {
  for (uint16_t x = 0; x < 256; x++) {
    uint8_t y = (uint8_t)((120.0 * sin((float)x / 255.0 * TAU * freq)) + 128.0);
    dac_xy(y, x);
  }
}

void f_of_sin(float freq) {
  for (uint16_t x = 0; x < 256; x++) {
    uint8_t y = (uint8_t)((120.0 * sin((float)x / 255.0 * TAU * freq)) + 128.0);
    dac_xy(x, y);
  }
}