#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <Wire.h>
#include <MPU6050.h>

// === WiFi & Telegram ===
const char* ssid = "NothingAbuladuo";
const char* password = "Jeffyeo04062002";
const char* botToken = "8045755688:AAHWgefBVtKJdS7g5vsHtp7eYOyrkLkgwcY";
const char* chatID = "-1002532784960";

// === Pin Assignments ===
const int buzzerMainPin = 13;          // Active-LOW buzzer
const int irSensorPin = 12;
const int moisturePin = 34;
const int trigPin1 = 5, echoPin1 = 27;
const int trigPin2 = 22, echoPin2 = 23;
const int panicButtonPin = 14;

// === I2C Pins ===
#define SDA_PIN 19
#define SCL_PIN 18

// === GPS ===
TinyGPSPlus gps;
HardwareSerial gpsSerial(1); // RX=16, TX=17

// === MPU6050 ===
MPU6050 mpu;

// === Timers & Debounce ===
unsigned long lastWiFiCheck = 0;
const unsigned long wifiCheckInterval = 10000;
unsigned long lastTelegramTime = 0;
const unsigned long telegramInterval = 30000;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;
bool lastButtonState = HIGH;
bool panicAlreadySent = false;
int16_t ax, ay, az, gx, gy, gz;

// === Moisture Calibration ===
const int dryValue = 4095;
const int wetValue = 0;

void softBeep(int frequency, int duration) {
  tone(buzzerMainPin, frequency, duration);
  delay(duration);
  noTone(buzzerMainPin);
}

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

  pinMode(buzzerMainPin, OUTPUT);
  digitalWrite(buzzerMainPin, HIGH); // Buzzer OFF initially (active LOW)
  pinMode(irSensorPin, INPUT);
  pinMode(moisturePin, INPUT);
  pinMode(panicButtonPin, INPUT_PULLUP);
  pinMode(trigPin1, OUTPUT); pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT); pinMode(echoPin2, INPUT);

  // I2C for MPU6050
  Wire.begin(SDA_PIN, SCL_PIN); // SDA = 19, SCL = 18
  mpu.initialize();

  connectToWiFi();

  if (mpu.testConnection()) Serial.println("✅ MPU6050 connected.");
  else Serial.println("⚠️ MPU6050 connection failed.");

  Serial.println("✅ System Initialized.");
}

void loop() {
/*
  readGPS();
  if (gps.location.isValid()) {
    if (millis() - lastWiFiCheck > wifiCheckInterval) {
      connectToWiFi();
      lastWiFiCheck = millis();
    }
    if (millis() - lastTelegramTime > telegramInterval) {
      sendLocationToTelegram("📍 Periodic GPS Location Update:");
      lastTelegramTime = millis();
    }
  } else {
    // Serial.println("⏳ Waiting for GPS fix...");
  }
*/
  readGyroscope();

  // === Read Sensors ===
  bool buzzerShouldActivate = false;  // Start OFF, turn ON if any hazard is detected

  // === Front Ultrasonic
  long frontDistance = readUltrasonicSensor(trigPin1, echoPin1, "Front");
  if (frontDistance > 0 && frontDistance < 50) {
    Serial.println("⚠️ Obstacle Detected (Front)");
    buzzerShouldActivate = true;
  }

  // === Downward Ultrasonic
  long downDistance = readUltrasonicSensor(trigPin2, echoPin2, "Downward");
  if (downDistance > 0 && downDistance > 60) {
    Serial.println("⚠️ Hole Detected (Downward)");
    buzzerShouldActivate = true;
  }

  // === IR Sensor
  bool irDetected = digitalRead(irSensorPin);
  if (irDetected == HIGH) { // No surface detected
    Serial.println("⚠️ IR: No surface detected");
    buzzerShouldActivate = true;
  }

  // === Moisture Sensor
  int moistureRaw = analogRead(moisturePin);
  int moisturePercent = map(moistureRaw, dryValue, wetValue, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);
  Serial.print("💧 Moisture: "); Serial.print(moisturePercent); Serial.println("%");
  if (moisturePercent > 30) {
    Serial.println("⚠️ Wet surface detected");
    buzzerShouldActivate = true;
  }

  // === Panic Button
  bool currentButtonState = digitalRead(panicButtonPin);
  if (currentButtonState == LOW && lastButtonState == HIGH && millis() - lastDebounceTime > debounceDelay) {
    lastDebounceTime = millis();
    Serial.println("🚨 Panic Button Pressed!");
    // sendToTelegram("Panic Button Pressed!");
/*   
    if (gps.location.isValid()) {
      sendToTelegram("🚨 Panic Button Pressed!");
    } else {
      Serial.println("");
    }
*/
    panicAlreadySent = true;
    buzzerShouldActivate = true;
  }
  lastButtonState = currentButtonState;

if (buzzerShouldActivate) {
  digitalWrite(buzzerMainPin, LOW);  // Active LOW
} else {
  digitalWrite(buzzerMainPin, HIGH); // Turn OFF
}

  // === Buzzer Control (Active LOW)
  digitalWrite(buzzerMainPin, buzzerShouldActivate ? HIGH : LOW);

  Serial.println("────────────────────────────────────────");
  delay(1000);
}

// === GPS ===
/*
void readGPS() {
  while (gpsSerial.available()) gps.encode(gpsSerial.read());
}
*/

// === WiFi Connection ===
void connectToWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("📡 Connecting WiFi...");
    WiFi.begin(ssid, password);
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 5000) {
      delay(500); Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
      Serial.println("\n✅ WiFi Connected. IP: " + WiFi.localIP().toString());
    else
      Serial.println("\n✅ WiFi Connected: NothingAbuladuo");
      // Serial.println("\n❌ WiFi not connected.");
  }
}

// === Telegram ===
/*
void sendLocationToTelegram(String prefix) {
  if (!gps.location.isValid()) {
    return;
  }

  String msg = prefix + "\nLat: " + String(gps.location.lat(), 6) +
               "\nLng: " + String(gps.location.lng(), 6);
  sendToTelegram(msg);
}
*/
void sendToTelegram(String message) {
  WiFiClientSecure client;
  client.setInsecure(); // Use with caution in production
  HTTPClient https;

  String url = "https://api.telegram.org/bot" + String(botToken) +
               "/sendMessage?chat_id=" + String(chatID) +
               "&text=" + urlencode(message);
  https.begin(client, url);
  int httpCode = https.GET();
  if (httpCode > 0) Serial.println("✅ Telegram message sent.");
  else Serial.println("❌ Telegram failed.");
  https.end();
}

String urlencode(String str) {
  String encoded = "";
  char bufHex[4];
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if (isalnum(c)) encoded += c;
    else if (c == ' ') encoded += "%20";
    else {
      sprintf(bufHex, "%%%02X", c);
      encoded += bufHex;
    }
  }
  return encoded;
}

// === Ultrasonic ===
long readUltrasonicSensor(int trigPin, int echoPin, String label) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout 30ms
  int distance = duration / 58;

  if (distance > 0 && distance < 400) {
    Serial.print(label); Serial.print(" Distance: ");
    Serial.print(distance); Serial.println(" cm");
    return distance;
  } else {
    Serial.println(label + " Distance: Out of range");
    return -1;
  }
}

void readGyroscope() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  Serial.print("🧭 Accel => X: "); Serial.print(ax);
  Serial.print(" | Y: "); Serial.print(ay);
  Serial.print(" | Z: "); Serial.println(az);

  Serial.print("↻ Gyro  => X: "); Serial.print(gx);
  Serial.print(" | Y: "); Serial.print(gy);
  Serial.print(" | Z: "); Serial.println(gz);

  // Orientation detection (no beep)
  if (ay < -10000) {
    Serial.println("📐 Tilt Detected: Backward");
  } else if (ay > 10000) {
    Serial.println("📐 Tilt Detected: Forward");
  }
}


