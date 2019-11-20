#ifndef ldr_h
#define ldr_h

#define MAX_LDR 3

class LDR {
  int pins[MAX_LDR];

  public:
    LDR(int pin0, int pin1, int pin2);
    int getBrightness();
};
#endif
