#include <Adafruit_MCP4725.h>
#include <Wire.h>


#define Low_THRESHOLD 1450
#define HIGH_THRESHOLD 1550


Adafruit_MCP4725 dac_steering;
Adafruit_MCP4725 dac_throttle;

int r_steering;
int r_throttle; 
bool r_brake;

int w_steering; 
int w_throttle; 
bool w_brake; 


void setup() {
  pinMode(5, INPUT); //set pin 5 to read steering input 
  pinMode(6, INPUT); //set pin 6 to read throttle/brake input
  pinMode(7, INPUT); //Button input from the receiver


  Serial.begin(9600);

  dac_steering.begin(0x60);
  dac_throttle.begin(0x61);
  pinMode(13, OUTPUT);
}


void loop() {
    //read input from the receiver
    r_steering = pulseIn(5, HIGH, 25000);
    r_throttle = pulseIn(5, HIGH, 25000);
    data_collect = digitalRead(7); //either 0 or 1

    //check the state
    if (r_throttle<HIGH_THRESHOLD){
        r_throttle = pulseIn(5, HIGH, 25000);
        r_brake = LOW;
      }
    else if (r_throttle>HIGH_THRESHOLD){
        r_throttle = 1490;
        r_brake = HIGH;
      }

    //map the read signals
    w_steering = map(r_steering, 993, 1987, 1365, 2730);
    w_throttle = map(r_throttle, 993, 1490, 2730, 1365); //check with Imad/Abduallah

    //write the mapped signals
    dac_steering.setVoltage(w_steering, false); 
    dac_throttle.setVoltage(w_throttle, false); 
    digitalWrite(13, r_brake);

    //delay
    delay(50);

    //print output
    Serial.print(r_steering);
    Serial.print("x"); 
    Serial.print(r_throttle);
    Serial.print("x");
    Serial.print(data_collect); 

}
