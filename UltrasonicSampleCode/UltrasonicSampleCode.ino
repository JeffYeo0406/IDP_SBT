/*
  ESP32 Ultrasonic Sensor Example (HC-SR04)

  This code demonstrates how to connect and read data from an ultrasonic sensor
  (like the HC-SR04) using an ESP32 microcontroller with the Arduino framework.

  Connections:
  - VCC: Connect to 5V or 3.3V (check your sensor's specifications, ESP32 pins are 3.3V tolerant)
  - GND: Connect to GND
  - Trig Pin: Connect to a digital GPIO pin on the ESP32 (e.g., GPIO 4)
  - Echo Pin: Connect to a digital GPIO pin on the ESP32 (e.g., GPIO 5)
    *Note: The Echo pin of the HC-SR04 outputs 5V. While ESP32 pins are often 5V tolerant,
           it's safer to use a voltage divider (e.g., two resistors, 1k and 2k)
           on the Echo pin to step down the voltage to 3.3V before connecting to the ESP32 GPIO.
           This example code assumes a direct connection, but be aware of this potential issue.
*/

// Define the pins connected to the sensor
const int trigPin = 4; // ESP32 GPIO pin connected to the sensor's Trig pin
const int echoPin = 5; // ESP32 GPIO pin connected to the sensor's Echo pin

// Define variables for duration and distance
long duration; // Variable to store the pulse duration
int distanceCm; // Variable to store the distance in centimeters
int distanceInch; // Variable to store the distance in inches

void setup() {
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  // Define the trigger pin as an output (to send the pulse)
  pinMode(trigPin, OUTPUT);
  // Define the echo pin as an input (to receive the pulse)
  pinMode(echoPin, INPUT);
}

void loop() {
  // --- Send the trigger pulse ---
  // Clear the trigPin by setting it low
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // Short delay

  // Set the trigPin high for 10 microseconds to send the pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); // Set trigPin low again

  // --- Measure the duration of the echo pulse ---
  // The pulseIn() function reads the duration of a pulse on a pin.
  // It waits for the pin to go HIGH, starts timing, waits for the pin to go LOW,
  // and stops timing. It returns the duration in microseconds.
  // We are measuring the duration of the HIGH pulse on the echoPin.
  duration = pulseIn(echoPin, HIGH);

  // --- Calculate the distance ---
  // The speed of sound is approximately 343 meters per second (or 34.3 cm/ms).
  // The pulse travels to the object and back, so the total distance is twice the
  // distance to the object.
  // Duration is in microseconds.
  // Distance (cm) = (Duration (us) / 2) / Speed of Sound (us/cm)
  // Speed of Sound = 343 m/s = 34300 cm/s = 0.0343 cm/us
  // Reciprocal of Speed of Sound = 1 / 0.0343 us/cm ≈ 29.1 us/cm
  // So, Distance (cm) = Duration (us) / 29.1 / 2  OR  Duration (us) / 58.2
  // A common simplification is to divide by 58 for cm and 148 for inches.
  distanceCm = duration / 58;
  distanceInch = duration / 148;

  // --- Print the results to the Serial Monitor ---
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm (");
  Serial.print(distanceInch);
  Serial.println(" inches)");

  // Wait a bit before taking the next reading
  delay(100); // Delay for 100 milliseconds
}
