#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define BLYNK_TEMPLATE_ID "TMPL6CsJrfp3d"
#define BLYNK_TEMPLATE_NAME "Houseplant Monitoring System"
#define BLYNK_AUTH_TOKEN "lW5QbsRxDtKgQKqUJKFPbKJv9G9wIm-9"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WiFi credentials
char ssid[] = "Oilville@unifi";
char pass[] = "12345678";

const int moisturePin = 34; // Analog pin for moisture sensor
const int ldrPin = 33; // Pin connected to the LDR analog output (AO)
const int growLightPin = 18; // Pin connected to the grow light
const int oneWireBus = 19; // Pin connected to temperature sensor
const int relayPin = 17; // Digital pin for relay
const int thresholdValue = 800; // Threshold value for moisture level, adjust as needed
const int threshold = 1000; // Adjust this value based on your light intensity requirement

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

  // Connect to WiFi and Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// Blynk button handler for V2
BLYNK_WRITE(V2) {
  int pinValue = param.asInt();
  if (pinValue == 1) {
    digitalWrite(relayPin, HIGH);
    delay(1500);
    Serial.println("Relay activated via Blynk button.");
    Blynk.virtualWrite(V2, 0);
  } else {
    digitalWrite(relayPin, LOW);
    Serial.println("Relay deactivated via Blynk button.");
  }
}

void loop() {
  Blynk.run();

  int moistureValue = analogRead(moisturePin);
  int moisturePercent = map(moistureValue, 0, 2500, 0, 100);
  Serial.print("Moisture Level: ");
  Serial.println(moisturePercent);

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
  lcd.print(moisturePercent);
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

  // Send data to Blynk
  Blynk.virtualWrite(V0, temperatureC);
  Blynk.virtualWrite(V1, moisturePercent);
  
}
