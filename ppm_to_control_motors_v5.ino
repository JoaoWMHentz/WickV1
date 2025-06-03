#include "turnigy.h"
#include "motor.h"

int mode = 0;
unsigned long lastBlink = 0;
bool ledState = false;
bool wasConnected = false; // ⬅️ novo estado anterior da conexão

void setup() {
  initMotors();
  initTurnigy();
  pinMode(1, OUTPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  digitalWrite(1, HIGH);
  playStartupTone();

  if (digitalRead(5)) mode = 1;
  if (digitalRead(6)) mode = 2;
}

void loop() {
  bool connected = verifyConection();

  if (connected && !wasConnected) {
    playConnectedTone(); // ⬅️ toca só ao conectar pela primeira vez
  }

  if (!connected) {
    if (millis() - lastBlink >= 100) {
      ledState = !ledState;
      digitalWrite(1, ledState);
      lastBlink = millis();
    }
    brakeA();
    brakeB();
  } else {
    digitalWrite(1, HIGH);
    switch (mode) {
      case 0: modeCar(); break;
      case 1: modeTank(); break;
      case 2: modeMirror(); break;
      default: modeCar(); break;
    }
  }

  wasConnected = connected; // ⬅️ atualiza o estado anterior
}

void modeCar() {
  int ch1 = turnigy(TURNIGY_CH1, -255, 255);
  int ch2 = turnigy(TURNIGY_CH2, -255, 255);
  motorControl(ch1 + ch2, ch1 - ch2);
}

void modeTank() {
  int ch1 = turnigy(TURNIGY_CH1, -255, 255);
  int ch2 = turnigy(TURNIGY_CH2, -255, 255);
  motorControl(ch1, ch2);
}

void modeMirror() {
  int ch1 = turnigy(TURNIGY_CH1, -255, 255);
  motorControl(ch1, ch1);
}
