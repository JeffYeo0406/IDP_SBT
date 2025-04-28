/*
  ESP32 WiFi Connection Setup

  This code demonstrates how to connect your ESP32 to a WiFi network
  using the Arduino framework.

  Before uploading, replace "YOUR_SSID" and "YOUR_PASSWORD" with
  your actual WiFi network name and password.
*/

#include <WiFi.h> // Include the WiFi library for ESP32

// Define your WiFi network credentials
const char* ssid = "YOUR_SSID";       // Your WiFi network SSID (name)
const char* password = "YOUR_PASSWORD"; // Your WiFi network password

void setup() {
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  // Set WiFi mode to Station (STA) - connects to an access point
  WiFi.mode(WiFi_STA);

  Serial.print("Connecting to WiFi network: ");
  Serial.println(ssid);

  // Attempt to connect to the WiFi network
  WiFi.begin(ssid, password);

  // Wait for the WiFi connection to be established
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // Wait 1 second before checking again
    Serial.print("."); // Print a dot to show that it's trying to connect
  }

  // If the loop finishes, the connection is successful
  Serial.println(""); // New line after the dots
  Serial.println("WiFi connected successfully!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP()); // Print the assigned IP address
}

void loop() {
  // The WiFi connection is handled in setup().
  // The loop() can be used for other tasks that require WiFi,
  // such as fetching data from the internet, running a web server, etc.
  // For this basic example, the loop can be empty or perform other non-WiFi tasks.

  // Example: Keep the connection alive (though the ESP32 usually handles this)
  // if (WiFi.status() != WL_CONNECTED) {
  //   Serial.println("WiFi connection lost. Reconnecting...");
  //   WiFi.reconnect(); // Attempt to reconnect
  //   // You might want to add a delay and check status again here
  // }

  // Add your main application logic here
  // For instance, read sensors, update displays, etc.
  delay(1000); // Example delay in the loop
}
