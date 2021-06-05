#include <Adafruit_MCP4725.h>
#include <Wire.h>

#define THRESHOLD 1450
#define AI_THRESHOLD 1530
#define MANUAL_state 0
#define AI_state 1
int g_current_state;

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

// AI mode varibles
String incomingByte ;
String AI_steering;
String AI_throttle;
int AI_str;
int AI_thr;

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
  switch (g_current_state) {
    case MANUAL_state:
      manual_mode();
      break;
    case AI_state:
      ai_mode();
      break;
    default:
      manual_mode();
  }
  check_current_state();
  delay(50);
}

void check_current_state() {
  state_channel = pulseIn(ch_4, HIGH);
  if (state_channel > 980 && state_channel < 1000) {
    gear_state = HIGH;
    digitalWrite(gear_pin, gear_state);
    g_current_state = MANUAL_state;
  }
  else if (state_channel > 1480 && state_channel < 1500) {
    gear_state = LOW;
    digitalWrite(gear_pin, gear_state);
    g_current_state = MANUAL_state;
  }
  else if (state_channel > 1980 && state_channel < 2000) {
    gear_state = LOW;
    digitalWrite(gear_pin, gear_state);
    g_current_state = AI_state;
  }
}

void manual_mode () {
  // Steering Control
  r_steering = pulseIn(ch_1, HIGH);
  w_steering = map(r_steering, 993, 1987, 1365, 2730);
  dac_steering.setVoltage(w_steering, false);

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
  dac_throttle.setVoltage(w_throttle, false);
  digitalWrite(brake_pin, brake_state);
}

void ai_mode() {
  if (Serial.available() > 0) {
    incomingByte = Serial.readStringUntil('\n');
    AI_steering = (incomingByte.substring(0, 4));
    AI_throttle = (incomingByte.substring(4, 8));

    AI_str = AI_steering.toInt();
    AI_thr = AI_throttle.toInt();
    r_throttle = pulseIn(ch_2, HIGH);
    brake_state = LOW;

    if (r_throttle > AI_THRESHOLD) {
      brake_state = HIGH;
      AI_str = 2047;
      AI_thr = 0;
    }
    dac_steering.setVoltage(AI_str, false);
    dac_throttle.setVoltage(AI_thr, false);
    digitalWrite(brake_pin, brake_state);
  }
}
