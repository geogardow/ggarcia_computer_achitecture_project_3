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

#define LINE_RIGHT 8
#define LINE_LEFT 9

#define TRIGGER 12
#define ECHO 13

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

void stop(){
  digitalWrite(UPPER_LEFT_0, LOW);
  digitalWrite(UPPER_RIGHT_0, LOW);
  digitalWrite(LOWER_LEFT_0, LOW);
  digitalWrite(LOWER_RIGHT_0, LOW);
  digitalWrite(UPPER_LEFT_1, LOW);
  digitalWrite(UPPER_RIGHT_1, LOW);
  digitalWrite(LOWER_LEFT_1, LOW);
  digitalWrite(LOWER_RIGHT_1, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(UPPER_LEFT_0, OUTPUT);
  pinMode(UPPER_LEFT_1, OUTPUT);
  pinMode(UPPER_RIGHT_0, OUTPUT);
  pinMode(UPPER_RIGHT_1, OUTPUT);
  pinMode(LOWER_LEFT_0, OUTPUT);
  pinMode(LOWER_LEFT_1, OUTPUT);
  pinMode(LOWER_RIGHT_0, OUTPUT);
  pinMode(LOWER_RIGHT_1, OUTPUT);

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIGGER, LOW);
}

void followLine(){
  int followerR, followerL;

  followerR = digitalRead (LINE_RIGHT);
  followerL = digitalRead (LINE_LEFT);

  if ((followerR == 0)& (followerL == 0)) // FORWARD - AVANZAR HACIA DELANTE
  {
    moveForward();
  }

  if ((followerR == 0)& (followerL == 1)) //RIGHT - DERECHA
  {
    moveRight();
  }

  if ((followerR == 1)& (followerL == 0)) //LEFT - IZQUIERDA
  {
    moveLeft();
  }

  if ((followerR == 1)& (followerL == 1)) //BACK - ATRÁS
  {
    moveBackward();
  }
}

void skipObstacle(){
  moveBackward();
  delay(500);
  moveRight();
  delay(500);
  stop();
}

bool checkObstacle(){
  long time;
  long distance;

  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(1);          //Enviamos un pulso de 10us
  digitalWrite(TRIGGER, LOW);

  time = pulseIn(ECHO, HIGH); //obtenemos el ancho del pulso
  
  distance = time/59;         //escalamos el tiempo a una distancia en cm

  if (distance > 25){
    return true;
  } else {
    return false;
  }
}

void loop() {
  if (checkObstacle()){
    moveForward();
  } else {
    skipObstacle();
  }
}


