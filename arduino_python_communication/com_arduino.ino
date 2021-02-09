
int sensorA = A0;
int sensorB = A1;


void setup(){
  Serial.begin(9600);
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
}

void loop(){
  int sensorA_val = analogRead(sensorA);
  int sensorB_val = analogRead(sensorB);
  
  Serial.print(sensorA_val);
  Serial.print("x");
  Serial.println(sensorB_val);
  delay(50);
}

/*
String InBytes;

void setup(){
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
  }

void loop(){
    if (Serial.available()>0){        
        Serial.write("1");
        
        InBytes = Serial.readStringUntil('\n');
        
        if (InBytes == "on"){
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.write("LED is on");
          }
        if (InBytes == "off"){
            digitalWrite(LED_BUILTIN, LOW);
            Serial.write("LED is off");
          }
        else{
            Serial.write("invalid");
          }
         
      }
  }
*/
