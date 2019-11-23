#ifndef ldr_h
#define ldr_h

#define MAX_LDR 1

class LDR {
  int pins[MAX_LDR];

  public:
    LDR(int pin0);
    int getBrightness();
};
#endif
