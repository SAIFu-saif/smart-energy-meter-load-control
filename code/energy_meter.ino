
#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const int currentPin = A0;     // ACS712 output
const int voltagePin = A1;     // Voltage divider output
const int relayPin   = 8;      // Relay control
const float currentFactor = 0.185; // ACS712 5A sensitivity (V/A)
const float voltageFactor = 230.0 / 1023.0; // for 0-230V mapped to 0-5V via voltage divider

float current = 0.0;
float voltage = 0.0;
float power = 0.0;

float thresholdPower = 300.0; // Max power allowed (in Watts)

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay off initially
  lcd.begin(16, 2);
  lcd.print("Smart Energy Mtr");
  delay(2000);
  lcd.clear();
}

void loop() {
  voltage = analogRead(voltagePin) * voltageFactor;
  float sensorValue = analogRead(currentPin);
  float voltageOut = (sensorValue * 5.0) / 1023.0;
  current = (voltageOut - 2.5) / currentFactor;
  current = abs(current); // Remove negative values
  power = voltage * current;

  // Display on LCD
  lcd.setCursor(0, 0);
  lcd.print("V: ");
  lcd.print(voltage, 1);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("I: ");
  lcd.print(current, 2);
  lcd.print("A ");

  // Control Load
  if (power > thresholdPower) {
    digitalWrite(relayPin, LOW);  // Turn OFF Load
  } else {
    digitalWrite(relayPin, HIGH); // Keep Load ON
  }

  delay(1000);
}
