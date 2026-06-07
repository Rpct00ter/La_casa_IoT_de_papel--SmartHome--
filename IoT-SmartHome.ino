#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// =========================
// WIFI
// =========================
//const char* ssid = "###";
//const char* password = "###";

const char* ssid = "###";
const char* password = "###";

// =========================
// HTTP SERVER
// =========================
WebServer server(80);

// =========================
// DHT22
// =========================
#define DHTPIN 4
#define DHTTYPE DHT22

// =========================
// PIR
// =========================
#define PIR_PIN 18

// =========================
// MQ-2
// =========================
#define GAS_PIN 23

// =========================
// Czujnik światła
// =========================
#define LIGHT_PIN 19

// =========================
// LED
// =========================
#define LED_PIN 22

// ========================
// Wentylator
// ========================
#define RELAY_PIN 25

// ========================
// Alarm
// ========================
#define BUZZER_PIN 27

// ========================
// Czujka do wilgotnosci gleby
// ========================
#define SOIL_PIN 34


DHT dht(DHTPIN, DHTTYPE);

// =========================
// Zmienne globalne
// =========================
float currentTemperature = 0;
float currentHumidity = 0;
int klimatyzacja = 0;
int zazbrojenie = 0;
int currentSoil = 0;
// =========================
// API
// =========================
void handleTemperature()
{
  String json = "{";
  json += "\"temperature\": ";
  json += String(currentTemperature);
  json += ",";

  json += "\"humidity\": ";
  json += String(currentHumidity);

  json += "}";

  server.send(200, "application/json", json);
}

void setup() {

  Serial.begin(115200);

  // =========================
  // WIFI
  // =========================
  WiFi.begin(ssid, password);

  Serial.print("Łączenie z WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Połączono z WiFi!");

  Serial.print("IP ESP32: ");
  Serial.println(WiFi.localIP());

  // =========================
  // HTTP endpoint
  // =========================
  server.on("/odczyt_sensorow", handleTemperature);

  server.begin();

  Serial.println("HTTP server started");

  // =========================
  // DHT22
  // =========================
  dht.begin();

  // =========================
  // PIR
  // =========================
  pinMode(PIR_PIN, INPUT_PULLDOWN);

  // =========================
  // MQ-2
  // =========================
  pinMode(GAS_PIN, INPUT);

  // =========================
  // Światło
  // =========================
  pinMode(LIGHT_PIN, INPUT);

  // =========================
  // LED
  // =========================
  pinMode(LED_PIN, OUTPUT);

  Serial.println("System uruchomiony");
  // =======================
  // Wentylator
  // =======================
  pinMode(RELAY_PIN, OUTPUT);

  // =======================
  // Alarm
  // =======================
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {

  // obsługa requestów HTTP
  server.handleClient();

  // =========================
  // DHT22
  // =========================

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {

    currentTemperature = temperature;
    currentHumidity = humidity;

    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Wilgotnosc: ");
    Serial.print(humidity);
    Serial.println(" %");

  } else {

    Serial.println("Blad odczytu DHT22");
  }

  // =========================
  // PIR
  // =========================

  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {

      Serial.println("Wykryto ruch!");

      if (zazbrojenie == 1) {

          Serial.println("ALARM!");

          digitalWrite(BUZZER_PIN, HIGH);

      } else {

          digitalWrite(BUZZER_PIN, LOW);
      }

  } else {

      Serial.println("Brak ruchu");

      digitalWrite(BUZZER_PIN, LOW);
  }

  // =========================
  // MQ-2
  // =========================

  int gasState = digitalRead(GAS_PIN);

  if (gasState == HIGH) {

    Serial.println("Wykryto szkodliwy gaz!");

  } else {

    Serial.println("Powietrze bezpieczne do oddychania");
  }

  // =========================
  // Czujnik światła
  // =========================

  int lightState = digitalRead(LIGHT_PIN);

  Serial.print("Poziom swiatla: ");
  Serial.println(lightState);

  if (lightState == LOW) {

    Serial.println("Jest jasno");
    digitalWrite(LED_PIN, LOW);

  } else {

    Serial.println("Jest ciemno");
    digitalWrite(LED_PIN, HIGH);
  }

  // ==========================
  // WENTYLACJA
  // ==============================
  if (temperature > 24 || klimatyzacja == 1) {

      Serial.println("WIATRACZEK ON");

      // relay ON
      digitalWrite(RELAY_PIN, LOW);

  } else {

      Serial.println("WIATRACZEK OFF");

      // relay OFF
      digitalWrite(RELAY_PIN, HIGH);
  }

  // =========================
  // Czujnik wilgotnosci gleby
  // =========================

  int soilValue = analogRead(SOIL_PIN);

  currentSoil = soilValue;

  Serial.print("Wilgotnosc gleby: ");
  Serial.println(soilValue);

  if (soilValue > 3000) {

      Serial.println("ZIEMIA SUCHA!");

  } else {

      Serial.println("Ziemia wilgotna");
  }

  Serial.println("----------------");
  delay(2000);
}
