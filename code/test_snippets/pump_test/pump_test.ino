byte pumpPin = 14;

void setup() {
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
  analogWrite(pumpPin, 0);
}

void loop() {

  if (Serial.available())
  {
    int speed = Serial.parseInt();
    if (speed >= 0 && speed <= 1023)
    {
      analogWrite(pumpPin, 1023);
      delay(15);
      analogWrite(pumpPin, speed);
      Serial.print("PWM set to ");
      Serial.println(speed);
    }
  }
}
