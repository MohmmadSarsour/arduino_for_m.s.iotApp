# Arduino Code - iot.m.s (IoT Mobile System)

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)](https://www.arduino.cc/)
[![ESP32](https://img.shields.io/badge/ESP32-000000?style=for-the-badge&logo=Espressif&logoColor=white)](https://www.espressif.com/)
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=for-the-badge)](LICENSE)

> Control your Arduino and ESP32 boards from a mobile app without writing code!

This repository contains Arduino/ESP32 firmware for the **iot.m.s** (IoT Mobile System) project - a no-code IoT platform that lets you control microcontroller GPIO pins via a Flutter mobile application.

---

## 🎯 Features

- ✅ **Multiple Board Support**: ESP32, Arduino Uno, Arduino Mega, NodeMCU
- ✅ **Multiple Connections**: WiFi, Bluetooth, USB Serial
- ✅ **Simple Protocol**: Text-based commands (D2:1, A5:128)
- ✅ **Plug & Play**: Ready-to-use sketches for common setups
- ✅ **Reusable Libraries**: CommandParser and PinValidator
- ✅ **Well Documented**: Comprehensive guides and examples
- ✅ **Mobile Integration**: Works seamlessly with Flutter app

---

## 📁 Project Structure

```
my_first_app_toArduino/
├── boards/              # Board-specific implementations
│   ├── ESP32/
│   │   ├── Bluetooth/   # ESP32 Bluetooth sketches
│   │   └── WiFi/        # ESP32 WiFi sketches
│   ├── ArduinoUno/
│   │   ├── Bluetooth/   # Uno + HC-05 module
│   │   └── Serial/      # Uno USB Serial
│   ├── ArduinoMega/     # Future expansion
│   └── NodeMCU/         # Future expansion
│
├── common/              # Reusable libraries
│   ├── protocol/        # CommandParser.h
│   └── utils/          # PinValidator.h
│
├── examples/            # Advanced examples
├── templates/           # Starting templates
└── docs/               # Documentation
```

---

## 🚀 Quick Start

### 1. Choose Your Setup

| Board | Connection | Sketch File |
|-------|------------|-------------|
| ESP32 | WiFi | `boards/ESP32/WiFi/ESP32_WiFi_Basic.ino` |
| ESP32 | Bluetooth | `boards/ESP32/Bluetooth/ESP32_Bluetooth_Basic.ino` |
| Arduino Uno | Bluetooth (HC-05) | `boards/ArduinoUno/Bluetooth/Uno_Bluetooth_HC05.ino` |
| Arduino Uno | USB Serial | `boards/ArduinoUno/Serial/Uno_Serial_USB.ino` |

### 2. Install Board Support (ESP32 only)

```
Arduino IDE → File → Preferences
Additional Board Manager URLs:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Tools → Board → Board Manager → Search "ESP32" → Install
```

### 3. Configure & Upload

**For ESP32 WiFi:**
```cpp
const char* WIFI_SSID = "YourWiFiName";
const char* WIFI_PASSWORD = "YourPassword";
```

**For ESP32 Bluetooth:**
- Device Name: "ESP32_LED"
- PIN: "1234"
- No configuration needed!

**Upload Steps:**
1. Open sketch in Arduino IDE
2. Select board and port
3. Click Upload
4. Open Serial Monitor (115200 baud)
5. Note the IP address (WiFi) or pair the device (Bluetooth)

### 4. Connect from Mobile App

1. Download iot.m.s Flutter app
2. Select your board type
3. Choose connection method (WiFi/Bluetooth)
4. Enter IP address or select paired device
5. Start controlling!

---

## 📡 Communication Protocol

All commands are text-based and newline-terminated (`\n`):

### Digital Control
```
D<pin>:<value>
```
**Examples:**
- `D2:1` → Turn ON pin 2
- `D2:0` → Turn OFF pin 2
- `D13:1` → Turn ON built-in LED

### Analog/PWM Control
```
A<pin>:<value>
```
**Examples:**
- `A5:128` → Set pin 5 to 50% PWM (128/255)
- `A9:255` → Set pin 9 to 100% brightness
- `A3:0` → Turn off pin 3

### Responses (from board)
```
OK                    # Success
ERROR:<message>       # Error
VALUE:<pin>:<value>   # Sensor reading (future)
STATUS:<message>      # Status update (future)
```

---

## 🔌 Supported Boards

### ESP32
```
✅ Built-in WiFi
✅ Built-in Bluetooth
📍 Digital pins: 2, 4, 5, 12-19, 21-27, 32-33
📍 PWM: Most GPIO pins
📍 ADC: 32-39
⚡ Voltage: 3.3V
```

### Arduino Uno
```
⚠️ WiFi: Not supported (requires shield)
✅ Bluetooth: Via HC-05/HC-06 module
✅ USB Serial: Built-in
📍 Digital pins: 2-13
📍 PWM: 3, 5, 6, 9, 10, 11
📍 Analog: A0-A5
⚡ Voltage: 5V
```

### Arduino Mega (Coming Soon)
```
⚠️ WiFi: Via ESP8266 shield
✅ Bluetooth: Via HC-05/HC-06 module
✅ USB Serial: Built-in
📍 Digital pins: 2-53
📍 PWM: 2-13, 44-46
📍 Analog: A0-A15
⚡ Voltage: 5V
```

### NodeMCU ESP8266 (Coming Soon)
```
✅ Built-in WiFi
❌ Bluetooth: Not supported
📍 Digital pins: D0-D8
📍 PWM: D0-D8
📍 ADC: A0 (single channel)
⚡ Voltage: 3.3V
```

---

## 🛠️ Using Common Libraries

### CommandParser

Simplifies command parsing and execution:

```cpp
#include "common/protocol/CommandParser.h"

void processCommand(String cmdString) {
  Command cmd = CommandParser::parse(cmdString);

  if (cmd.valid) {
    if (cmd.type == CMD_DIGITAL) {
      CommandParser::executeDigital(cmd);
    } else if (cmd.type == CMD_ANALOG) {
      CommandParser::executeAnalog(cmd);
    }
  }
}
```

### PinValidator

Board-specific pin validation:

```cpp
#include "common/utils/PinValidator.h"

if (PinValidator::isValidDigitalPin(pin)) {
  // Safe to use
}

if (PinValidator::isPWMPin(pin)) {
  // Supports PWM
}

Serial.println(PinValidator::getBoardName());
// Output: "ESP32" or "Arduino Uno" etc.
```

---

## 📖 Documentation

| Document | Purpose |
|----------|---------|
| [README.md](README.md) | Complete user documentation |
| [QUICK_START.md](QUICK_START.md) | 5-minute setup guide |
| [CLAUDE.md](CLAUDE.md) | Developer & architecture guide |
| [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md) | Project overview |

---

## 💡 Examples

### Basic Digital Control (ESP32 WiFi)

```cpp
#include <WiFi.h>

const char* WIFI_SSID = "MyWiFi";
const char* WIFI_PASSWORD = "MyPassword";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String cmd = client.readStringUntil('\n');
    // Process command (D2:1, A5:128, etc.)
  }
}
```

### Advanced Example with Libraries

See [examples/ESP32_WiFi_Advanced.ino](examples/ESP32_WiFi_Advanced.ino) for a complete implementation using CommandParser and PinValidator.

---

## 🔧 Troubleshooting

### WiFi Won't Connect
- ✅ Check SSID and password (case-sensitive)
- ✅ Use 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- ✅ Ensure good signal strength

### Bluetooth Pairing Fails
- ✅ ESP32: Device name is "ESP32_LED", PIN is "1234"
- ✅ HC-05: Check wiring, default baud is 9600
- ✅ Ensure module is powered and LED blinking

### Upload Fails (ESP32)
- ✅ Hold BOOT button while uploading
- ✅ Select correct board ("ESP32 Dev Module")
- ✅ Install CP210x or CH340 driver

### Commands Not Working
- ✅ Check Serial Monitor output for errors
- ✅ Verify commands end with newline (`\n`)
- ✅ Ensure pin numbers are valid for your board

For more troubleshooting, see [README.md](README.md).

---

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

### Adding a New Board

1. Create folder: `boards/YourBoard/ConnectionType/`
2. Copy template: `templates/Template_Basic.ino`
3. Modify for your board
4. Update documentation
5. Test thoroughly
6. Submit pull request

### Reporting Issues

- Use GitHub Issues
- Include: Board type, connection method, error messages
- Attach Serial Monitor output if possible

### Feature Requests

- Sensor reading commands
- Servo control
- Multi-client support
- Auto-code generation

---

## 📋 Requirements

### Hardware
- ESP32, Arduino Uno, or compatible board
- (Optional) HC-05/HC-06 Bluetooth module for Arduino
- (Optional) ESP8266 WiFi shield for Arduino Mega
- USB cable for programming
- LEDs, relays, or other devices to control

### Software
- Arduino IDE 1.8.x or 2.x
- ESP32 board support (for ESP32)
- Mobile device with iot.m.s Flutter app

---

## 📊 Protocol Details

### Command Format
```
<Command><Pin>:<Value>\n
```

### Command Types
- `D` - Digital (HIGH/LOW)
- `A` - Analog/PWM (0-255)
- `R` - Read (future)
- `S` - Servo (future)
- `C` - Config (future)

### Pin Validation
Each board has specific valid pins. The firmware validates pins before use to prevent damage.

### Flow Diagram
```
Mobile App
    ↓
  WiFi/BT
    ↓
ESP32/Arduino
    ↓
Parse Command
    ↓
Validate Pin
    ↓
Execute Action
    ↓
Send Response
```

---

## 📱 Mobile App Integration

This Arduino code works with the **iot.m.s Flutter mobile application**:

- Visual drag-and-drop control interface
- No coding required on mobile side
- Real-time GPIO control
- Support for multiple boards and connections

**App Features:**
- Board selection (ESP32, Uno, Mega, etc.)
- Connection method (WiFi, Bluetooth, Serial)
- Playground with draggable controls
- Digital buttons and analog sliders
- Custom pin naming and positioning

---

## 🎓 Learning Resources

- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [HC-05 Bluetooth Module Guide](https://components101.com/wireless/hc-05-bluetooth-module)
- [Arduino Serial Communication](https://www.arduino.cc/reference/en/language/functions/communication/serial/)

---

## 🗺️ Roadmap

- [ ] Arduino Mega complete implementation
- [ ] NodeMCU/ESP8266 sketches
- [ ] Sensor reading commands (R prefix)
- [ ] Servo control commands (S prefix)
- [ ] OTA updates for ESP32
- [ ] Multi-client WiFi support
- [ ] Auto-generated code from app config
- [ ] BLE support for ESP32
- [ ] Web interface for configuration

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 👥 Authors

- **iot.m.s Team** - Initial work

---

## 🙏 Acknowledgments

- Arduino community for excellent documentation
- Espressif for ESP32 platform
- Flutter team for mobile framework
- All contributors and testers

---

## 📞 Support

- 📧 Email: [Your Email]
- 🐛 Issues: [GitHub Issues](https://github.com/yourusername/iot.m.s/issues)
- 💬 Discussions: [GitHub Discussions](https://github.com/yourusername/iot.m.s/discussions)
- 📖 Docs: [Full Documentation](README.md)

---

## 🌟 Star History

If you find this project useful, please consider giving it a star ⭐

---

## 📸 Screenshots

### Serial Monitor Output
```
========================================
ESP32 WiFi Control - iot.m.s
========================================
Connecting to WiFi: MyHomeWiFi
.......
✅ WiFi connected!
IP Address: 192.168.1.100
Signal Strength: -45 dBm
TCP Server started on port 80
Ready to accept connections!
========================================

✅ New client connected: 192.168.1.101
📩 Received: D2:1
💡 Digital Pin 2 → HIGH
📤 Response: OK
```

### Mobile App Playground
```
[Visual representation - see Flutter app screenshots]
- Draggable digital buttons
- Analog sliders
- Real-time control
- Custom layouts
```

---

## 🔗 Related Projects

- [iot.m.s Flutter App](../my_first_app_1/) - Mobile application
- [Main Project](../) - Complete iot.m.s system

---

## 📈 Version History

### v1.0.0 (Current)
- ✅ ESP32 WiFi and Bluetooth support
- ✅ Arduino Uno Bluetooth and Serial support
- ✅ Common libraries (CommandParser, PinValidator)
- ✅ Complete documentation
- ✅ Template system

### Upcoming
- Arduino Mega support
- NodeMCU support
- Sensor reading
- Servo control

---

**Made with ❤️ for the IoT community**

---

## 🚦 Getting Help

**Before asking for help, please:**

1. ✅ Read [QUICK_START.md](QUICK_START.md)
2. ✅ Check [Troubleshooting](#-troubleshooting) section
3. ✅ Review Serial Monitor output
4. ✅ Search existing issues

**When reporting issues, include:**
- Board type and connection method
- Exact error message
- Serial Monitor output
- Sketch you're using
- Steps to reproduce

---

**Happy Making! 🎉**
