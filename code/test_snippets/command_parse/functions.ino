
/*
 * Set all LED sections to given brigthness i
 * (max 65535)
 */
void setLED(long i) {
   if (i >= 0 && i <= 65535) {
    for (byte j = 0; j <= 7; j++) {
      tlc.setLED(j, i, i, i);
      tlc.write();
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
  tlc.setLED(0, val * (i == 8), val * (i == 4), val * (i == 6));
  tlc.write();
  tlc.setLED(1, val * (i == 9), val * (i == 12), val * (i == 5));
  tlc.write();
  tlc.setLED(2, val * (i == 7), val * (i == 10), val * (i == 3));
  tlc.write();
  tlc.setLED(3, val * (i == 11), val * (i == 1), val * (i == 2));
  tlc.write();


  // top LED strip
  tlc.setLED(4, val * (i == 23), val * (i == 14), val * (i == 18));
  tlc.write();
  tlc.setLED(5, val * (i == 19), val * (i == 20), val * (i == 21));
  tlc.write();
  tlc.setLED(6, val * (i == 17), val * (i == 16), val * (i == 22));
  tlc.write();
  tlc.setLED(7, val * (i == 13), val * (i == 15), val * (i == 24));
  tlc.write();
  
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
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
   tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}
