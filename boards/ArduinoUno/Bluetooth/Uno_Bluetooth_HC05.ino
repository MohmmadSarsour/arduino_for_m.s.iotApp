/**
 * Arduino Uno Bluetooth Control (HC-05/HC-06)
 *
 * This sketch allows control of Arduino Uno pins via Bluetooth
 * Compatible with iot.m.s Flutter application
 *
 * Connection: Bluetooth (HC-05 or HC-06 module)
 * Board: Arduino Uno
 * Protocol: D<pin>:<value> for digital, A<pin>:<value> for analog/PWM
 *
 * Wiring:
 * HC-05/HC-06 VCC → 5V
 * HC-05/HC-06 GND → GND
 * HC-05/HC-06 TXD → Arduino Pin 10 (RX)
 * HC-05/HC-06 RXD → Arduino Pin 11 (TX) via voltage divider (5V to 3.3V)
 *
 * Note: HC-05/HC-06 RX is 3.3V tolerant! Use a voltage divider:
 *       Arduino Pin 11 → 1K resistor → HC-05 RXD
 *                                    → 2K resistor → GND
 */

#include <SoftwareSerial.h>

// ========== CONFIGURATION ==========
#define BT_RX_PIN 10        // Connect to HC-05 TX
#define BT_TX_PIN 11        // Connect to HC-05 RX (via voltage divider)
#define BT_BAUD_RATE 9600   // HC-05/HC-06 default baud rate
#define SERIAL_BAUD 115200  // USB Serial for debugging

// Enable debug output
#define DEBUG_SERIAL

// Built-in LED pin
const int LED_PIN = 13;

// ========== GLOBAL VARIABLES ==========
SoftwareSerial BTSerial(BT_RX_PIN, BT_TX_PIN);
String commandBuffer = "";

// ========== SETUP ==========
void setup() {
  // Initialize USB Serial for debugging
  Serial.begin(SERIAL_BAUD);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("Arduino Uno Bluetooth Control - iot.m.s");
  Serial.println("========================================");

  // Initialize Bluetooth Serial
  BTSerial.begin(BT_BAUD_RATE);
  Serial.println("✅ Bluetooth Serial initialized");
  Serial.println("Baud Rate: " + String(BT_BAUD_RATE));
  Serial.println("Waiting for Bluetooth connection...");

  // Initialize built-in LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("========================================\n");
}

// ========== MAIN LOOP ==========
void loop() {
  // Process Bluetooth data
  if (BTSerial.available()) {
    char c = BTSerial.read();

    if (c == '\n') {
      // Process complete command
      processCommand(commandBuffer);
      commandBuffer = "";
    } else if (c != '\r') {
      // Build command buffer (ignore carriage return)
      commandBuffer += c;
    }
  }

  // Optional: Echo USB Serial to Bluetooth for testing
  #ifdef DEBUG_SERIAL
  if (Serial.available()) {
    char c = Serial.read();
    BTSerial.write(c);
  }
  #endif

  delay(10);
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

      // Validate pin (2-13 for Uno)
      if (pin >= 2 && pin <= 13) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, value ? HIGH : LOW);

        Serial.print("💡 Digital Pin ");
        Serial.print(pin);
        Serial.print(" → ");
        Serial.println(value ? "HIGH" : "LOW");

        // Send acknowledgment via Bluetooth
        BTSerial.println("OK");
      } else {
        Serial.println("❌ Invalid pin: " + String(pin));
        BTSerial.println("ERROR:Invalid pin");
      }
    }
  }

  // Analog/PWM command: A<pin>:<value>
  else if (command.startsWith("A") || command.startsWith("a")) {
    int colonIndex = command.indexOf(':');
    if (colonIndex > 0) {
      int pin = command.substring(1, colonIndex).toInt();
      int value = command.substring(colonIndex + 1).toInt();

      // Constrain value to 0-255
      value = constrain(value, 0, 255);

      // Validate PWM pins (3, 5, 6, 9, 10, 11 for Uno)
      if (pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11) {
        pinMode(pin, OUTPUT);
        analogWrite(pin, value);

        Serial.print("🎚️ PWM Pin ");
        Serial.print(pin);
        Serial.print(" → ");
        Serial.println(value);

        // Send acknowledgment
        BTSerial.println("OK");
      } else {
        Serial.println("❌ Invalid PWM pin: " + String(pin));
        Serial.println("   Valid PWM pins: 3, 5, 6, 9, 10, 11");
        BTSerial.println("ERROR:Invalid PWM pin");
      }
    }
  }

  else {
    Serial.println("❓ Unknown command: " + command);
    BTSerial.println("ERROR:Unknown command");
  }
}
