#include <Adafruit_MCP4725.h>
#include <Wire.h>
#define THRESHOLD 1450

Adafruit_MCP4725 dac_steering;
Adafruit_MCP4725 dac_throttle;

// Assigned Pins on MCU
int ch_1 = 7;
int ch_2 = 6;
int ch_4 = 5;
int ch_5 = 4;
int brake_pin = 13;
int gear_pin = 2;

// Assigned Variables
int r_steering; // Receiver Channel 1
int r_throttle; // Receiver Channel 2
int w_steering; // Write DAC Steering
int w_throttle; // Write DAC Throttle

// Assigned Variables for Receiver Channel 4 "State '0' = !Record; State '1' = Record"
bool brake_state;
bool gear_state = LOW;
int state_channel;
int camera_state = 0;
int throttle_tune;
int throttle_max = 1050;
int updated_throttle;
float throttle_percentage;

void setup() {
  pinMode(ch_1, INPUT); // Steering Channel 1 in receiver
  pinMode(ch_2, INPUT); // Throttle Channel 2 in receiver
  pinMode(ch_4, INPUT); // Recording/Gear Channel 4 in receiver
  pinMode(ch_5, INPUT); // Throttle Tuning Channel 5 in reeiver
  pinMode(brake_pin, OUTPUT); // Brake pin
  pinMode(gear_pin, OUTPUT); // Gear pin

  Serial.begin(9600);
  dac_steering.begin(0x60);
  dac_throttle.begin(0x61);
}

void loop() {
  // Steering Control
  r_steering = pulseIn(ch_1, HIGH);
  w_steering = map(r_steering, 993, 1987, 1365, 2730);
  Serial.print(w_steering);
  dac_steering.setVoltage(w_steering, false);
  Serial.print("x");

  // Throttle Control
  r_throttle = pulseIn(ch_2, HIGH);
  throttle_tune = pulseIn(ch_5, HIGH);
  throttle_percentage = (float(throttle_tune) - 992.0) / 991.0;
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
  digitalWrite(brake_pin, brake_state);
  Serial.print("x");

  // State Control (Gear State && Camera State)
  state_channel = pulseIn(ch_4, HIGH);
  if (state_channel > 980 && state_channel < 1000) {
    gear_state = HIGH;
    digitalWrite(gear_pin, gear_state);
    camera_state = 0;
    Serial.println(camera_state);
  }
  else if (state_channel > 1480 && state_channel < 1500) {
    gear_state = LOW;
    digitalWrite(gear_pin, gear_state);
    camera_state = 0;
    Serial.println(camera_state);
  }
  else if (state_channel > 1980 && state_channel < 2000) {
    gear_state = LOW;
    digitalWrite(gear_pin, gear_state);
    camera_state = 1;
    Serial.println(camera_state);
  }

  delay(50);
}
