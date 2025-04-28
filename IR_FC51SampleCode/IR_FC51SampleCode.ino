/*
  ESP32 IR Obstacle Avoidance Sensor Example (FC-51 or similar digital output)

  This code demonstrates how to connect and read data from a digital IR obstacle
  avoidance sensor (like the FC-51) using an ESP32 microcontroller with the Arduino framework.

  The FC-51 sensor typically has a digital output pin (DO) that goes LOW when
  an obstacle is detected within its range, and HIGH when no obstacle is detected.
  (Note: Some modules might have inverted logic, check your sensor's documentation).

  Connections:
  - VCC: Connect to 3.3V or 5V (check your sensor's specifications).
  - GND: Connect to GND.
  - DO (Digital Output): Connect to a digital GPIO pin on the ESP32 (e.g., GPIO 18).
*/

// Define the pin connected to the sensor's Digital Output (DO)
const int irSensorPin = 18; // ESP32 GPIO pin connected to the sensor's DO pin

// Define the state that indicates an obstacle is detected
// For FC-51, this is typically LOW. If your sensor outputs HIGH on detection, change this to HIGH.
const int OBSTACLE_DETECTED_STATE = LOW;

void setup() {
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  // Define the sensor pin as an input
  pinMode(irSensorPin, INPUT);

  Serial.println("IR Obstacle Sensor example started.");
  Serial.println("Move an object in front of the sensor.");
}

void loop() {
  // --- Read the digital value from the sensor ---
  // digitalRead() returns either HIGH or LOW.
  int sensorState = digitalRead(irSensorPin);

  // --- Check if an obstacle is detected ---
  if (sensorState == OBSTACLE_DETECTED_STATE) {
    // If the sensor state matches the defined 'obstacle detected' state
    Serial.println("Obstacle Detected!");
  } else {
    // Otherwise, no obstacle is detected (or it's outside the range)
    Serial.println("Path Clear");
  }

  // Wait a bit before taking the next reading
  delay(100); // Delay for 100 milliseconds to avoid spamming the serial monitor
}
