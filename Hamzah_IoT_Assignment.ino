#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int moisturePin = 34  ; // Analog pin for moisture sensor
const int ldrPin = 33;         // Pin connected to the LDR analog output (AO)
const int growLightPin = 18;   // Pin connected to the growlight
const int oneWireBus = 19;     //Pin connected to temperature sensor
const int relayPin = 17;    // Digital pin for relay
const int thresholdValue = 800; // Threshold value for moisture level, adjust as needed
const int threshold = 1000;     // Adjust this value based on your light intensity requirement

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);

  pinMode(moisturePin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(growLightPin, OUTPUT);
  sensors.begin();
  lcd.init(); // initialize LCD
  lcd.backlight(); // turn on LCD backlight 

}

void loop() {
  int moistureValue = analogRead(moisturePin);
  Serial.print("Moisture Level: ");
  Serial.println(moistureValue);

  if (moistureValue < thresholdValue) {
    // Soil moisture is low, turn on water pump
    digitalWrite(relayPin, HIGH);
    Serial.println("Soil moisture is low. Water pump activated.");
  } else {
    // Soil moisture is sufficient, turn off water pump
    digitalWrite(relayPin, LOW);
    Serial.println("Soil moisture is sufficient. Water pump deactivated.");
  }

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  
  int temperatureLCD = sensors.getTempCByIndex(0);
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.print(temperatureLCD);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Moisture:");
  lcd.print(moistureValue);
  delay(1000); // Adjust delay time as needed
   
  int lightLevel = analogRead(ldrPin);
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

    // Check if the light level is below the threshold
  if (lightLevel < threshold) {
    // Turn on the grow light
    digitalWrite(growLightPin, HIGH);
  } else {
    // Turn off the grow light
    digitalWrite(growLightPin, LOW);
  }
}

