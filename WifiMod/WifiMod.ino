#include <WiFi.h>
#include <WiFiUdp.h>

#define LED 0

const char *ssid = "Diani y Rei";
const char *password = "Direi0810";
const int udpPort = 12345; // Choose a port number
const char *udpIP = "186.4.53.160";

WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  udp.begin(udpPort);
}

void loop() {

  sendData("Hello from ESP32!");

  receiveData();

  delay(1000); // Adjust the delay based on your requirements
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
    
    if (packetBuffer == "ledon"){
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
    }
  }
}
