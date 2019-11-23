#include <Arduino.h>

#include "ldr.h"

LDR::LDR(int pin0) {
  pins[0] = pin0;
}

int LDR::getBrightness() {
  int reading = 0;

  for (int i = 0; i < MAX_LDR; i++) {
    reading += analogRead(pins[i]);
  }

  return floor(reading/MAX_LDR);
}
