#define BLYNK_TEMPLATE_ID "Your_template_id"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_PRINT Serial


#include "EmonLib.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Calibration Constants
const float vCalibration = 42.5;
const float currCalibration = 1.45;

// Blynk and WiFi Credentials
const char auth[] = "Your_auth_token";
const char ssid[] = "Your_wifi_ssid";
const char pass[] = "your_wifi_password";

// EnergyMonitor instance
EnergyMonitor emon;
BlynkTimer timer;

// Energy Variables
float kWh = 0.0;
float cost = 0.0;
const float ratePerkWh = 6.5;
unsigned long lastMillis = millis();

// EEPROM Addresses
const int addrKWh = 12;
const int addrCost = 16;

// Display Page Variable
int displayPage = 0;

// Reset Button
const int resetButtonPin = 4;

// **Relay Module Pins**
const int relay1Pin = 32;  // IN1 -> G32
const int relay2Pin = 33;  // IN2 -> G33

// Function Prototypes
void sendEnergyDataToBlynk();
void readEnergyDataFromEEPROM();
void saveEnergyDataToEEPROM();
void updateOLED();
void changeDisplayPage();
void resetEEPROM();

void setup() {
  Serial.begin(115200);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  // WiFi Connection
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Connecting to WiFi...");
    display.display();
  }
  Blynk.begin(auth, ssid, pass);

  // Initialize EEPROM
  EEPROM.begin(32);

  // Initialize Buttons and Relays
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  digitalWrite(relay1Pin, HIGH);  // Start with relay OFF
  digitalWrite(relay2Pin, HIGH);  // Start with relay OFF

  // Read Stored Data
  readEnergyDataFromEEPROM();

  // Setup Sensors
  emon.voltage(35, vCalibration, 1.7);
  emon.current(34, currCalibration);

  // Set Timers
  timer.setInterval(2000L, sendEnergyDataToBlynk);
  timer.setInterval(2000L, changeDisplayPage);
}

void loop() {
  Blynk.run();
  timer.run();

  if (digitalRead(resetButtonPin) == LOW) {
    delay(200);
    resetEEPROM();
  }
}

// **Blynk Functions to Control Relay**
BLYNK_WRITE(V5) {  
  int relayState = param.asInt();
  digitalWrite(relay1Pin, relayState);
}

BLYNK_WRITE(V6) {  
  int relayState = param.asInt();
  digitalWrite(relay2Pin, relayState);
}

void sendEnergyDataToBlynk() {
  emon.calcVI(20, 2000);
  float Vrms = emon.Vrms;
  float Irms = emon.Irms;
  float apparentPower = emon.apparentPower;

  unsigned long currentMillis = millis();
  kWh += apparentPower * (currentMillis - lastMillis) / 3600000000.0;
  lastMillis = currentMillis;
  cost = kWh * ratePerkWh;
  
  saveEnergyDataToEEPROM();

  Blynk.virtualWrite(V0, Vrms);
  Blynk.virtualWrite(V1, Irms);
  Blynk.virtualWrite(V2, apparentPower);
  Blynk.virtualWrite(V3, kWh);
  Blynk.virtualWrite(V4, cost);

  updateOLED();
}

void readEnergyDataFromEEPROM() {
  EEPROM.get(addrKWh, kWh);
  EEPROM.get(addrCost, cost);

  if (isnan(kWh)) {
    kWh = 0.0;
    saveEnergyDataToEEPROM();
  }
  if (isnan(cost)) {
    cost = 0.0;
    saveEnergyDataToEEPROM();
  }
}

void saveEnergyDataToEEPROM() {
  EEPROM.put(addrKWh, kWh);
  EEPROM.put(addrCost, cost);
  EEPROM.commit();
}

void updateOLED() {
  display.clearDisplay();
  display.setTextSize(1);

  if (displayPage == 0) {
    display.setCursor(0, 0);
    display.print("Voltage: ");
    display.print(emon.Vrms, 1);
    display.println(" V");

    display.print("Current: ");
    display.print(emon.Irms, 2);
    display.println(" A");

    display.print("Power: ");
    display.print(emon.apparentPower, 1);
    display.println(" W");
  } else {
    display.setCursor(0, 0);
    display.print("Energy: ");
    display.print(kWh, 2);
    display.println(" kWh");

    display.print("Cost: $");
    display.print(cost, 2);
  }

  display.display();
}

void changeDisplayPage() {
  displayPage = (displayPage + 1) % 2;
  updateOLED();
}

void resetEEPROM() {
  kWh = 0.0;
  cost = 0.0;
  saveEnergyDataToEEPROM();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("EEPROM Reset");
  display.println("Energy & Cost");
  display.println("Cleared");
  display.display();
  delay(2000);
}