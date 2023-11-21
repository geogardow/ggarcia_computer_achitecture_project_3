#include <WiFi.h>
#include <WiFiUdp.h>

#define LED 0
const char *ssid = "Diani y Rei";
const char *password = "Direi0810";
const int udpPort = 8888; // Choose a port number

WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize UDP
  udp.begin(udpPort);
  pinMode(LED, OUTPUT);

}

void loop() {
  // Read sensor data or perform any other task

  // Send data to Python script
  sendData("Hello from ESP32!");

  // Receive data from Python script
  receiveData();

  delay(1000); // Adjust the delay based on your requirements
}

void sendData(const char *data) {
  udp.beginPacket("192.168.44.23", udpPort); // Replace with the IP of your Python script
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
        digitalWrite(LED, HIGH);
    }else if (packetBuffer[0] == '0'){
        digitalWrite(LED, LOW);
    }
  }
}
