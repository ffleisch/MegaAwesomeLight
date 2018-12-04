#include <Wire.h>

#include "ESP8266WiFi.h"
 
const char* ssid = "TC-A46AF";
const char* password =  "Kzkm64Kvhc56";

#define LEDPIN 2

WiFiServer wifiServer(80);

void setup() {
 
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  delay(1000);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();
}
 
void loop() {
 
  WiFiClient client = wifiServer.available();

  if (client) {
 
    while (client.connected()) {
 
      while (client.available()>0) {
        char c = client.read();
        if(c=='L'){
            digitalWrite(LEDPIN, HIGH);
        }
        if(c=='D'){
            digitalWrite(LEDPIN,LOW);
        }
        
        Serial.write(c);
      }
 
      delay(10);
    }
 
    client.stop();
    Serial.println("Client disconnected");
 
  }
}
