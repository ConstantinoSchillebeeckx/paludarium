byte fanOnOff = 13;
byte fanPWM = 12;

void setup() {
  Serial.begin(9600);
  pinMode(fanOnOff, OUTPUT);
  analogWrite(fanOnOff, 0);
  pinMode(fanPWM, OUTPUT);
  analogWrite(fanPWM, 0);
}

void loop() {

  if (Serial.available())
  {
    int speed = Serial.parseInt();
    if (speed >= 0 && speed <= 1023) {
      analogWrite(fanPWM, speed);
      analogWrite(fanOnOff, speed == 0 ? 0 : 1023);
      
      Serial.print("Fan set to ");
      Serial.println(speed);
    }
  }
}
