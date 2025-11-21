# Arduino Code Library - iot.m.s

Ready-to-use Arduino/ESP32 firmware for the **iot.m.s** (IoT Mobile System) platform. Upload these files to your microcontroller boards to enable remote control from the mobile app—no coding required.

## Quick Start

1. **Choose your board and connection type**
2. **Navigate to the appropriate folder** under `boards/`
3. **Open the `.ino` file** in Arduino IDE or PlatformIO
4. **Configure settings** (WiFi credentials, Bluetooth name, etc.)
5. **Upload to your board**
6. **Connect from the Flutter mobile app**

## Directory Structure

```
my_first_app_toArduino/
├── boards/                    # Board-specific implementations
│   ├── ESP32/
│   │   ├── Bluetooth/        # ESP32_Bluetooth_Basic.ino
│   │   └── WiFi/             # ESP32_WiFi_Basic.ino
│   ├── ArduinoUno/
│   │   ├── Bluetooth/        # Uno_Bluetooth_HC05.ino
│   │   └── Serial/           # Uno_Serial_USB.ino
│   ├── ArduinoMega/
│   │   ├── Bluetooth/
│   │   ├── Serial/
│   │   └── WiFi/
│   └── NodeMCU/
│       └── WiFi/
├── common/                    # Shared utilities
│   ├── protocol/             # CommandParser.h
│   └── utils/                # PinValidator.h
├── templates/                 # Template_Basic.ino
├── examples/                  # Complete working examples
├── CLAUDE.md                  # Developer guide
└── README.md                  # This file
```

## Supported Boards

| Board | Bluetooth | WiFi | Serial |
|-------|-----------|------|--------|
| **ESP32** | ✅ Built-in | ✅ Built-in | ✅ USB |
| **Arduino Uno** | ✅ HC-05/HC-06 | ❌ | ✅ USB |
| **Arduino Mega** | ✅ HC-05/HC-06 | ⚠️ ESP8266 Shield | ✅ USB |
| **NodeMCU (ESP8266)** | ❌ | ✅ Built-in | ✅ USB |

## Communication Protocol

All commands are text-based, newline-terminated (`\n`):

### Digital Control
```
D<pin>:<value>
```
- `D2:1` - Set digital pin 2 HIGH (ON)
- `D2:0` - Set digital pin 2 LOW (OFF)
- `D13:1` - Turn on built-in LED

### Analog/PWM Control
```
A<pin>:<value>
```
- `A5:128` - Set pin 5 to PWM value 128 (0-255 range)
- `A9:255` - Set pin 9 to maximum brightness
- `A3:0` - Set pin 3 to minimum value

### Responses (from board to app)
```
OK                      # Command executed successfully
ERROR:<message>         # Command failed
VALUE:<pin>:<value>     # Sensor reading (future)
STATUS:<message>        # Status update (future)
```

## Getting Started by Board

### ESP32 + WiFi

**File**: `boards/ESP32/WiFi/ESP32_WiFi_Basic.ino`

**Setup**:
1. Install ESP32 board support in Arduino IDE
2. Open the sketch
3. Update WiFi credentials:
   ```cpp
   const char* WIFI_SSID = "YOUR_WIFI_SSID";
   const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
   ```
4. Select board: "ESP32 Dev Module"
5. Upload
6. Open Serial Monitor to see IP address
7. Connect mobile app to the IP address shown

**Pin Configuration**:
- Safe GPIO pins: 2, 4, 5, 12-19, 21-27, 32-33
- Avoid: 0, 1, 3, 6-11 (boot/flash)

---

### ESP32 + Bluetooth

**File**: `boards/ESP32/Bluetooth/ESP32_Bluetooth_Basic.ino`

**Setup**:
1. Install ESP32 board support
2. Open the sketch
3. Device will appear as "ESP32_LED" with PIN "1234"
4. Upload
5. Pair from mobile device
6. Connect via app Bluetooth page

---

### Arduino Uno + Bluetooth (HC-05/HC-06)

**File**: `boards/ArduinoUno/Bluetooth/Uno_Bluetooth_HC05.ino`

**Wiring**:
```
HC-05/HC-06 VCC → Arduino 5V
HC-05/HC-06 GND → Arduino GND
HC-05/HC-06 TXD → Arduino Pin 10
HC-05/HC-06 RXD → Arduino Pin 11 (via voltage divider)
```

**Voltage Divider** (5V to 3.3V for HC-05 RX):
```
Arduino Pin 11 → 1K resistor → HC-05 RXD
                              → 2K resistor → GND
```

**Setup**:
1. Wire HC-05/HC-06 module
2. Upload sketch
3. Pair Bluetooth module from phone settings
4. Connect via app Bluetooth page

**Note**: HC-05 default baud rate is 9600. Some modules may require AT commands to configure.

---

### Arduino Uno + USB Serial

**File**: `boards/ArduinoUno/Serial/Uno_Serial_USB.ino`

**Setup**:
1. Upload sketch
2. Connect Arduino to Android device via USB OTG cable
3. Use Serial communication in app

**Testing**:
Open Serial Monitor in Arduino IDE and type:
```
D13:1   (turn on LED)
D13:0   (turn off LED)
```

## Using Common Libraries

### CommandParser (Recommended)

Located in `common/protocol/CommandParser.h`, this header provides standardized command parsing:

```cpp
#include "common/protocol/CommandParser.h"

void processCommand(String cmd) {
  Command parsedCmd = CommandParser::parse(cmd);

  if (parsedCmd.valid) {
    if (parsedCmd.type == CMD_DIGITAL) {
      CommandParser::executeDigital(parsedCmd);
    } else if (parsedCmd.type == CMD_ANALOG) {
      CommandParser::executeAnalog(parsedCmd);
    }
  }
}
```

### PinValidator (Recommended)

Located in `common/utils/PinValidator.h`, validates pins for different boards:

```cpp
#include "common/utils/PinValidator.h"

if (PinValidator::isValidDigitalPin(pin)) {
  // Pin is valid for this board
}

if (PinValidator::isPWMPin(pin)) {
  // Pin supports PWM on this board
}

Serial.println(PinValidator::getBoardName());
```

## Creating a New Configuration

1. **Copy a template**:
   ```bash
   cp templates/Template_Basic.ino boards/YourBoard/YourConnection/YourSketch.ino
   ```

2. **Modify configuration section**:
   - Update includes for your board
   - Set pin definitions
   - Configure connection (WiFi SSID, Bluetooth name, etc.)

3. **Implement connection setup**:
   - `initializeConnection()` function
   - Update `loop()` to read from your connection

4. **Implement pin validation**:
   - `isValidDigitalPin()`
   - `isPWMPin()`

5. **Test**:
   - Upload to board
   - Open Serial Monitor (115200 baud)
   - Send test commands manually
   - Connect mobile app

## Troubleshooting

### Board not responding to commands
- Check Serial Monitor output for received commands
- Verify baud rate matches between code and Serial Monitor
- Ensure commands end with newline (`\n`)

### WiFi connection fails (ESP32/NodeMCU)
- Verify SSID and password are correct
- Check router is on 2.4GHz (not 5GHz)
- Ensure good signal strength
- Check Serial Monitor for IP address

### Bluetooth pairing fails
- **ESP32**: Check device appears in Bluetooth settings as "ESP32_LED"
- **HC-05**: Verify module is powered and LED is blinking
- **HC-05**: Default PIN is usually "1234" or "0000"
- Check wiring, especially RX/TX connections

### Pin not working
- Verify pin number is valid for your board
- Check pin supports the operation (digital vs PWM)
- Ensure pin is not used by system (Serial, SPI, etc.)
- Check physical wiring

### Commands received but pins don't respond
- Enable `DEBUG_SERIAL` to see parsed commands
- Check `pinMode()` is called before `digitalWrite()`/`analogWrite()`
- Verify voltage levels (3.3V vs 5V logic)

## Pin Reference

### ESP32
- **Digital**: 2, 4, 5, 12-19, 21-27, 32-33
- **ADC**: 32-39 (analog read)
- **PWM**: Most GPIO pins
- **Avoid**: 0, 1, 3, 6-11

### Arduino Uno
- **Digital**: 2-13 (13 has built-in LED)
- **PWM**: 3, 5, 6, 9, 10, 11 (marked with ~)
- **Analog**: A0-A5

### Arduino Mega
- **Digital**: 2-53
- **PWM**: 2-13, 44-46
- **Analog**: A0-A15

### NodeMCU (ESP8266)
- **Digital**: D0-D8 (GPIO 16, 5, 4, 0, 2, 14, 12, 13, 15)
- **PWM**: D0-D8
- **Analog**: A0 (only one ADC)

## Need Help?

- **Troubleshooting**: See sections above for common issues
- **Pin compatibility**: Check pin reference tables for your board
- **Examples**: Browse `examples/` folder for complete working projects
- **Custom features**: Modify the code to add sensors, servos, or other components

## License

This code is part of the iot.m.s project.
