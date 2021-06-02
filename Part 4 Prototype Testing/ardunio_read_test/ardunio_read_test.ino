String incomingByte ;    
String AI_steering;
String AI_throttle; 


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.readStringUntil('\n');
    AI_steering = (incomingByte.substring(0,4)).toInt();
    AI_throttle = (incomingByte.substring(4,8)).toInt();

    Serial.print(AI_steering);
    
    Serial.print('x') ;
    Serial.println(AI_throttle);
    delay(50);
  }
}
