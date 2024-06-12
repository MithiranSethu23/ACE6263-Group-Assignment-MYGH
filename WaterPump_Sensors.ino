
#include <OneWire.h>
#include <DallasTemperature.h>

const int moisturePin = 34  ; // Analog pin for moisture sensor
const int oneWireBus = 5;     
const int relayPin = 4;    // Digital pin for relay
const int thresholdValue = 800; // Threshold value for moisture level, adjust as needed

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);

  pinMode(moisturePin, INPUT);
  pinMode(relayPin, OUTPUT);
  sensors.begin();
 
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

  delay(1000); // Adjust delay time as needed
}

