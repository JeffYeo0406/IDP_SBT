/*
  ESP32 Moisture Sensor Example (YL-69 or similar analog output)

  This code demonstrates how to connect and read data from an analog moisture sensor
  (like the YL-69) using an ESP32 microcontroller with the Arduino framework.

  The YL-69 sensor typically comes with a comparator module (like the KY-53).
  This example reads the ANALOG output (AO) from the comparator module.
  The DIGITAL output (DO) provides a simple threshold detection, which is not used here.

  Connections:
  - VCC: Connect to 3.3V or 5V (ESP32 GPIOs are 3.3V tolerant, but the sensor might need 5V for full range - check sensor specs).
  - GND: Connect to GND
  - AO (Analog Output): Connect to an ADC-capable GPIO pin on the ESP32 (e.g., GPIO 34, 35, 36, 39 - these are ADC1 pins, generally safe to use).
    *Note: ESP32 ADC pins have limitations (e.g., voltage range, non-linearity).
           Refer to the ESP32 datasheet for details on ADC usage.
*/

// Define the pin connected to the sensor's Analog Output (AO)
// Use an ADC-capable pin like GPIO 34, 35, 36, 39
const int moisturePin = 34; // Example: Using GPIO 34

// Define variables to store the raw sensor value and a percentage
int rawValue = 0;
int moisturePercentage = 0;

// Define calibration values (these may need adjustment for your specific sensor and soil)
// These represent the expected raw sensor values for dry and wet conditions.
// Read values from your sensor in dry and wet soil to calibrate.
const int dryValue = 4095; // Approx. raw reading when sensor is completely dry (max value for 12-bit ADC)
const int wetValue = 0;   // Approx. raw reading when sensor is in fully saturated water (min value)

void setup() {
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  // No pinMode needed for analogRead on ESP32
}

void loop() {
  // --- Read the analog value from the sensor ---
  // analogRead() returns a value between 0 and 4095 for the ESP32's 12-bit ADC.
  // A higher raw value typically means drier soil (less conductivity).
  // A lower raw value typically means wetter soil (more conductivity).
  rawValue = analogRead(moisturePin);

  // --- Convert the raw value to a percentage ---
  // Use the map() function to scale the raw value from the sensor's range
  // (dryValue to wetValue) to a percentage range (0 to 100).
  // Note that the dryValue is usually higher than the wetValue for this type of sensor.
  moisturePercentage = map(rawValue, dryValue, wetValue, 0, 100);

  // --- Constrain the percentage to be within 0 and 100 ---
  // The map function can sometimes produce values outside the range due to noise
  // or readings slightly outside the calibrated dry/wet points.
  moisturePercentage = constrain(moisturePercentage, 0, 100);

  // --- Print the results to the Serial Monitor ---
  Serial.print("Raw Value: ");
  Serial.print(rawValue);
  Serial.print(" | Moisture: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  // Wait a bit before taking the next reading
  delay(500); // Delay for 500 milliseconds
}
