void setup() {
  // put your setup code here, to run once:

}/*
  ESP32 Buzzer Control Example

  This code demonstrates how to control a passive or active buzzer
  using a digital GPIO pin on an ESP32 microcontroller with the Arduino framework.

  Connections:
  - Buzzer Positive (+): Connect to a digital GPIO pin on the ESP32 (e.g., GPIO 2).
  - Buzzer Negative (-): Connect to GND.

  Note: Active buzzers typically produce a sound when power is applied.
        Passive buzzers require a varying frequency signal (like from tone())
        to produce different tones. This code uses tone() which works well
        with passive buzzers for varying tones, and can simply turn an active
        buzzer on/off by using tone() with a frequency (on) or noTone() (off).
*/

// Define the pin connected to the buzzer
const int buzzerPin = 2; // ESP32 GPIO pin connected to the buzzer

void setup() {
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  // Define the buzzer pin as an output
  pinMode(buzzerPin, OUTPUT);

  Serial.println("Buzzer control example started.");
}

void loop() {
  // --- Example 1: Play a simple tone (for passive buzzers) ---
  Serial.println("Playing a tone...");
  // tone(pin, frequency, duration_ms)
  // Plays a tone of a specific frequency on the specified pin for a duration.
  // If duration is not specified, the tone continues until noTone() is called.
  tone(buzzerPin, 1000, 500); // Play a 1000 Hz tone for 500 milliseconds
  delay(1000); // Wait for the tone to finish and a bit more

  Serial.println("Stopping tone.");
  noTone(buzzerPin); // Stop the tone
  delay(500); // Short delay

  // --- Example 2: Simple ON/OFF (works for both active and passive buzzers) ---
  Serial.println("Turning buzzer ON...");
  // For active buzzers, any frequency will turn it on. For passive, it plays a tone.
  tone(buzzerPin, 500, 1000); // Play a 500 Hz tone (or just turn ON active buzzer) for 1 second
  delay(1500); // Wait for the sound to finish

  Serial.println("Turning buzzer OFF.");
  noTone(buzzerPin); // Turn the buzzer OFF
  delay(1000); // Wait before repeating

  // --- Example 3: Play a different tone (passive buzzer) ---
  Serial.println("Playing a different tone...");
  tone(buzzerPin, 2000, 750); // Play a 2000 Hz tone for 750 milliseconds
  delay(1250); // Wait for the tone to finish

  Serial.println("Stopping tone.");
  noTone(buzzerPin); // Stop the tone
  delay(500); // Short delay
}


void loop() {
  // put your main code here, to run repeatedly:

}
