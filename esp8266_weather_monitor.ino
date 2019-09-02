#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define DHTPIN 14     // D5 connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const int sensor_pin = A0;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  delay(1000);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1,5);
  display.print("SOIL MOISTURE %");
  display.display();
}

void loop() {
  
  float moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
  
  float h = dht.readHumidity();
  
  float t = dht.readTemperature(); // Read temperature as Celsius (by default)

  float hic = dht.computeHeatIndex(t, h, false); // Compute heat index in Celsius (isFahreheit = false)

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
//---Serial block
  Serial.print("Soil Moisture = ");
  Serial.print(moisture_percentage);
  Serial.print("%");
  Serial.print(F(" || Humidity: "));
  Serial.print(h);
  Serial.print(F("% || Temperature: "));
  Serial.print(t);
  Serial.print(F(" C || Heat index: "));
  Serial.print(hic);
  Serial.println(F(" C "));
//---End of Serial block

//---Start of screen output
  display.clearDisplay();
  display.setCursor(1,5);
  display.print("SOIL MOISTURE % ");
  //display.setCursor(40, 20);
  display.print(moisture_percentage);
  display.setCursor(15, 20);
  display.print("HUMIDITY: ");
  display.print(h);
  display.setCursor(15, 35);
  display.print("TEMP (C): ");
  display.print(t);
  display.setCursor(5, 50);
  display.print("Heat index: ");
  display.print(hic);
  display.display();
//---End of screen output

  delay(1000);
}