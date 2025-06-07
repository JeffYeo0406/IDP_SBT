#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>

const char* ssid = "internetssid";
const char* password = "possward";

const char* botToken = "8045755688:AAHWgefBVtKJdS7g5vsHtp7eYOyrkLkgwcY"; // Your bot token
const char* chatID = "-1002532784960"; // Your chat ID from @userinfobot

TinyGPSPlus gps;
HardwareSerial gpsSerial(1); // Use Serial1 for GPS

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17

  connectToWiFi();
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    float lat = gps.location.lat();
    float lng = gps.location.lng();

    Serial.println("--- New GPS Data ---");
    Serial.print("Latitude: ");
    Serial.println(lat, 6);
    Serial.print("Longitude: ");
    Serial.println(lng, 6);

    String message = "📍 Location:\nLatitude: " + String(lat, 6) + 
                     "\nLongitude: " + String(lng, 6);

    sendToTelegram(message);
    delay(10000); // Send every 10 seconds
  }
}

void sendToTelegram(String message) {
  WiFiClientSecure client;
  client.setInsecure(); // Skip cert validation

  HTTPClient https;
  String url = "https://api.telegram.org/bot" + String(botToken) +
               "/sendMessage?chat_id=" + String(chatID) +
               "&text=" + urlencode(message);

  https.begin(client, url);
  int httpCode = https.GET();

  if (httpCode > 0) {
    Serial.print("Telegram send success. Code: ");
    Serial.println(httpCode);
  } else {
    Serial.print("Telegram send failed. Code: ");
    Serial.println(httpCode);
  }

  https.end();
}

String urlencode(String str) {
  String encoded = "";
  char c;
  char bufHex[4];
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else if (c == ' ') {
      encoded += "%20";
    } else {
      sprintf(bufHex, "%%%02X", c);
      encoded += bufHex;
    }
  }
  return encoded;
}
