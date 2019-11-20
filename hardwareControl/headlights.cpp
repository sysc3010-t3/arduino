#include "headlights.h"
#include "ldr.h"

// Initializes headlights with the output pins and an LDR object
Headlights::Headlights(int leftPin, int rightPin) {
  ledL = leftPin;
  ledR = rightPin;
  state = LED_AUTO; // set to auto by default
}

// Gets the brightness reading from the LDR and set the LEDs accordingly
void Headlights::autoSet(LDR& ldr) {
  int brightness = ldr.getBrightness();
  if (brightness < 800) {
    setLEDs(LED_ON);
  } else {
    setLEDs(LED_OFF);
  }
}

// Sets the state of the Headlights object and sets the LEDs accordingly
void Headlights::setState(int stateArg) {
  if (stateArg == state){
    return;
  }
  
  // If state is being changed from AUTO
  if (state == LED_AUTO) {
    TIMSK1 &= (0 << OCIE1A);  // disable timer compare interrupt
  }
  
  switch(stateArg) {
    case LED_OFF:
    case LED_ON:
      setLEDs(stateArg);
      break;
    case LED_AUTO:
      TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
      break;
     default:
      Serial.print("Invalid LED state: ");
      Serial.println(state);
      return;
  }
  
  state = stateArg;
}

// Writes the provided value to the output pins
void Headlights::setLEDs(int value) {
  digitalWrite(ledL, value);
  digitalWrite(ledR, value);
}
