/*********
  0 AS MSB
  1 AS LSB
*********/

#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <WiFiUdp.h>

ThreadController controll = ThreadController();
Thread* myThread = new Thread();

const char *ssid = "Diani y Rei";
const char *password = "Direi0810";
const int udpPort = 8888; // Choose a port number
const char *udpIP = "192.168.44.15";

bool AUTOFLAG = false;
char DIRECTION;

WiFiUDP udp;

#define UPPER_LEFT_0 0
#define UPPER_LEFT_1 1
#define UPPER_RIGHT_0 2
#define UPPER_RIGHT_1 3
#define LOWER_LEFT_0 4
#define LOWER_LEFT_1 5
#define LOWER_RIGHT_0 6
#define LOWER_RIGHT_1 7

#define LINE_RIGHT 23
#define LINE_LEFT 15

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

void wifiConection(){
  sendData("Hello from ESP32!");

  receiveData();

  delay(100);

}

void sendData(const char *data) {
  udp.beginPacket(udpIP, udpPort); // Replace with the IP of your Python script
  udp.print(data);
  udp.endPacket();
}

void receiveData() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char packetBuffer[packetSize + 1];
    udp.read(packetBuffer, packetSize);
    packetBuffer[packetSize] = '\0';
    if (packetBuffer[0] == '1'){
        AUTOFLAG = false;
    } else if (packetBuffer[0] == '0'){
        AUTOFLAG = true;
    } else {
        DIRECTION = packetBuffer[0];
    }
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize UDP
  udp.begin(udpPort);

  pinMode(UPPER_LEFT_0, OUTPUT);
  pinMode(UPPER_LEFT_1, OUTPUT);
  pinMode(UPPER_RIGHT_0, OUTPUT);
  pinMode(UPPER_RIGHT_1, OUTPUT);
  pinMode(LOWER_LEFT_0, OUTPUT);
  pinMode(LOWER_LEFT_1, OUTPUT);
  pinMode(LOWER_RIGHT_0, OUTPUT);
  pinMode(LOWER_RIGHT_1, OUTPUT);
  pinMode(TRIGGER, OUTPUT);

  pinMode(LINE_RIGHT, INPUT);
  pinMode(LINE_LEFT, INPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIGGER, LOW);

  myThread->onRun(wifiConection);
  controll.add(myThread);

}

void followLine(){
  int followerR, followerL;

  followerR = digitalRead (LINE_RIGHT);
  followerL = digitalRead (LINE_LEFT);

  if ((followerR == 0)& (followerL == 0)) // FORWARD - AVANZAR HACIA DELANTE
  {
    moveForward();
  }

  if ((followerR == 1)& (followerL == 0)) //RIGHT - DERECHA
  {
    moveRight();
  }

  if ((followerR == 0)& (followerL == 1)) //LEFT - IZQUIERDA
  {
    moveLeft(); 
  }

  if ((followerR == 1)& (followerL == 1)) //BACK - ATRÁS
  {
    // Stop - la idea es que se reanude desde control remoto
    moveBackward();

  }
  delay(75);
  stop();
  delay(150);
}


bool checkObstacle(){
  long time;
  long distance;

  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(1);          //Enviamos un pulso de 10us
  digitalWrite(TRIGGER, LOW);

  time = pulseIn(ECHO, HIGH); //obtenemos el ancho del pulso
  
  distance = time/59;         //escalamos el tiempo a una distancia en cm

  if (distance > 10){
    return true;
  } else {
    return false;
  }
}


void loop() {
  controll.run();
  if(AUTOFLAG){
    if(checkObstacle()){
      followLine();
      }
    else{
      stop();
      }
  } else {
    if(DIRECTION == '2'){
      moveForward();
    } else if(DIRECTION == '3'){
      moveBackward();
    } else if(DIRECTION == '4'){
      moveLeft();
    } else if(DIRECTION == '5'){
      moveRight();
    } else if(DIRECTION == '9'){
      stop();
    }

  }
  
  }



