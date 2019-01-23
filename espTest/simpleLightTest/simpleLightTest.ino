#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "ESP8266WiFi.h"

const char* ssid = "TC-A46AF";
const char* password =  "Kzkm64Kvhc56";

WiFiServer wifiServer(80);

struct myCol {
  int r = 0;
  int g = 0;
  int b = 0;
};

struct myCol col;


// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void readRGBInt(myCol* inp, WiFiClient* myClient) {
  //long start=millis();
  bool found = false;
  char b;
  //do{
  if (myClient->available() > 0) {
    b = myClient->read();
    if (b == 'R') {
      inp->r = myClient->parseInt();
      inp->g = myClient->parseInt();
      inp->b = myClient->parseInt();
      //found=true;
      Serial.printf("r: %d g: %d b: %d set\n", inp->r, inp->g, inp->b);
      //break;
    }
  }
  //}while((millis()-start<30)&&(!found));
  //col=CRGB(inp[0],inp[1],inp[2]);
  //sprintf(msg,"%d %d %d\n",inp[0],inp[1],inp[2]);
  //Serial.print(msg);
}


void setLight(myCol* inp) {
  pwm.setPWM(0, 0, inp->r);
  pwm.setPWM(1, 0, inp->g);
  pwm.setPWM(2, 0, inp->b);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Simple Light Test!");
  delay(1000);

  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }

  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

  wifiServer.begin();

  Wire.begin(2, 0);
  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  Wire.setClock(400000);
}


void loop() {
  // put your main code here, to run repeatedly:
  col.r = 0;
  col.g = 0;
  col.b = 0;
  setLight(&col);

  col.r = 100;
  col.g = 100;
  col.b = 100;

  WiFiClient client = wifiServer.available();
  if (client) {

    while (client.connected()) {
      readRGBInt(&col, &client);
      setLight(&col);

      yield();  // take a breather, required for ESP8266
    }
    setLight(&col);

    client.stop();
    Serial.println("Client disconnected");

  }


}
