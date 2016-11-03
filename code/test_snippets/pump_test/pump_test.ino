void setup() {
  Serial.begin(9600);
  pinMode(14, OUTPUT);
  analogWrite(14, 0);
}

void loop() {

  if (Serial.available())
  {
    int speed = Serial.parseInt();
    if (speed >= 0 && speed <= 1023)
    {
      analogWrite(14, 1023);
      delay(15);
      analogWrite(14, speed);
      Serial.print("PWM set to ");
      Serial.println(speed);
    }
  }
}
