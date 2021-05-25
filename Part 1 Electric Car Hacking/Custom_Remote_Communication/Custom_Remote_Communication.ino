#include <Adafruit_MCP4725.h>
#include <Wire.h>
#define THRESHOLD 1450

Adafruit_MCP4725 dac_steering;
Adafruit_MCP4725 dac_throttle;

int r_steering; // Receiver Channel 1
int r_throttle; // Receiver Channel 2
int w_steering;
int w_throttle;
int is_recording; // Receiver Channel 4
bool r_brake;
bool w_brake;
int ch5;

void setup() {
  pinMode(5, INPUT); // Steering Channel 1 in receiver
  pinMode(6, INPUT); // Throttle Channel 2 in receiver
  pinMode(9, INPUT); // Recording Channel 4 in receiver
  pinMode(13, OUTPUT); // Brake pin
  pinMode(10, OUTPUT); // LED ON = Record; LED OFF = !Record;
  pinMode(2, OUTPUT); // Gear pin

  Serial.begin(9600);
  dac_steering.begin(0x60);
  dac_throttle.begin(0x61);
}

void loop() {
  // Steering Channel
  r_steering = pulseIn(5, HIGH);
  w_steering = map(r_steering, 993, 1987, 1365, 2730);
  Serial.print(w_steering);
  dac_steering.setVoltage(w_steering, false);
  Serial.print("x");

  // Throttle Channel
  r_throttle = pulseIn(6, HIGH);
  if (r_throttle < THRESHOLD) {
    r_brake = LOW;
  }
  else if (r_throttle > THRESHOLD) {
    r_throttle = 1490;
    r_brake = HIGH;
  }
  w_throttle = map(r_throttle, 993, 1490, 1050, 0);
  Serial.print(r_throttle);
  dac_throttle.setVoltage(w_throttle, false);
  digitalWrite(13, r_brake);
  Serial.print("x");
  Serial.println();

  // Gear
  ch5=pulseIn(4,HIGH);
  if(ch5>1492){digitalWrite(2,HIGH);}
  else{digitalWrite(2,LOW);}
  
  // LED isRecord Channel
  is_recording = pulseIn(9, HIGH, 25000);
  if (is_recording > 1400 && is_recording < 1600){
    digitalWrite(10,HIGH);
    Serial.println("1");
    }
  else{
    digitalWrite(10,LOW);
    Serial.println("0");
    }
  delay(50);
}
