# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with Arduino code in this repository.

## Project Overview

This directory contains Arduino/ESP32 firmware code for the **iot.m.s** (IoT Mobile System) project. The firmware receives commands from a Flutter mobile application via Bluetooth or WiFi and controls GPIO pins accordingly.

### Supported Boards
- **ESP32** - WiFi & Bluetooth support
- **Arduino Uno** - Bluetooth (via HC-05/HC-06) & Serial support
- **Arduino Mega** - Bluetooth, Serial & WiFi (via ESP8266 shield) support
- **NodeMCU (ESP8266)** - WiFi support

## Directory Structure

```
my_first_app_toArduino/
├── boards/                          # Board-specific implementations
│   ├── ESP32/
│   │   ├── Bluetooth/              # ESP32 Bluetooth examples
│   │   └── WiFi/                   # ESP32 WiFi examples
│   ├── ArduinoUno/
│   │   ├── Bluetooth/              # Uno with HC-05/HC-06 module
│   │   └── Serial/                 # Uno USB Serial communication
│   ├── ArduinoMega/
│   │   ├── Bluetooth/              # Mega with Bluetooth module
│   │   ├── Serial/                 # Mega USB Serial
│   │   └── WiFi/                   # Mega with ESP8266 shield
│   └── NodeMCU/
│       └── WiFi/                   # NodeMCU WiFi examples
├── common/                          # Shared code across boards
│   ├── protocol/                   # Protocol parsing utilities
│   └── utils/                      # Helper functions
├── examples/                        # Complete working examples
├── templates/                       # Base templates for new projects
└── CLAUDE.md                       # This file
```

## Communication Protocol

### Command Format

All commands are text-based, newline-terminated strings sent from the mobile app:

```
Digital Pin Control:
  Format: D<pin>:<value>\n
  Examples:
    D2:1    → Set digital pin 2 HIGH (ON)
    D2:0    → Set digital pin 2 LOW (OFF)
    D13:1   → Turn on built-in LED (pin 13)

Analog/PWM Pin Control:
  Format: A<pin>:<value>\n
  Examples:
    A5:128   → Set pin 5 to PWM value 128 (0-255)
    A9:255   → Set pin 9 to full brightness
    A3:0     → Set pin 3 to minimum value

Future Extensions (not yet implemented):
  Sensor Read: R<pin>\n          → Request reading from pin
  Servo Control: S<pin>:<angle>\n → Set servo angle (0-180)
  Configuration: C<command>\n     → System configuration commands
```

### Response Format (Optional)

Boards can send responses back to the app:

```
Success: OK\n
Error: ERROR:<message>\n
Reading: VALUE:<pin>:<value>\n
Status: STATUS:<message>\n
```

## Code Architecture

### Standard Sketch Structure

Every Arduino sketch should follow this pattern:

```cpp
// 1. INCLUDES
#include <ConnectionLibrary.h>  // WiFi, Bluetooth, or Serial

// 2. CONFIGURATION
#define LED_PIN 2
#define BAUD_RATE 115200

// 3. GLOBAL VARIABLES
ConnectionType connection;
String commandBuffer = "";

// 4. SETUP
void setup() {
  // Initialize serial for debugging
  Serial.begin(BAUD_RATE);

  // Initialize connection (Bluetooth/WiFi/Serial)
  initializeConnection();

  // Configure pins
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

// 5. MAIN LOOP
void loop() {
  // Check for incoming data
  if (connection.available()) {
    char c = connection.read();

    if (c == '\n') {
      processCommand(commandBuffer);
      commandBuffer = "";
    } else {
      commandBuffer += c;
    }
  }

  // Optional: Add sensor reading, heartbeat, etc.
  delay(10);
}

// 6. COMMAND PROCESSING
void processCommand(String command) {
  command.trim();

  // Digital command: D<pin>:<value>
  if (command.startsWith("D")) {
    int colonIndex = command.indexOf(':');
    if (colonIndex > 0) {
      int pin = command.substring(1, colonIndex).toInt();
      int value = command.substring(colonIndex + 1).toInt();

      pinMode(pin, OUTPUT);
      digitalWrite(pin, value ? HIGH : LOW);

      Serial.println("Digital pin " + String(pin) + " set to " + String(value));
    }
  }

  // Analog command: A<pin>:<value>
  else if (command.startsWith("A")) {
    int colonIndex = command.indexOf(':');
    if (colonIndex > 0) {
      int pin = command.substring(1, colonIndex).toInt();
      int value = command.substring(colonIndex + 1).toInt();

      pinMode(pin, OUTPUT);
      analogWrite(pin, constrain(value, 0, 255));

      Serial.println("Analog pin " + String(pin) + " set to " + String(value));
    }
  }

  else {
    Serial.println("Unknown command: " + command);
  }
}
```

### Connection-Specific Setup

#### ESP32 Bluetooth
```cpp
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void initializeConnection() {
  if (!SerialBT.begin("ESP32_LED", true)) {
    Serial.println("Bluetooth initialization failed!");
    while(1);
  }
  SerialBT.setPin("1234", 4);
  Serial.println("Bluetooth ready");
}

// In loop: SerialBT.available(), SerialBT.read()
```

#### ESP32 WiFi (TCP Server)
```cpp
#include <WiFi.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
WiFiServer server(80);
WiFiClient client;

void initializeConnection() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

// In loop: client = server.available(), client.available(), client.read()
```

#### Arduino Uno Bluetooth (HC-05/HC-06)
```cpp
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX, TX

void initializeConnection() {
  BTSerial.begin(9600);  // HC-05/HC-06 default baud rate
  Serial.println("Bluetooth ready (HC-05)");
}

// In loop: BTSerial.available(), BTSerial.read()
```

#### Arduino Uno Serial (USB)
```cpp
void initializeConnection() {
  Serial.begin(115200);
  Serial.println("Serial ready");
}

// In loop: Serial.available(), Serial.read()
```

## Pin Configuration Guidelines

### ESP32 Pin Mapping
```cpp
// Safe GPIO pins for general use:
// Digital: 2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33
// Analog (ADC): 32, 33, 34, 35, 36, 39
// PWM: Most GPIO pins support PWM

// Avoid these pins:
// GPIO 0, 6-11 (boot/flash), GPIO 1, 3 (UART)
```

### Arduino Uno Pin Mapping
```cpp
// Digital: 2-13 (pin 13 has built-in LED)
// PWM: 3, 5, 6, 9, 10, 11 (marked with ~)
// Analog Input: A0-A5 (can also be used as digital 14-19)
```

### Arduino Mega Pin Mapping
```cpp
// Digital: 2-53
// PWM: 2-13, 44-46
// Analog Input: A0-A15
```

## Development Workflow

### Testing a New Board

1. Open the appropriate sketch in Arduino IDE or PlatformIO
2. Select the correct board and port from Tools menu
3. Upload the sketch
4. Open Serial Monitor (115200 baud) to see debug output
5. Test commands manually via Serial Monitor:
   ```
   D13:1    (turn on built-in LED)
   D13:0    (turn off)
   ```
6. Connect mobile app and test via Bluetooth/WiFi

### Creating a New Board Configuration

1. Choose the appropriate folder: `boards/<BoardType>/<ConnectionType>/`
2. Copy the closest template sketch
3. Modify pin definitions for your board
4. Update connection initialization code
5. Test thoroughly with Serial Monitor first
6. Document any special requirements in comments

### Adding New Features

When adding features (sensors, servos, etc.):
1. Define new command prefixes in the protocol (e.g., 'S' for servo)
2. Update the `processCommand()` function
3. Document the new commands at the top of the sketch
4. Update mobile app to send new command formats

## Library Dependencies

### ESP32 Boards
```
BluetoothSerial (built-in)
WiFi (built-in)
```

### Arduino Uno/Mega with Bluetooth Module
```
SoftwareSerial (built-in)
```

### Arduino with WiFi Shield
```
WiFi (built-in for boards with WiFi)
ESP8266WiFi (for ESP8266-based shields)
```

### Optional Libraries (for advanced features)
```
Servo - For servo motor control
DHT - For temperature/humidity sensors
Adafruit_NeoPixel - For LED strips
AccelStepper - For stepper motors
```

## Common Issues and Solutions

### Issue: Bluetooth won't pair
**ESP32**: Ensure `SerialBT.begin()` includes device name. Check if Bluetooth is enabled in menuconfig.
**HC-05/HC-06**: Verify RX/TX connections, check baud rate (default 9600), ensure module has power.

### Issue: WiFi connection fails
**ESP32/NodeMCU**: Verify SSID and password are correct. Check 2.4GHz band (5GHz not supported). Ensure router allows new devices.

### Issue: Commands not working
- Check newline termination (`\n`) in commands
- Verify baud rates match between Serial Monitor and code
- Enable Serial debug output to see received commands
- Check pin numbers are valid for your board

### Issue: Pin not responding
- Verify pin supports the operation (digital/PWM)
- Check pin isn't used by system (flash, UART, etc.)
- Ensure pinMode() is called before digitalWrite/analogWrite

## Code Generation Guidelines

When generating Arduino code based on Flutter app configuration:

1. **Extract pin configuration** from Flutter `PinConfig` models
2. **Determine board type** from selected board in app
3. **Select connection type** (Bluetooth/WiFi) based on user choice
4. **Generate setup code** with appropriate pins configured as INPUT/OUTPUT
5. **Include all commands** used by the playground (D and A commands for configured pins)
6. **Add validation** to prevent invalid pin access
7. **Include debugging** via Serial.println for troubleshooting

### Example Auto-Generated Code Structure
```cpp
// Auto-generated by iot.m.s Flutter App
// Board: ESP32
// Connection: Bluetooth
// Generated: 2025-11-21

// User configured pins:
// - Pin 2 (Digital): LED
// - Pin 5 (Analog): Brightness Control

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
const int PIN_LED = 2;
const int PIN_BRIGHTNESS = 5;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BRIGHTNESS, OUTPUT);

  // ... Bluetooth initialization
}

// ... rest of code
```
