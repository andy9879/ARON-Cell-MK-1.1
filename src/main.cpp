#include <Arduino.h>
#include <avr/power.h>

const int NumberOfSensors = 2;
const String DepthOfSensors[NumberOfSensors] = {"3","7"};
const int PinsOfSensors[NumberOfSensors] = {A0,A2};
const int LaserPin = 4;
const int SimMosfet = 3;
const String passwd = "9879";
const String Location = "test";

void setup() {
  // put your setup code here, to run once:
  pinMode(LaserPin,OUTPUT);
  pinMode(SimMosfet,OUTPUT);
  for(int i = 0; i < NumberOfSensors; i++){
    pinMode(PinsOfSensors[i],INPUT);
  }
  Serial.begin(9600);
}

void loop() {
  digitalWrite(SimMosfet,HIGH);
  uint32_t data[NumberOfSensors][2];

  //Ambient Readings
  for(int i = 0;i < NumberOfSensors;i++){
    data[i][0] = 0;
    for(int x = 0;x < 10; x++){
      data[i][0] += analogRead(PinsOfSensors[i]);
      delay(100);
    }
  }

  //Laser Readings
  digitalWrite(LaserPin,HIGH);
  for(int i = 0;i < NumberOfSensors;i++){
    data[i][1] = 0;
    for(int x = 0;x < 10; x++){
      data[i][1] += analogRead(PinsOfSensors[i]);
      delay(100);
    }
  }
  digitalWrite(LaserPin,LOW);

  //Makes json array of data
  String json = "[";
  
  for(int i = 0;i < NumberOfSensors;i++){
    json += "{\"ambient\":" + String((data[i][0])/10) + ",";
    json += "\"laser\":" + String((data[i][1])/10) + ",";
    json += "\"depth\":" + DepthOfSensors[i] + "},";
  }
  json.remove(json.length() - 1);
  json += "]";

  //Delay to let sim7000 turn on
  delay(15000);
  //Sends text
  Serial.println("AT");
  delay(200);
  Serial.println("AT+CMGF=1");
  delay(200);
  Serial.println("AT+CMGS=\"+19046183058\"");
  delay(200);
  Serial.print(passwd + "{\"reading\":"+ json +",\"location\": \"" + Location + "\"}");
  delay(200);
  Serial.write(26);
  delay(15000);
  digitalWrite(SimMosfet,LOW);
  delay(60000);

}