#define BLYNK_TEMPLATE_ID "TMPL6GYyi3Et9"
#define BLYNK_TEMPLATE_NAME "Lamp Asrama"
#define BLYNK_AUTH_TOKEN "Id_CptxrFgXu6KikqEg53ahGHuDZ9zez"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include "DHT.h"

#define DHTPIN D7
#define DHTTYPE DHT11
#define RELAY_PIN 13

char auth[] = BLYNK_AUTH_TOKEN;
int StatusRelay;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();

  Serial.begin(115200);
  WiFi.begin("Asrama Ustad", "yarrahman1977"); // ganti "ssid" dan "password" dengan SSID dan password WiFi Anda

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("connected...yeey :)");
  Blynk.config(auth);

  if(!SPIFFS.begin()){ // inisialisasi SPIFFS
    Serial.println("Failed to mount file system");
    return;
  }

  // membaca data dari file
  File file = SPIFFS.open("/relayStatus.txt", "r");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  StatusRelay = file.parseInt();
  file.close();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, StatusRelay);
}

BLYNK_WRITE(V2){
  StatusRelay = param.asInt();

  // menulis data ke file
  File file = SPIFFS.open("/relayStatus.txt", "w");
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  file.print(StatusRelay);
  file.close();

  digitalWrite(RELAY_PIN, StatusRelay);
}

void loop() {
  Blynk.run();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Blynk.virtualWrite(V0,h);
  Blynk.virtualWrite(V1,t);
}
