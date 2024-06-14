#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Pengaturan WiFi
const char* ssid = "C42";
const char* password = "0806040200";

// Pengaturan DHT11
#define DHTPIN 4     // Pin yang terhubung ke DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// URL server Flask
const char* serverName = "https://0cd5w308ynnj.share.zrok.io/sensor/data";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menunggu koneksi ke WiFi...");
  }
  dht.begin();
}

void loop() {
  // Membaca kelembapan dan suhu
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  long timestamp = millis(); // Menggunakan waktu sejak ESP8266/ESP32 dihidupkan sebagai timestamp

  // Periksa apakah pembacaan berhasil
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  // Jika koneksi WiFi terputus, coba koneksi ulang
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Menunggu koneksi ke WiFi...");
    }
  }

  // Mengirim data ke server Flask
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    
    // Membuat JSON untuk dikirim
    StaticJsonDocument<200> jsonDocument;
    jsonDocument["temperature"] = temperature;
    jsonDocument["humidity"] = humidity;
    jsonDocument["timestamp"] = timestamp;
    String httpRequestData;
    serializeJson(jsonDocument, httpRequestData);
    
    int httpResponseCode = http.POST(httpRequestData);

    // Cetak kode respons HTTP
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    }
    else {
      Serial.print("Error pada kode: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  delay(2000); // Tunda 2 detik antara pengiriman
}
