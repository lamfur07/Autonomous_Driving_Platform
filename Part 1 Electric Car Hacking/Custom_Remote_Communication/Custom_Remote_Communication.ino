#include <Adafruit_MCP4725.h>
#include <Wire.h>
#define THRESHOLD 1450

Adafruit_MCP4725 dac_steering;
Adafruit_MCP4725 dac_throttle;

int r_steering; // Receiver Channel 1
int r_throttle; // Receiver Channel 2
int w_steering; // Write DAC Steering
int w_throttle; // Write DAC Throttle
int state_channel; // Receiver Channel 4

bool brake_state;
bool gear_state = LOW;
int camera_state = 0; // State '0' = !Record; State '1' = Record;
int ch5;
int throttle_max = 1050;
float throttle_percentage;
int updated_throttle;

void setup() {
  pinMode(5, INPUT); // Steering Channel 1 in receiver
  pinMode(6, INPUT); // Throttle Channel 2 in receiver
  pinMode(9, INPUT); // Recording/Gear Channel 4 in receiver
  pinMode(13, OUTPUT); // Brake pin
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
  ch5 = pulseIn(4, HIGH);
  throttle_percentage = (float(ch5) - 992.0) / 991.0;
  updated_throttle = throttle_percentage * throttle_max;
  updated_throttle = int(updated_throttle);
  
  if (r_throttle < THRESHOLD) {
    brake_state = LOW;
  }
  else if (r_throttle > THRESHOLD) {
    r_throttle = 1490;
    brake_state = HIGH;
  }
  w_throttle = map(r_throttle, 993, 1490, updated_throttle, 0);
  Serial.print(w_throttle);
  dac_throttle.setVoltage(w_throttle, false);
  digitalWrite(13, brake_state);
  Serial.print("x");

  // State Channel (Gear State && Camera State)
  state_channel = pulseIn(9, HIGH);
  if (state_channel > 980 && state_channel < 1000) {
    gear_state = HIGH;
    digitalWrite(2, gear_state);
    camera_state = 0;
    Serial.println(camera_state);
  }
  else if (state_channel > 1480 && state_channel < 1500) {
    gear_state = LOW;
    digitalWrite(2, gear_state);
    camera_state = 0;
    Serial.println(camera_state);
  }
  else if (state_channel > 1980 && state_channel < 2000) {
    gear_state = LOW;
    digitalWrite(2, gear_state);
    camera_state = 1;
    Serial.println(camera_state);
  }

  delay(50);
}
