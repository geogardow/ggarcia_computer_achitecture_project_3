/*********
  0 AS MSB
  1 AS LSB
*********/

#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include <SoftwareSerial.h>

ThreadController controll = ThreadController();
Thread* myThread = new Thread();

SoftwareSerial serial_connection(10, 11);//Create a serial connection with TX and RX on these pins
#define BUFFER_SIZE 64//This will prevent buffer overruns.
char inData[BUFFER_SIZE];//This is a character buffer where the data sent by the python script will go.
char inChar=-1;//Initialie the first character as nothing
int count=0;//This is the number of lines sent in from the python script
int i=0;//Arduinos are not the most capable chips in the world so I just create the looping variable once

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

bool START_FLAG = false;

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

void bluetoothConection(){
  //This will prevent bufferoverrun errors
  byte byte_count=serial_connection.available();//This gets the number of bytes that were sent by the python script
  if(byte_count)//If there are any bytes then deal with them
  {
    Serial.println("Incoming Data");//Signal to the monitor that something is happening
    int first_bytes=byte_count;//initialize the number of bytes that we might handle.
    int remaining_bytes=0;//Initialize the bytes that we may have to burn off to prevent a buffer overrun
    if(first_bytes>=BUFFER_SIZE-1)//If the incoming byte count is more than our buffer...
    {
      remaining_bytes=byte_count-(BUFFER_SIZE-1);//Reduce the bytes that we plan on handleing to below the buffer size
    }
    for(i=0;i<first_bytes;i++)//Handle the number of incoming bytes
    {
      inChar=serial_connection.read();//Read one byte
      inData[i]=inChar;//Put it into a character string(array)
    }
    inData[i]='\0';//This ends the character array with a null character. This signals the end of a string


    //We have read, here we add the options
    if(String(inData)=="START")//From Python script Turn LED ON when transmission starts
    {
      Serial.println("********* Transmission Started *********");
      START_FLAG = true;
    }
    else if(String(inData)=="STOP")//Turn OFF LED when transmission Stops
    {
      Serial.println("********* Transmission Ended *********");
      START_FLAG = false;
    }
    for(i=0;i<remaining_bytes;i++)//This burns off any remaining bytes that the buffer can't handle.
    {
      inChar=serial_connection.read();
    }
    Serial.println(inData);//Print to the monitor what was detected
    serial_connection.println("Bluetooth answers "+String(count)+": "+inData+" received");//Then send an incremented string back to the python script
    count++;//Increment the line counter
  }
}

void setup() {
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

  Serial.begin(9600);//Initialize communications to the serial monitor in the Arduino IDE
  serial_connection.begin(9600);//Initialize communications with the bluetooth module
  serial_connection.println("Ready!!!");//Send something to just start comms. This will never be seen.
  Serial.println("Started");//Tell the serial monitor that the sketch has started.

  myThread->onRun(bluetoothConection);
  myThread->setInterval(100);
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
  //controll.run();
  //if (START_FLAG){
  if (true){
    if (checkObstacle()){
    //if (true){
      //followLine();
      moveForward();
    } else {
      skipObstacle();
    }
  }
}


