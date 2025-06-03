#ifndef _TURNIGY_H
#define _TURNIGY_H

#include <Arduino.h>

#define TURNIGY_CH1 2
#define TURNIGY_CH2 3

void initTurnigy() {
  pinMode(TURNIGY_CH1, INPUT_PULLUP);
  pinMode(TURNIGY_CH2, INPUT_PULLUP);
}

int valueCh1Min = 1000, valueCh1Max = 2000;

unsigned long myPulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L) {
  uint8_t stateMask = state ? HIGH : LOW;
  unsigned long start = micros();

  while (digitalRead(pin) == stateMask) {
    if (micros() - start >= timeout) {
      return 0;
    }
  }

  start = micros();
  while (digitalRead(pin) != stateMask) {
    if (micros() - start >= timeout) {
      return 0;
    }
  }

  unsigned long pulseStart = micros();
  while (digitalRead(pin) == stateMask) {
    if (micros() - pulseStart >= timeout) {
      return 0;
    }
  }

  return micros() - pulseStart;
}

int verifyConection() {
  int valueIn1 = myPulseIn(TURNIGY_CH1, HIGH, 100000);
  int valueIn2 = myPulseIn(TURNIGY_CH2, HIGH, 100000);
  return !(valueIn1 == 0 || valueIn2 == 0);
}
int turnigy(int pinChannel, int valueStart, int valueEnd) {
  int valueIn = myPulseIn(pinChannel, HIGH, 100000);
  if (valueIn == 0) {
    return 0;
  }

  valueIn = map(valueIn, valueCh1Min, valueCh1Max, valueStart, valueEnd);
  if (valueIn <= 10 && valueIn >= -10) {
    valueIn = 0;
  }

  return valueIn;
}



#endif