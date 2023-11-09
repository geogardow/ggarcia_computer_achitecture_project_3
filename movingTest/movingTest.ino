/*********
  0 AS MSB
  1 AS LSB
*********/

#include <Arduino.h>

#define UPPER_LEFT_0 0
#define UPPER_LEFT_1 1
#define UPPER_RIGHT_0 2
#define UPPER_RIGHT_1 3
#define LOWER_LEFT_0 4
#define LOWER_LEFT_1 5
#define LOWER_RIGHT_0 6
#define LOWER_RIGHT_1 7

void moveForward(){
  digitalWrite(UPPER_LEFT_0, HIGH);
  digitalWrite(UPPER_RIGHT_0, HIGH);
  digitalWrite(LOWER_LEFT_0, HIGH);
  digitalWrite(LOWER_RIGHT_0, HIGH);
  digitalWrite(UPPER_LEFT_1, LOW);
  digitalWrite(UPPER_RIGHT_1, LOW);
  digitalWrite(LOWER_LEFT_1, LOW);
  digitalWrite(LOWER_RIGHT_1, LOW);
}

void moveBackward(){
  digitalWrite(UPPER_LEFT_0, LOW);
  digitalWrite(UPPER_RIGHT_0, LOW);
  digitalWrite(LOWER_LEFT_0, LOW);
  digitalWrite(LOWER_RIGHT_0, LOW);
  digitalWrite(UPPER_LEFT_1, HIGH);
  digitalWrite(UPPER_RIGHT_1, HIGH);
  digitalWrite(LOWER_LEFT_1, HIGH);
  digitalWrite(LOWER_RIGHT_1, HIGH);
}

void moveRight(){
  digitalWrite(UPPER_LEFT_0, HIGH);
  digitalWrite(UPPER_RIGHT_0, LOW);
  digitalWrite(LOWER_LEFT_0, HIGH);
  digitalWrite(LOWER_RIGHT_0, LOW);
  digitalWrite(UPPER_LEFT_1, LOW);
  digitalWrite(UPPER_RIGHT_1, LOW);
  digitalWrite(LOWER_LEFT_1, LOW);
  digitalWrite(LOWER_RIGHT_1, LOW);
}

void moveLeft(){
  digitalWrite(UPPER_LEFT_0, LOW);
  digitalWrite(UPPER_RIGHT_0, HIGH);
  digitalWrite(LOWER_LEFT_0, LOW);
  digitalWrite(LOWER_RIGHT_0, HIGH);
  digitalWrite(UPPER_LEFT_1, LOW);
  digitalWrite(UPPER_RIGHT_1, LOW);
  digitalWrite(LOWER_LEFT_1, LOW);
  digitalWrite(LOWER_RIGHT_1, LOW);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(UPPER_LEFT_0, OUTPUT);
  pinMode(UPPER_LEFT_1, OUTPUT);
  pinMode(UPPER_RIGHT_0, OUTPUT);
  pinMode(UPPER_RIGHT_1, OUTPUT);
  pinMode(LOWER_LEFT_0, OUTPUT);
  pinMode(LOWER_LEFT_1, OUTPUT);
  pinMode(LOWER_RIGHT_0, OUTPUT);
  pinMode(LOWER_RIGHT_1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveForward();
  delay(3000);
  moveBackward();
  delay(3000);
}


