#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// =========================
// WIFI
// =========================
const char* ssid = "###";
const char* password = "###";

//const char* ssid = "###";
//const char* password = "###";

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
#define LED1_PIN 16
#define LED2_PIN 21
#define LED3_PIN 32
#define LED4_PIN 33

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

// ========================
// Pompka wody
// ========================
#define WATER_RELAY_PIN 26

DHT dht(DHTPIN, DHTTYPE);

// =========================
// Zmienne globalne
// =========================
float currentTemperature = 0;
float currentHumidity = 0;
int klimatyzacja = 0;
int zazbrojenie = 0;
int currentSoil = 0;
int led1 = 1;
int led2 = 1;
int led3 = 1;
int led4 = 1;

int podlewanie = 0;

bool currentMotion = false;
bool currentGas = false;
bool currentLight = false;
bool currentFan = false;
// =========================
// API
// =========================
void losSensores()
{
    String json = "{";

    json += "\"temperatura\": ";
    json += String(currentTemperature);
    json += ",";

    json += "\"wilgotnosc\": ";
    json += String(currentHumidity);
    json += ",";

    json += "\"wilgotnoscGleby\": ";
    json += String(currentSoil);
    json += ",";

    json += "\"ruch\": ";
    json += currentMotion ? "true" : "false";
    json += ",";

    json += "\"gaz\": ";
    json += currentGas ? "true" : "false";
    json += ",";

    json += "\"poziomSwiatla\": ";
    json += currentLight ? "true" : "false";
    json += ",";

    json += "\"wiatrak\": ";
    json += currentFan ? "true" : "false";
    json += ",";

    json += "\"podlewanie\": ";
    json += podlewanie ? "true" : "false";
    json += ",";

    json += "\"alarm\": ";
    json += zazbrojenie ? "true" : "false";
    json += ",";

    json += "\"zapaloneSwiatlo1\": ";
    json += led1 ? "true" : "false";
    json += ",";

    json += "\"zapaloneSwiatlo2\": ";
    json += led2 ? "true" : "false";
    json += ",";

    json += "\"zapaloneSwiatlo3\": ";
    json += led3 ? "true" : "false";
    json += ",";

    json += "\"zapaloneSwiatlo4\": ";
    json += led4 ? "true" : "false";

    json += "}";

    server.send(200, "application/json", json);
}

void handleFan()
{
    klimatyzacja = server.arg("plain") == "true";
    server.send(200, "text/plain", "OK");
}

void handleAlarm()
{
    zazbrojenie = server.arg("plain") == "true";
    server.send(200, "text/plain", "OK");
}

void handleWatering()
{
    podlewanie = server.arg("plain") == "true";
    server.send(200, "text/plain", "OK");
}

void handleLed1()
{
    led1 = server.arg("plain") == "true";
    server.send(200, "text/plain", "OK");
}

void handleLed2()
{
    led2 = server.arg("plain") == "true";
    server.send(200, "text/plain", "OK");
}

void handleLed3()
{
    led3 = server.arg("plain") == "true";
    server.send(200, "text/plain", "OK");
}

void handleLed4()
{
    led4 = server.arg("plain") == "true";
    server.send(200, "text/plain", "OK");
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
  server.on("/odczyt_sensorow", losSensores);
  server.on("/wiatrak", HTTP_POST, handleFan);
  server.on("/alarm", HTTP_POST, handleAlarm);
  server.on("/podlewanie", HTTP_POST, handleWatering);
  server.on("/zapaloneSwiatlo1", HTTP_POST, handleLed1);
  server.on("/zapaloneSwiatlo2", HTTP_POST, handleLed2);
  server.on("/zapaloneSwiatlo3", HTTP_POST, handleLed3);
  server.on("/zapaloneSwiatlo4", HTTP_POST, handleLed4);

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
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
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

  // =====================
  // Pompka wodna
  // =====================
  pinMode(WATER_RELAY_PIN, OUTPUT);
  digitalWrite(WATER_RELAY_PIN, HIGH);
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
  currentMotion = motion == HIGH;
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
  currentGas = gasState == HIGH;
  if (gasState == HIGH) {

    Serial.println("Wykryto szkodliwy gaz!");
    digitalWrite(BUZZER_PIN, HIGH);

  } else {

    Serial.println("Powietrze bezpieczne do oddychania");
    digitalWrite(BUZZER_PIN, LOW);
  }

  // =========================
  // Czujnik światła + LED'y
  // =========================

  int lightState = digitalRead(LIGHT_PIN);
  currentLight = lightState == HIGH;
  Serial.print("Poziom swiatla: ");
  Serial.println(lightState);

  if (lightState == LOW) {

    Serial.println("Jest jasno");
    digitalWrite(LED_PIN, LOW);

  } else {

    Serial.println("Jest ciemno");
    digitalWrite(LED_PIN, HIGH);
  }
  digitalWrite(LED1_PIN, led1);
  digitalWrite(LED2_PIN, led2);
  digitalWrite(LED3_PIN, led3);
  digitalWrite(LED4_PIN, led4);

  // ==========================
  // WENTYLACJA
  // ==============================
  if (temperature > 24 || klimatyzacja == 1) {

      Serial.println("WIATRACZEK ON");

      // relay ON
      digitalWrite(RELAY_PIN, LOW);
      currentFan = true;
  } else {

      Serial.println("WIATRACZEK OFF");

      // relay OFF
      digitalWrite(RELAY_PIN, HIGH);
      currentFan = false;
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
      podlewanie = 1;

  } else {

      Serial.println("Ziemia wilgotna");
      podlewanie = 0;
  }

  // ===================
  // Jest pompa (wodna)
  // ===================
  if (podlewanie == 1) {
    Serial.println("PODLEWANIE ON");
    digitalWrite(WATER_RELAY_PIN, LOW);

  } else {
      Serial.println("PODLEWANIE OFF");
      digitalWrite(WATER_RELAY_PIN, HIGH);
  }
  Serial.println("----------------");
  delay(2000);
}
