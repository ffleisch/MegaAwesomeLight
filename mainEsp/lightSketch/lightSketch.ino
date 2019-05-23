#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "ESP8266WiFi.h"

WiFiServer wifiServer(80);

char ssid[64] = "Gulasch mit Kraut";//"TC-A46AF";
char password[64] = "basis nord richard blau";//"Kzkm64Kvhc56";


boolean useFade = false;
int fadeTime = 1000;
boolean useBlink = false;
int blinkTime = 500;


struct longCol {
  int r;
  int g;
  int b;
};


//ein schritt im farbverlauf
struct colStep {
  longCol myCol;
  int duration;
};


colStep* fadeTable;
int tableLength=3;


struct longCol theTable[256];
struct longCol col = (longCol) {
  0, 0, 0
};
struct longCol absWhite = (longCol) {
  4095, 3500, 2500
};

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

float dimFuncLog(float val) {
  return (pow(500, val - 1)); //from Texas Instruments
}

float dimFuncQuad(float val) {
  return (pow(val, 2));
}
float dimFuncLin(float val) {
  return (val);
}

void calcTable(float (*func)(float)) {
  for (int i = 0; i < 256; i++) {
    theTable[i].r = absWhite.r * (*func)(i / 255.f);
    theTable[i].g = absWhite.g * (*func)(i / 255.f);
    theTable[i].b = absWhite.b * (*func)(i / 255.f);
  }
}

void setLight(longCol* inp) {
  pwm.setPWM(0, 0, inp->r);
  pwm.setPWM(1, 0, inp->g);
  pwm.setPWM(2, 0, inp->b);
}

void setLightDebug(longCol* inp){
  Serial.println("set Light:");
  Serial.println(inp->r);
  Serial.println(inp->g);
  Serial.println(inp->b);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Super Mega Awesome Light!");



  //debug
  fadeTable=(colStep*)malloc(sizeof(colStep)*3);
  fadeTable[0]={.myCol={.r=50,.g=2000,.b=1300},.duration=500};
  fadeTable[1]={.myCol={.r=3000,.g=1000,.b=10},.duration=2000};
  fadeTable[2]={.myCol={.r=0,.g=4000,.b=10},.duration=1100};
    

  
  //fill lookup table
  calcTable(&dimFuncLog);

  //connect to wifi
  connectWifi();
  wifiServer.begin();
  Serial.println("WiFi Server started");

  //start pwm
  Wire.begin(2, 0);
  pwm.begin();
  pwm.setPWMFreq(1600);
  Wire.setClock(400000);
}

#define COMMAND 0x80
#define SEND_TABLE 0x1
//#define
#define CMD_LENGTH 3

byte bytes[CMD_LENGTH];

void readTimeTable(Stream* inp, int num) {
  for(int i=0;i<num*sizeof(colStep);i++){
    
  }
}


colStep* currentStep=fadeTable;
colStep* nextStep=fadeTable;
int lastStepStart=0;

void setLightFromTable(){
    Serial.println("SetFromTable");
    if(millis()-lastStepStart>=currentStep->duration){
      currentStep=nextStep;
      //TODO dieser scheiss
      nextStep=fadeTable+(currentStep-fadeTable+1)%tableLength;
      lastStepStart=millis();    
    }
    
    //setLightDebug(&currentStep->myCol);
    //setLight(&currentStep->myCol);
}

void readCommand(Stream* inpStream) {
  char b;
  if (inpStream->available() > 0) {
    b = inpStream->read();

    while (inpStream->available() < CMD_LENGTH) {
      yield();
    }

    for (int i = 0; i < CMD_LENGTH; i++) {
      bytes[i] = inpStream->read();
    }

    if (b && COMMAND) {
      b &= 127;
      byte cmd = b && 0xF0;

      Serial.printf("%x\n", b);
      Serial.printf("%x\n", cmd);

      if (b ^ SEND_TABLE) {
        short num=*((short*)(b+1));
        Serial.printf("num: %d\n", num);

        //TODO
        //readTable(inpStream,num);
      }

    }


    /*
      if (b == 'C') {
      while (inpStream->available() < 3) {
        //Serial.println("incomplete color!");
        yield();
      }
      col.r = theTable[inpStream->read()].r;
      col.g = theTable[inpStream->read()].g;
      col.b = theTable[inpStream->read()].b;
      }
      if (b == 'S') {
      char command[64];
      char body[64];
      inpStream->readBytesUntil('=', command, 64);
      //b = inpStream->read();
      inpStream->readBytesUntil('\n', body, 64);
      inpStream->println(command);
      inpStream->println(body);

      int num = 0;
      if (!strcmp(command, "FADE")) {
        sscanf(body, "%i", &num);
        useFade = num;
        inpStream->println(useFade);
      }
      if (!strcmp(command, "FADET")) {
        sscanf(body, "%i", &num);
        fadeTime = num;
        inpStream->println(fadeTime);
      }
      if (!strcmp(command, "BLINK")) {
        sscanf(body, "%i", &num);
        useBlink = num;
        inpStream->println(useBlink);
      }
      if (!strcmp(command, "BLINKT")) {
        sscanf(body, "%i", &num);
        blinkTime = num;
        inpStream->println(blinkTime);
      }
      if (!strcmp(command, "TMODE")) {
        if (!strcmp(body, "QUAD")) {
          calcTable(&dimFuncQuad);
          inpStream->println("QUAD");
        }
        if (!strcmp(body, "LOG")) {
          calcTable(&dimFuncLog);
          inpStream->println("LOG");
        }
        if (!strcmp(body, "LIN")) {
          calcTable(&dimFuncLin);
          inpStream->println("LIN");
        }
      }
      if (!strcmp(command, "PASS")) {
        strcpy(password, body);
      }
      if (!strcmp(command, "SSID")) {
        strcpy(ssid, body);
      }
      if (!strcmp(command, "NORM")) {
        sscanf(body, "%i %i %i", &absWhite.r, &absWhite.g, &absWhite.b);
        inpStream->println(absWhite.r);
        inpStream->println(absWhite.g);
        inpStream->println(absWhite.b);
      }
      if (!strcmp(command, "RESET")) {
        inpStream->println("Resetting wifi");
        resetWifi();
      }
      if (!strcmp(command, "TABLE")) {
        for (int i = 0; i < 256; i++) {
          inpStream->printf("%d: %d,%d,%d\n", i, theTable[i].r, theTable[i].g, theTable[i].b);
        }
      }
      }
      //*/
  }

}

void readColor() {
  //Todo
}

void connectWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
    readCommand(&Serial);
  }
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
}

void resetWifi() {
  wifiServer.stop();
  WiFi.disconnect();
  connectWifi();
  wifiServer.begin();
  Serial.println("WiFi Server started");
};


void loop() {
  //setLight(&col);
  //setLightDebug(&fadeTable[0].myCol);
  WiFiClient myClient = wifiServer.available();
  if (myClient) {
    Serial.println("Client connected");
    //myClient.printf("Client connected");
    while (myClient.connected()) {
      readCommand(&myClient);
      readCommand(&Serial);
      yield();  // take a breather, required for ESP8266
    }

    myClient.stop();
    Serial.println("Client disconnected");
  }
  readCommand(&Serial);
  setLightFromTable();

}
