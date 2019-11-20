#ifndef headlights_h
#define headlights_h
#include <Arduino.h>

#include "ldr.h"

#define LED_OFF 0
#define LED_ON 1
#define LED_AUTO 2

class Headlights {
  int ledL;
  int ledR;
  int state;

  public:
    // Initializes headlights with the output pins and an LDR object
    Headlights(int leftPin, int rightPin);

    // Gets the brightness reading from the LDR and set the LEDs accordingly
    void autoSet(LDR& ldr);

    // Sets the state of the Headlights object and sets the LEDs accordingly
    void setState(int stateArg);

  private:
    // Writes the provided value to the output pins
    void setLEDs(int value);
};
#endif
