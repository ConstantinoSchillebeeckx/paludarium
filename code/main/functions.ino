
void parseSerialComm() {
  // Get next command from Serial (add 1 for final 0)
  char input[INPUT_SIZE + 1];
  byte size = Serial.readBytes(input, INPUT_SIZE);
  
  // Add the final 0 to end the C string
  input[size] = 0;

  // Read each command pair, split on ':'
  // e.g. L:65535
  // L -> LED
  // P -> pump
  // F -> fan
  char* tok = strtok(input, ":");
  while (tok != NULL) {

    if (strchr(tok, 'L')) { // LED COMMANDS
      Serial.print("LED command: ");
      tok = strtok(NULL, ":"); // get next token
      
      long val = atoi(tok);
      Serial.println(val);
      setLED(val);
    } else if (strchr(tok, 'P')) { // PUMP COMMANDS
      Serial.print("Pump command: ");
      tok = strtok(NULL, ":"); // get next token
      
      long val = atoi(tok);
 
      if (val >= 0 && val <= 1023) {
        analogWrite(pumpPin, 1023);
        delay(100);
        analogWrite(pumpPin, val);
        Serial.println(val);
      }
    } else if (strchr(tok, 'F')) { // FAN COMMANDS
      Serial.print("Fan command: ");
      tok = strtok(NULL, ":"); // get next token

      long val = atoi(tok);
 
      if (val >= 0 && val <= 1023) {
        analogWrite(fanPWM, val);
        analogWrite(fanOnOff, val == 0 ? 0 : 1023);
        Serial.println(val);
      }
    }
    
    tok = strtok(NULL, ":"); // get next token
  }

}


/*
 * Set all LED sections to given brigthness i
 * (max 65535)
 */
void setLED(long i) {
   if (i >= 0 && i <= 65535) {
    for (byte j = 0; j <= 7; j++) {
      led.setLED(j, i, i, i);
      led.write();
    }
  }
}







/*
 * Set LED section brigthness.
 * NOTE: all other LEDs will be
 * turned off.
 * 
 * Params:
 * -------
 * i - index for LED section (1 to 24)
 * val - brigthness value (0 to 65535
 * 
 */
void setLEDSection(byte i, long val) {
  
  // bottom LED strip
  led.setLED(0, val * (i == 8), val * (i == 4), val * (i == 6));
  led.write();
  led.setLED(1, val * (i == 9), val * (i == 12), val * (i == 5));
  led.write();
  led.setLED(2, val * (i == 7), val * (i == 10), val * (i == 3));
  led.write();
  led.setLED(3, val * (i == 11), val * (i == 1), val * (i == 2));
  led.write();


  // top LED strip
  led.setLED(4, val * (i == 23), val * (i == 14), val * (i == 18));
  led.write();
  led.setLED(5, val * (i == 19), val * (i == 20), val * (i == 21));
  led.write();
  led.setLED(6, val * (i == 17), val * (i == 16), val * (i == 22));
  led.write();
  led.setLED(7, val * (i == 13), val * (i == 15), val * (i == 24));
  led.write();
  
}





// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 60 seconds...");
       mqtt.disconnect();
       delay(60000);  // wait 60 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

