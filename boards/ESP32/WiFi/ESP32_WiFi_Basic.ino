/**
 * ESP32 WiFi Basic Control
 *
 * This sketch allows control of ESP32 GPIO pins via WiFi TCP connection
 * Compatible with iot.m.s Flutter application
 *
 * Connection: WiFi (TCP Server)
 * Board: ESP32
 * Protocol: D<pin>:<value> for digital, A<pin>:<value> for analog
 */

#include <WiFi.h>

// ========== CONFIGURATION ==========
// Update these with your WiFi credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const int SERVER_PORT = 80;

// Enable debug output
#define DEBUG_SERIAL
#define BAUD_RATE 115200

// Default LED pin
const int LED_PIN = 2;

// ========== GLOBAL VARIABLES ==========
WiFiServer server(SERVER_PORT);
WiFiClient client;
String commandBuffer = "";

// ========== SETUP ==========
void setup() {
  // Initialize Serial for debugging
  Serial.begin(BAUD_RATE);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("ESP32 WiFi Control - iot.m.s");
  Serial.println("========================================");

  // Initialize default LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Connect to WiFi
  connectToWiFi();

  // Start TCP server
  server.begin();
  Serial.println("TCP Server started on port " + String(SERVER_PORT));
  Serial.println("Ready to accept connections!");
  Serial.println("========================================\n");
}

// ========== MAIN LOOP ==========
void loop() {
  // Check for new client connections
  if (!client || !client.connected()) {
    client = server.available();
    if (client) {
      Serial.println("✅ New client connected: " + client.remoteIP().toString());
      commandBuffer = "";
    }
  }

  // Process data from connected client
  if (client && client.connected() && client.available()) {
    char c = client.read();

    if (c == '\n') {
      // Process complete command
      processCommand(commandBuffer);
      commandBuffer = "";
    } else if (c != '\r') {
      // Build command buffer (ignore carriage return)
      commandBuffer += c;
    }
  }

  delay(10);
}

// ========== WIFI CONNECTION ==========
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n❌ WiFi connection failed!");
    Serial.println("Please check SSID and password");
  }
}

// ========== COMMAND PROCESSING ==========
void processCommand(String command) {
  command.trim();

  if (command.length() == 0) {
    return;
  }

  #ifdef DEBUG_SERIAL
  Serial.print("📩 Received: ");
  Serial.println(command);
  #endif

  // Digital command: D<pin>:<value>
  if (command.startsWith("D") || command.startsWith("d")) {
    int colonIndex = command.indexOf(':');
    if (colonIndex > 0) {
      int pin = command.substring(1, colonIndex).toInt();
      int value = command.substring(colonIndex + 1).toInt();

      // Validate pin
      if (isValidPin(pin)) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, value ? HIGH : LOW);

        Serial.print("💡 Digital Pin ");
        Serial.print(pin);
        Serial.print(" → ");
        Serial.println(value ? "HIGH" : "LOW");

        // Send acknowledgment
        if (client && client.connected()) {
          client.println("OK");
        }
      } else {
        Serial.println("❌ Invalid pin: " + String(pin));
        if (client && client.connected()) {
          client.println("ERROR:Invalid pin");
        }
      }
    }
  }

  // Analog command: A<pin>:<value>
  else if (command.startsWith("A") || command.startsWith("a")) {
    int colonIndex = command.indexOf(':');
    if (colonIndex > 0) {
      int pin = command.substring(1, colonIndex).toInt();
      int value = command.substring(colonIndex + 1).toInt();

      // Constrain value to 0-255
      value = constrain(value, 0, 255);

      // Validate pin
      if (isValidPin(pin)) {
        pinMode(pin, OUTPUT);
        analogWrite(pin, value);

        Serial.print("🎚️ Analog Pin ");
        Serial.print(pin);
        Serial.print(" → ");
        Serial.println(value);

        // Send acknowledgment
        if (client && client.connected()) {
          client.println("OK");
        }
      } else {
        Serial.println("❌ Invalid pin: " + String(pin));
        if (client && client.connected()) {
          client.println("ERROR:Invalid pin");
        }
      }
    }
  }

  else {
    Serial.println("❓ Unknown command: " + command);
    if (client && client.connected()) {
      client.println("ERROR:Unknown command");
    }
  }
}

// ========== PIN VALIDATION ==========
bool isValidPin(int pin) {
  // Safe GPIO pins for ESP32
  const int validPins[] = {2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};

  for (int i = 0; i < sizeof(validPins) / sizeof(validPins[0]); i++) {
    if (pin == validPins[i]) {
      return true;
    }
  }

  return false;
}
