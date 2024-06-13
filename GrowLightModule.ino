const int ldrPin = 34;         // Pin connected to the LDR analog output (AO)
const int growLightPin = 18;   // Pin connected to the transistor base

// Define threshold value for the LDR
const int threshold = 1000;     // Adjust this value based on your light intensity requirement

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate

  // Set the grow light pin as output
  pinMode(growLightPin, OUTPUT);

  // Initialize grow light as off
  digitalWrite(growLightPin, LOW);
}

void loop() {
  // Read the analog value from the LDR
  int lightLevel = analogRead(ldrPin);

  // Print the light level for debugging
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

  // Small delay to stabilize the readings
  delay(500);
}