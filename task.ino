#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* serverName = "http://your_local_server_address:port/api/sensor_data";

int sensorPin = A0; // Pin analog untuk sensor

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    int sensorValue = analogRead(sensorPin);
    float temperature = sensorValue * (5.0 / 1023.0) * 100.0; // Contoh konversi sensor suhu

    String postData = "{\"temperature\": " + String(temperature, 2) + "}";

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }

  delay(60000); // Tunggu 60 detik sebelum membaca lagi
}
