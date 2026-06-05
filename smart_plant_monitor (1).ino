/*
  Smart Plant Monitor
  ===================
  Measures soil moisture using analog sensor and triggers
  LED + Buzzer when soil is dry (needs water).

  Components:
  - Arduino Uno
  - Soil Moisture Sensor (Analog)
  - LED (Digital Pin 13)
  - Buzzer (Digital Pin 12)
  - 220Ω Resistor for LED

  Wiring:
  - Moisture Sensor VCC -> 5V
  - Moisture Sensor GND -> GND
  - Moisture Sensor A0  -> A0
  - LED (+) -> Pin 13 (with 220Ω resistor)
  - LED (-) -> GND
  - Buzzer (+) -> Pin 12
  - Buzzer (-) -> GND
*/

// Pin definitions
const int MOISTURE_PIN = A0;    // Analog pin for moisture sensor
const int LED_PIN = 13;         // Digital pin for LED
const int BUZZER_PIN = 12;      // Digital pin for Buzzer

// Threshold values (adjust based on your sensor)
// Moisture sensor reads 0-1023 (dry=high, wet=low typically)
const int DRY_THRESHOLD = 700;   // Below this = needs water
const int WET_THRESHOLD = 400;   // Above this = well watered

// Timing
unsigned long lastBeep = 0;
const unsigned long BEEP_INTERVAL = 2000; // Beep every 2 seconds when dry

void setup() {
  // Initialize serial monitor
  Serial.begin(9600);
  Serial.println("================================");
  Serial.println("   SMART PLANT MONITOR v1.0");
  Serial.println("================================");
  Serial.println("Monitoring soil moisture...");
  Serial.println();

  // Set pin modes
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOISTURE_PIN, INPUT);

  // Initial state
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Startup blink
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void loop() {
  // Read moisture value (0-1023)
  int moistureValue = analogRead(MOISTURE_PIN);

  // Map to percentage (0% = dry, 100% = wet)
  // Note: Some sensors read high when dry, low when wet
  int moisturePercent = map(moistureValue, 1023, 0, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  // Print readings to Serial Monitor
  Serial.print("Moisture Raw: ");
  Serial.print(moistureValue);
  Serial.print("  |  Percentage: ");
  Serial.print(moisturePercent);
  Serial.print("%  |  Status: ");

  // Determine status and control outputs
  if (moistureValue > DRY_THRESHOLD) {
    // SOIL IS DRY - NEEDS WATER
    Serial.println("DRY - WATER NEEDED!");

    // Turn on LED continuously
    digitalWrite(LED_PIN, HIGH);

    // Beep buzzer periodically
    unsigned long currentTime = millis();
    if (currentTime - lastBeep >= BEEP_INTERVAL) {
      tone(BUZZER_PIN, 1000, 500); // 1kHz for 500ms
      lastBeep = currentTime;
    }

  } else if (moistureValue < WET_THRESHOLD) {
    // SOIL IS WET - GOOD
    Serial.println("WET - PLANT HAPPY");

    // Turn off LED
    digitalWrite(LED_PIN, LOW);

    // Turn off buzzer
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);

  } else {
    // MODERATE - OKAY
    Serial.println("MODERATE - MONITORING");

    // Slow blink LED
    digitalWrite(LED_PIN, (millis() / 1000) % 2);

    // No buzzer
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Delay between readings
  delay(500);
}
