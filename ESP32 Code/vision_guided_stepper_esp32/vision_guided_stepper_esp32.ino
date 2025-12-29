#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "NOT FBI";
const char* password = "ezpassword1234";
WiFiUDP udp;
const int localPort = 4210;
const int statusLed = 2; // Built-in LED on most ESP32s
unsigned long lastPacketTime = 0;
const int connectionTimeout = 2000; // 2 seconds of no data = "Lost PC"

void setup() {
  // Serial 0 is for Debugging (USB)
  delay(1000);
  Serial.begin(115200);
  pinMode(statusLed, OUTPUT);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  // Serial 2 is for Arduino Communication (GPIO 17 TX)
  // Use 9600 to ensure the Arduino doesn't drop pulses
  

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(statusLed, !digitalRead(statusLed));
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected. IP: " + WiFi.localIP().toString());
  udp.begin(localPort);
  
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
     
    char incomingChar = udp.read(); // Get the command (L, R, S, etc.)

    lastPacketTime = millis(); // Reset the "Good Connection" timer
     // Rapid flicker to show data is flowing
    digitalWrite(statusLed, HIGH); 
    
    // Forward the command to the Arduino
    Serial2.print(incomingChar);
    
    // Optional: Log to PC for debugging
    Serial.print("Forwarded to Arduino: ");
    Serial.println(incomingChar);
  }
  else {
    // If no packet received for a while, show a "Waiting" pulse
    if (millis() - lastPacketTime > connectionTimeout) {
      // Slow pulse (Heartbeat)
      bool ledState = (millis() / 1000) % 2; 
      digitalWrite(statusLed, ledState);
    }
  }
}
