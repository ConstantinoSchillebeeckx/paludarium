byte fanPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);
  analogWrite(fanPin, 0);
}

void loop() {

  if (Serial.available())
  {
    int speed = Serial.parseInt();
    if (speed >= 0 && speed <= 1023)
    {
      analogWrite(fanPin, 1023);
      delay(15);
      analogWrite(fanPin, speed);
      Serial.print("PWM set to ");
      Serial.println(speed);
    }
  }
}
