/**
 * ESP32 WiFi Advanced Control - Using Common Libraries
 *
 * This example demonstrates using the common protocol parser and pin validator
 * for a more robust implementation.
 *
 * Board: ESP32
 * Connection: WiFi (TCP Server)
 * Features:
 *  - CommandParser for protocol handling
 *  - PinValidator for safe pin operations
 *  - Connection status monitoring
 *  - Client management
 */

#include <WiFi.h>

// Include common libraries (adjust path if needed)
#include "../common/protocol/CommandParser.h"
#include "../common/utils/PinValidator.h"

// ========== CONFIGURATION ==========
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const int SERVER_PORT = 80;

#define DEBUG_SERIAL
#define BAUD_RATE 115200
#define LED_PIN 2

// Connection timeout
#define CLIENT_TIMEOUT 10000  // 10 seconds
#define HEARTBEAT_INTERVAL 5000  // 5 seconds

// ========== GLOBAL VARIABLES ==========
WiFiServer server(SERVER_PORT);
WiFiClient client;
String commandBuffer = "";
unsigned long lastClientActivity = 0;
unsigned long lastHeartbeat = 0;

// ========== SETUP ==========
void setup() {
  Serial.begin(BAUD_RATE);
  delay(100);

  printBanner();

  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Connect to WiFi
  connectToWiFi();

  // Start server
  server.begin();
  Serial.println("✅ TCP Server started on port " + String(SERVER_PORT));
  Serial.println("Waiting for connections...\n");
}

// ========== MAIN LOOP ==========
void loop() {
  // Handle client connection
  handleClient();

  // Process incoming commands
  if (client && client.connected() && client.available()) {
    lastClientActivity = millis();

    char c = client.read();

    if (c == '\n') {
      processCommand(commandBuffer);
      commandBuffer = "";
    } else if (c != '\r') {
      commandBuffer += c;
    }
  }

  // Check client timeout
  if (client && client.connected()) {
    if (millis() - lastClientActivity > CLIENT_TIMEOUT) {
      Serial.println("⚠️ Client timeout - disconnecting");
      client.stop();
    }
  }

  // Heartbeat
  if (millis() - lastHeartbeat > HEARTBEAT_INTERVAL) {
    lastHeartbeat = millis();
    if (client && client.connected()) {
      Serial.println("💓 Client connected: " + client.remoteIP().toString());
    }
  }

  delay(10);
}

// ========== CLIENT MANAGEMENT ==========
void handleClient() {
  if (!client || !client.connected()) {
    WiFiClient newClient = server.available();

    if (newClient) {
      // Disconnect old client if exists
      if (client) {
        client.stop();
      }

      client = newClient;
      lastClientActivity = millis();
      commandBuffer = "";

      Serial.println("\n✅ New client connected!");
      Serial.println("   IP: " + client.remoteIP().toString());
      Serial.println("   Port: " + String(client.remotePort()));

      // Send welcome message
      client.println("STATUS:Connected to " + PinValidator::getBoardName());
      client.println("STATUS:Ready");
    }
  }
}

// ========== COMMAND PROCESSING ==========
void processCommand(String cmdString) {
  cmdString.trim();

  if (cmdString.length() == 0) {
    return;
  }

  #ifdef DEBUG_SERIAL
  Serial.print("📩 Command: ");
  Serial.println(cmdString);
  #endif

  // Parse command using common library
  Command cmd = CommandParser::parse(cmdString);

  if (!cmd.valid) {
    Serial.println("❌ Invalid command format");
    sendResponse("ERROR:Invalid command format");
    return;
  }

  // Validate pin before executing
  bool pinValid = false;

  if (cmd.type == CMD_DIGITAL) {
    pinValid = PinValidator::isValidDigitalPin(cmd.pin);
    if (!pinValid) {
      Serial.println("❌ Invalid digital pin: " + String(cmd.pin));
      sendResponse("ERROR:Invalid digital pin");
      return;
    }
    CommandParser::executeDigital(cmd);
    sendResponse("OK");
  }

  else if (cmd.type == CMD_ANALOG) {
    pinValid = PinValidator::isPWMPin(cmd.pin);
    if (!pinValid) {
      Serial.println("❌ Pin " + String(cmd.pin) + " doesn't support PWM");
      sendResponse("ERROR:Pin doesn't support PWM");
      return;
    }
    CommandParser::executeAnalog(cmd);
    sendResponse("OK");
  }

  else if (cmd.type == CMD_READ) {
    handleReadCommand(cmd);
  }

  else {
    Serial.println("❓ Unknown command type");
    sendResponse("ERROR:Unknown command type");
  }
}

// ========== READ COMMAND HANDLER (Future) ==========
void handleReadCommand(const Command& cmd) {
  if (!PinValidator::isValidAnalogPin(cmd.pin)) {
    Serial.println("❌ Invalid analog read pin: " + String(cmd.pin));
    sendResponse("ERROR:Invalid analog pin");
    return;
  }

  pinMode(cmd.pin, INPUT);
  int value = analogRead(cmd.pin);

  Serial.print("📖 Read Pin ");
  Serial.print(cmd.pin);
  Serial.print(" → ");
  Serial.println(value);

  sendResponse("VALUE:" + String(cmd.pin) + ":" + String(value));
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

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ WiFi connected!");
    Serial.println("   SSID: " + String(WIFI_SSID));
    Serial.print("   IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("   Signal: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.print("   MAC: ");
    Serial.println(WiFi.macAddress());
  } else {
    Serial.println("❌ WiFi connection failed!");
    Serial.println("   Check SSID and password");
  }

  Serial.println();
}

// ========== RESPONSE HELPER ==========
void sendResponse(String response) {
  if (client && client.connected()) {
    client.println(response);
  }

  #ifdef DEBUG_SERIAL
  Serial.println("📤 Response: " + response);
  #endif
}

// ========== BANNER ==========
void printBanner() {
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   ESP32 WiFi Advanced Control         ║");
  Serial.println("║   iot.m.s - IoT Mobile System          ║");
  Serial.println("╚════════════════════════════════════════╝");
  Serial.println();
  Serial.println("Board: " + PinValidator::getBoardName());
  Serial.println("Connection: WiFi TCP Server");
  Serial.println("Features: CommandParser, PinValidator");
  Serial.println();
}
