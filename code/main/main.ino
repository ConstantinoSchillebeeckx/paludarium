#include "Adafruit_TLC59711.h"
#include <SPI.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_SSD1306.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include "Adafruit_Si7021.h"



#define data   13 // MO(SI)
#define clock  14 // SCK

#define numStrip 2

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "interpol"
#define WLAN_PASS       "schillebeeckx"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "constantinoschillebeeckx"
#define AIO_KEY         "f40d869993b5cc6315c96943bf7d005e4246fc86"

Adafruit_TLC59711 led = Adafruit_TLC59711(numStrip, clock, data); // LED driver
Adafruit_SSD1306 display = Adafruit_SSD1306(); // OLED display
//Adafruit_TSL2561_Unified lux = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345); // light sensor
Adafruit_Si7021 sensor = Adafruit_Si7021(); // temp & humidity

// Calculate based on max input size expected for one command
#define INPUT_SIZE 7  // max is L:65535 -> 7

#define BUTTON_A 0
#define BUTTON_B 16
#define BUTTON_C 2
#define LED      0

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup a feed for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish tempMQTT = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");
Adafruit_MQTT_Publish humidMQTT = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");


#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif



unsigned long previousMillis = 0;   
const long interval = 15 * 60 * 1000; // 15 min

uint32_t temp=0;
uint32_t humid=0;
uint32_t prevTemp=0;
uint32_t prevHumid=0;
//uint32_t lum=0;

byte pumpPin = 15;
byte fanOnOff = 13;
byte fanPWM = 12;


#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {

  #if !defined(ESP8266)
    while (!Serial) delay(1);
  #endif
  
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);


  pinMode(pumpPin, OUTPUT);
  analogWrite(pumpPin, 0);
  pinMode(fanOnOff, OUTPUT);
  analogWrite(fanOnOff, 0);
  pinMode(fanPWM, OUTPUT);
  analogWrite(fanPWM, 0);
  delay(10);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  display.println("Connecting to: ");
  display.println(WLAN_SSID);
  yield();
  display.display();

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
    

  if (!sensor.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
  
  /* Setup the sensor gain and integration time */
//  configureSensor();
  
  led.begin();
  led.write();

  // get initial reading
  temp = sensor.readTemperature() * 9.0/5 + 32;
  humid = sensor.readHumidity();
}

void loop() {

  delay(1000);
  unsigned long currentMillis = millis();

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  
  MQTT_connect();

  temp = sensor.readTemperature() * 9.0/5 + 32;
  humid = sensor.readHumidity();

  if (abs(currentMillis - previousMillis) >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    
    Serial.print(temp);
    Serial.print(" ");
    Serial.println(humid);
    
    tempMQTT.publish(temp);
    humidMQTT.publish(humid);

  }


  // keep the connection alive
  mqtt.ping();


  // update display if temp/humidity has changed
  if (prevHumid != humid || prevTemp != temp) {
    display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.print("Temp: ");
    display.println(temp);
    display.print("Humidity: ");
    display.println(humid);
    yield();
    display.display();
  }
  prevHumid = humid;
  prevTemp = temp;




  // PARSE COMMANDS ON SERIAL
  parseSerialComm();

  
}


