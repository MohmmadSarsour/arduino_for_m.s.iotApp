/**
 * iot.m.s Template - Basic Control
 *
 * This is a template sketch for iot.m.s project
 * Copy and modify for your specific board and connection type
 *
 * Board: [YOUR_BOARD] (ESP32, Arduino Uno, Arduino Mega, NodeMCU)
 * Connection: [CONNECTION_TYPE] (Bluetooth, WiFi, Serial)
 * Protocol: D<pin>:<value> for digital, A<pin>:<value> for analog
 */

// ========== INCLUDES ==========
// Add required libraries here
// #include <WiFi.h>           // For ESP32/NodeMCU WiFi
// #include <BluetoothSerial.h> // For ESP32 Bluetooth
// #include <SoftwareSerial.h>  // For Arduino Uno/Mega Bluetooth

// ========== CONFIGURATION ==========
#define BAUD_RATE 115200      // Serial baud rate for debugging
#define LED_PIN 13            // Built-in LED or test pin

// Connection-specific configuration
// const char* WIFI_SSID = "YOUR_SSID";
// const char* WIFI_PASSWORD = "YOUR_PASSWORD";
// const int SERVER_PORT = 80;

// Enable/disable debug output
#define DEBUG_SERIAL

// ========== GLOBAL VARIABLES ==========
String commandBuffer = "";
// Add connection objects here (WiFiServer, BluetoothSerial, etc.)

// ========== SETUP ==========
void setup() {
  // Initialize Serial for debugging
  Serial.begin(BAUD_RATE);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("iot.m.s - IoT Mobile System");
  Serial.println("Board: [YOUR_BOARD]");
  Serial.println("Connection: [CONNECTION_TYPE]");
  Serial.println("========================================");

  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize connection (WiFi, Bluetooth, or Serial)
  initializeConnection();

  Serial.println("✅ Ready to receive commands");
  Serial.println("========================================\n");
}

// ========== MAIN LOOP ==========
void loop() {
  // Check for incoming data
  // Replace 'connection' with your actual connection object
  // Examples:
  // - WiFiClient: if (client.available())
  // - BluetoothSerial: if (SerialBT.available())
  // - Serial: if (Serial.available())

  // if (connection.available()) {
  //   char c = connection.read();
  //
  //   if (c == '\n') {
  //     processCommand(commandBuffer);
  //     commandBuffer = "";
  //   } else if (c != '\r') {
  //     commandBuffer += c;
  //   }
  // }

  delay(10);
}

// ========== CONNECTION INITIALIZATION ==========
void initializeConnection() {
  // TODO: Implement connection setup
  // Examples:

  // WiFi (ESP32/NodeMCU):
  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // while (WiFi.status() != WL_CONNECTED) { delay(500); }
  // server.begin();

  // Bluetooth (ESP32):
  // SerialBT.begin("ESP32_Device");

  // Bluetooth (Arduino with HC-05):
  // BTSerial.begin(9600);

  Serial.println("TODO: Implement connection initialization");
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
    handleDigitalCommand(command);
  }

  // Analog/PWM command: A<pin>:<value>
  else if (command.startsWith("A") || command.startsWith("a")) {
    handleAnalogCommand(command);
  }

  else {
    Serial.println("❓ Unknown command: " + command);
    sendResponse("ERROR:Unknown command");
  }
}

// ========== DIGITAL COMMAND HANDLER ==========
void handleDigitalCommand(String command) {
  int colonIndex = command.indexOf(':');

  if (colonIndex < 0) {
    Serial.println("❌ Invalid format");
    sendResponse("ERROR:Invalid format");
    return;
  }

  int pin = command.substring(1, colonIndex).toInt();
  int value = command.substring(colonIndex + 1).toInt();

  // TODO: Validate pin for your specific board
  if (!isValidDigitalPin(pin)) {
    Serial.println("❌ Invalid pin: " + String(pin));
    sendResponse("ERROR:Invalid pin");
    return;
  }

  // Execute command
  pinMode(pin, OUTPUT);
  digitalWrite(pin, value ? HIGH : LOW);

  Serial.print("💡 Digital Pin ");
  Serial.print(pin);
  Serial.print(" → ");
  Serial.println(value ? "HIGH" : "LOW");

  sendResponse("OK");
}

// ========== ANALOG/PWM COMMAND HANDLER ==========
void handleAnalogCommand(String command) {
  int colonIndex = command.indexOf(':');

  if (colonIndex < 0) {
    Serial.println("❌ Invalid format");
    sendResponse("ERROR:Invalid format");
    return;
  }

  int pin = command.substring(1, colonIndex).toInt();
  int value = command.substring(colonIndex + 1).toInt();

  // Constrain value to 0-255
  value = constrain(value, 0, 255);

  // TODO: Validate PWM pin for your specific board
  if (!isPWMPin(pin)) {
    Serial.println("❌ Invalid PWM pin: " + String(pin));
    sendResponse("ERROR:Invalid PWM pin");
    return;
  }

  // Execute command
  pinMode(pin, OUTPUT);
  analogWrite(pin, value);

  Serial.print("🎚️ PWM Pin ");
  Serial.print(pin);
  Serial.print(" → ");
  Serial.println(value);

  sendResponse("OK");
}

// ========== PIN VALIDATION ==========
bool isValidDigitalPin(int pin) {
  // TODO: Implement pin validation for your board
  // Examples:

  // ESP32:
  // const int validPins[] = {2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};

  // Arduino Uno:
  // return (pin >= 2 && pin <= 13);

  // Arduino Mega:
  // return (pin >= 2 && pin <= 53);

  return true; // Placeholder
}

bool isPWMPin(int pin) {
  // TODO: Implement PWM pin validation for your board
  // Examples:

  // ESP32: Most GPIO pins support PWM
  // return isValidDigitalPin(pin);

  // Arduino Uno: pins 3, 5, 6, 9, 10, 11
  // const int pwmPins[] = {3, 5, 6, 9, 10, 11};

  // Arduino Mega: pins 2-13, 44-46
  // return ((pin >= 2 && pin <= 13) || (pin >= 44 && pin <= 46));

  return true; // Placeholder
}

// ========== RESPONSE HELPER ==========
void sendResponse(String response) {
  // TODO: Send response via your connection
  // Examples:
  // client.println(response);      // WiFi
  // SerialBT.println(response);    // Bluetooth
  // Serial.println(response);      // Serial

  #ifdef DEBUG_SERIAL
  Serial.println("Response: " + response);
  #endif
}
