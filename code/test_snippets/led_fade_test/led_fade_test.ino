#include "Adafruit_TLC59711.h"
#include <SPI.h>

#define data   13 // MO(SI)
#define clock  14 // SCK

#define numStrip 2

int val = 500;

Adafruit_TLC59711 tlc = Adafruit_TLC59711(numStrip, clock, data);


// Calculate based on max input size expected for one command
#define INPUT_SIZE 30  // max is L:65535 -> 7





void setup() {
  Serial.begin(9600);
  
  tlc.begin();
  tlc.write();
}

void loop() {


  for (byte i = 0; i <= 24; i++) {
    setLED(i, val);
    delay(50);
  }




/*
  if (Serial.available()) {
    int i = Serial.parseInt();
    if (i >= 0 && i <= 65535) {
      for (byte j = 0; j <= 7; j++) {
        tlc.setLED(j, i, i, i);
        tlc.write();
      }
      delay(15);
    }
  }
  */

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
void setLED(byte i, long val) {
  
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


