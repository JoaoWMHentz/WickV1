#ifndef _MOTOR_H
#define _MOTOR_H

#include <Arduino.h>

// Portas driver motor
#define MOTOR_AIN1 16
#define MOTOR_AIN2 0
#define MOTOR_BIN1 4
#define MOTOR_BIN2 9

// M1 IN1 PA3 M1 IN2 PA4 M2 IN2 PB0 M2 IN1 PB5

void motorControl(int speedLeft, int speedRight);

void initMotors();

void brakeA() {
  analogWrite(MOTOR_AIN1, 255);
  analogWrite(MOTOR_AIN2, 255);
}

void brakeB() {
  analogWrite(MOTOR_BIN1, 255);
  analogWrite(MOTOR_BIN2, 255);
}

void initMotors(){
  // Definições das portas digitais
  pinMode(MOTOR_AIN1, OUTPUT);
  pinMode(MOTOR_AIN2, OUTPUT);
  pinMode(MOTOR_BIN1, OUTPUT);
  pinMode(MOTOR_BIN2, OUTPUT);
}

void stopMotors(){
  motorControl(0, 0);
}

void motorControl(int speedLeft, int speedRight) {
  if(speedLeft < 50 && speedLeft > -50){
    brakeA();
  }

  if(speedRight < 50 && speedRight > -50){
    brakeB();
  }

  if (speedLeft < 0) {
    speedLeft = -speedLeft;
    analogWrite (MOTOR_AIN1, speedLeft);
    analogWrite (MOTOR_AIN2, 0);
  } else {
    analogWrite (MOTOR_AIN2, speedLeft);
    analogWrite (MOTOR_AIN1, 0);
  }

  if (speedRight < 0) {
    speedRight = -speedRight;
    analogWrite (MOTOR_BIN1, speedRight);
    analogWrite (MOTOR_BIN2, 0);
  } else {
    analogWrite (MOTOR_BIN2, speedRight);
    analogWrite (MOTOR_BIN1, 0);
  }
}

void playToneMotor(int freq, int duration) {
  int period = 1000000 / freq; // em microssegundos
  int halfPeriod = period / 2;
  int cycles = (duration * 1000L) / period;

  for (int i = 0; i < cycles; i++) {
    digitalWrite(MOTOR_AIN1, HIGH);
    digitalWrite(MOTOR_BIN1, HIGH);
    digitalWrite(MOTOR_AIN2, LOW);
    digitalWrite(MOTOR_BIN2, LOW);
    delayMicroseconds(halfPeriod);
    digitalWrite(MOTOR_AIN1, LOW);
    digitalWrite(MOTOR_BIN1, LOW);
    digitalWrite(MOTOR_AIN2, HIGH);
    digitalWrite(MOTOR_BIN2, HIGH);
    delayMicroseconds(halfPeriod);
  }

  // Desliga os pinos após o som
  digitalWrite(MOTOR_AIN1, LOW);
  digitalWrite(MOTOR_AIN2, LOW);
  digitalWrite(MOTOR_BIN1, LOW);
  digitalWrite(MOTOR_BIN2, LOW);
}

void playStartupTone() {
  // Garante que os pinos do outro motor estejam desligados
  digitalWrite(MOTOR_BIN1, LOW);
  digitalWrite(MOTOR_BIN2, LOW);
  digitalWrite(MOTOR_AIN1, LOW);
  digitalWrite(MOTOR_AIN2, LOW);

  playToneMotor(1500, 200); // Bip de 1kHz
  delay(100);
  playToneMotor(2000, 200); // Bip de 1.5kHz
  delay(100);
  playToneMotor(2500, 200); 
  delay(100);// Bip de 2kHz
}

void playConnectedTone() {
  stopMotors(); // Garante que nada esteja ativo

  playToneMotor(800, 150);  // "Tun" - nota grave
  delay(100);
  playToneMotor(1600, 150); // "Dun" - nota aguda

  stopMotors(); // Desliga após o som
}


#endif
