/*********
  0 AS MSB
  1 AS LSB
*********/

#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include <SoftwareSerial.h>

#define LED_ONE 0
#define LED_TWO 1

ThreadController controll = ThreadController();
Thread* myThread = new Thread();

bool START_FLAG = true;


SoftwareSerial serial_connection(10 , 11);//Create a serial connection with TX and RX on these pins
#define BUFFER_SIZE 64//This will prevent buffer overruns.
char inData[BUFFER_SIZE];//This is a character buffer where the data sent by the python script will go.
char inChar=-1;//Initialie the first character as nothing
int count=0;//This is the number of lines sent in from the python script
int i=0;//Arduinos are not the most capable chips in the world so I just create the looping variable once


void bluetoothConection(){
  //This will prevent bufferover run errors
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

  Serial.begin(9600);//Initialize communications to the serial monitor in the Arduino IDE
  serial_connection.begin(9600);//Initialize communications with the bluetooth module
  serial_connection.println("Ready!!!");//Send something to just start comms. This will never be seen.
  Serial.println("Started");//Tell the serial monitor that the sketch has started.

  pinMode(LED_ONE, OUTPUT);
  pinMode(LED_TWO, OUTPUT);

  myThread->onRun(bluetoothConection);
  myThread->setInterval(100);
  controll.add(myThread);

}

void loop() {
  controll.run();
  if (START_FLAG == true){
    digitalWrite(LED_ONE, HIGH);
    digitalWrite(LED_TWO, LOW);
  } else {
    digitalWrite(LED_ONE, LOW);
    digitalWrite(LED_TWO, HIGH);
  }

}
