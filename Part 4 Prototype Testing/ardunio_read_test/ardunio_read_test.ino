#include <Adafruit_MCP4725.h>
#include <Wire.h>

Adafruit_MCP4725 dac_steering;
Adafruit_MCP4725 dac_throttle;

String incomingByte ;
String AI_steering;
String AI_throttle;
int AI_str;
int AI_thr;
int brake_pin = 13;
bool brake_state = LOW;

void setup() {
  pinMode(brake_pin, OUTPUT); // Brake pin
  Serial.begin(9600);
  dac_steering.begin(0x60);
  dac_throttle.begin(0x61);
  
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.readStringUntil('\n');
    AI_steering = (incomingByte.substring(0, 4));
    AI_throttle = (incomingByte.substring(4, 8));

    AI_str = AI_steering.toInt();
    AI_thr = AI_throttle.toInt();
    //r_throttle = pulseIn(ch_2, HIGH);
    brake_state = LOW;

    //if (r_throttle > AI_THRESHOLD) {
      //brake_state = HIGH;
      //AI_str = 2047;
      //AI_thr = 0;
    //}
    dac_steering.setVoltage(AI_str, false);
    dac_throttle.setVoltage(AI_thr, false);
    digitalWrite(brake_pin, brake_state);


//    Serial.print(AI_str);
//
//    Serial.print('x') ;
//    Serial.println(AI_thr);
    delay(50);
  }
}

//void setup(){
//}
//void loop(){
//}
//
