byte fan = 3;


void setup() {
  Serial.begin(115200);
  pinMode(fan, OUTPUT);
}

void loop() {
 if (Serial.available())
  {
    int speed = Serial.parseInt();
    if (speed >= 0 && speed <= 1023) {
      
      analogWrite(fan, speed);
      Serial.print("fan PWM set to ");
      Serial.println(speed);
    }
  }

}
