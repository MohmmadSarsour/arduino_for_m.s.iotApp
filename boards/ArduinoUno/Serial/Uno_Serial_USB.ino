/**
 * Arduino Uno USB Serial Control
 *
 * This sketch allows control of Arduino Uno pins via USB Serial
 * Compatible with iot.m.s Flutter application (when connected via USB OTG)
 *
 * Connection: USB Serial
 * Board: Arduino Uno
 * Protocol: D<pin>:<value> for digital, A<pin>:<value> for analog/PWM
 */

// ========== CONFIGURATION ==========
#define BAUD_RATE 115200    // Serial baud rate
const int LED_PIN = 13;     // Built-in LED

// ========== GLOBAL VARIABLES ==========
String commandBuffer = "";

// ========== SETUP ==========
void setup() {
  // Initialize Serial
  Serial.begin(BAUD_RATE);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("Arduino Uno USB Serial Control");
  Serial.println("iot.m.s - IoT Mobile System");
  Serial.println("========================================");

  // Initialize built-in LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("✅ Ready to receive commands");
  Serial.println("========================================\n");
}

// ========== MAIN LOOP ==========
void loop() {
  // Process Serial data
  if (Serial.available()) {
    char c = Serial.read();

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

// ========== COMMAND PROCESSING ==========
void processCommand(String command) {
  command.trim();

  if (command.length() == 0) {
    return;
  }

  Serial.print("📩 Received: ");
  Serial.println(command);

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
    Serial.println("ERROR:Unknown command");
  }
}

// ========== DIGITAL COMMAND HANDLER ==========
void handleDigitalCommand(String command) {
  int colonIndex = command.indexOf(':');

  if (colonIndex < 0) {
    Serial.println("❌ Invalid format. Use: D<pin>:<value>");
    Serial.println("ERROR:Invalid format");
    return;
  }

  int pin = command.substring(1, colonIndex).toInt();
  int value = command.substring(colonIndex + 1).toInt();

  // Validate pin (2-13 for Arduino Uno)
  if (pin < 2 || pin > 13) {
    Serial.println("❌ Invalid pin: " + String(pin));
    Serial.println("   Valid digital pins: 2-13");
    Serial.println("ERROR:Invalid pin");
    return;
  }

  // Execute command
  pinMode(pin, OUTPUT);
  digitalWrite(pin, value ? HIGH : LOW);

  Serial.print("💡 Digital Pin ");
  Serial.print(pin);
  Serial.print(" → ");
  Serial.println(value ? "HIGH (ON)" : "LOW (OFF)");
  Serial.println("OK");
}

// ========== ANALOG/PWM COMMAND HANDLER ==========
void handleAnalogCommand(String command) {
  int colonIndex = command.indexOf(':');

  if (colonIndex < 0) {
    Serial.println("❌ Invalid format. Use: A<pin>:<value>");
    Serial.println("ERROR:Invalid format");
    return;
  }

  int pin = command.substring(1, colonIndex).toInt();
  int value = command.substring(colonIndex + 1).toInt();

  // Constrain value to 0-255
  value = constrain(value, 0, 255);

  // Validate PWM pins (3, 5, 6, 9, 10, 11 for Arduino Uno)
  const int pwmPins[] = {3, 5, 6, 9, 10, 11};
  bool isValidPWM = false;

  for (int i = 0; i < 6; i++) {
    if (pin == pwmPins[i]) {
      isValidPWM = true;
      break;
    }
  }

  if (!isValidPWM) {
    Serial.println("❌ Invalid PWM pin: " + String(pin));
    Serial.println("   Valid PWM pins: 3, 5, 6, 9, 10, 11");
    Serial.println("ERROR:Invalid PWM pin");
    return;
  }

  // Execute command
  pinMode(pin, OUTPUT);
  analogWrite(pin, value);

  Serial.print("🎚️ PWM Pin ");
  Serial.print(pin);
  Serial.print(" → ");
  Serial.print(value);
  Serial.print(" (");
  Serial.print(map(value, 0, 255, 0, 100));
  Serial.println("%)");
  Serial.println("OK");
}
