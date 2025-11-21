# Arduino Code - Project Structure

Complete overview of the iot.m.s Arduino code organization.

## Directory Tree

```
my_first_app_toArduino/
│
├── 📁 boards/                          Board-specific implementations
│   ├── 📁 ESP32/
│   │   ├── 📁 Bluetooth/
│   │   │   └── ESP32_Bluetooth_Basic.ino    ✅ Ready to use
│   │   └── 📁 WiFi/
│   │       └── ESP32_WiFi_Basic.ino         ✅ Ready to use
│   │
│   ├── 📁 ArduinoUno/
│   │   ├── 📁 Bluetooth/
│   │   │   └── Uno_Bluetooth_HC05.ino       ✅ Ready to use
│   │   └── 📁 Serial/
│   │       └── Uno_Serial_USB.ino           ✅ Ready to use
│   │
│   ├── 📁 ArduinoMega/
│   │   ├── 📁 Bluetooth/                    🚧 Future
│   │   ├── 📁 Serial/                       🚧 Future
│   │   └── 📁 WiFi/                         🚧 Future
│   │
│   └── 📁 NodeMCU/
│       └── 📁 WiFi/                         🚧 Future
│
├── 📁 common/                          Shared utilities (reusable)
│   ├── 📁 protocol/
│   │   └── CommandParser.h              Protocol parsing library
│   └── 📁 utils/
│       └── PinValidator.h               Pin validation for all boards
│
├── 📁 examples/                        Complete working examples
│   └── ESP32_WiFi_Advanced.ino         Advanced example using common libs
│
├── 📁 templates/                       Starting templates
│   └── Template_Basic.ino              Basic template for new boards
│
├── 📄 CLAUDE.md                        Developer documentation
├── 📄 README.md                        Full documentation
├── 📄 QUICK_START.md                   5-minute setup guide
├── 📄 PROJECT_STRUCTURE.md             This file
└── 📄 my_first_app_toArduino.ino       Original code (legacy)
```

## File Purposes

### Ready-to-Use Sketches

| File | Board | Connection | Purpose |
|------|-------|------------|---------|
| `ESP32_Bluetooth_Basic.ino` | ESP32 | Bluetooth | Basic BT control with built-in module |
| `ESP32_WiFi_Basic.ino` | ESP32 | WiFi | WiFi TCP server control |
| `Uno_Bluetooth_HC05.ino` | Arduino Uno | Bluetooth | HC-05/HC-06 module control |
| `Uno_Serial_USB.ino` | Arduino Uno | USB Serial | USB OTG connection |

### Common Libraries

| File | Purpose | Used By |
|------|---------|---------|
| `CommandParser.h` | Parse D/A/R/S/C commands | All sketches |
| `PinValidator.h` | Validate pins per board | All sketches |

### Templates & Examples

| File | Purpose |
|------|---------|
| `Template_Basic.ino` | Starting point for new board configs |
| `ESP32_WiFi_Advanced.ino` | Advanced example with common libs |

### Documentation

| File | Audience | Content |
|------|----------|---------|
| `QUICK_START.md` | Beginners | 5-minute setup guide |
| `README.md` | All users | Complete documentation |
| `CLAUDE.md` | Developers | Architecture & code guidelines |
| `PROJECT_STRUCTURE.md` | All users | This overview |

## Code Flow

### Standard Sketch Structure

```
┌─────────────────┐
│  setup()        │
│  ├─ Serial init│
│  ├─ Pin init   │
│  └─ Connection │
└─────────────────┘
         ↓
┌─────────────────┐
│  loop()         │
│  ├─ Read data  │
│  ├─ Build cmd  │
│  └─ On \n →    │────┐
└─────────────────┘    │
                       ↓
         ┌─────────────────────┐
         │  processCommand()   │
         │  ├─ Parse          │
         │  ├─ Validate pin   │
         │  ├─ Execute        │
         │  └─ Send response  │
         └─────────────────────┘
```

### Using Common Libraries

```
┌──────────────────┐
│ Received: "D2:1" │
└──────────────────┘
         ↓
┌────────────────────────────┐
│ CommandParser::parse()     │
│ Returns: {                 │
│   type: CMD_DIGITAL,       │
│   pin: 2,                  │
│   value: 1,                │
│   valid: true              │
│ }                          │
└────────────────────────────┘
         ↓
┌────────────────────────────┐
│ PinValidator::             │
│   isValidDigitalPin(2)     │
│ Returns: true (ESP32)      │
└────────────────────────────┘
         ↓
┌────────────────────────────┐
│ CommandParser::            │
│   executeDigital(cmd)      │
│ → digitalWrite(2, HIGH)    │
└────────────────────────────┘
```

## Communication Protocol

### Command Format

```
┌─────────────────────────────────────┐
│  Mobile App                         │
│  ↓ sends command                    │
│  D2:1\n                             │
└─────────────────────────────────────┘
         ↓ WiFi/Bluetooth
┌─────────────────────────────────────┐
│  Arduino/ESP32                      │
│  ↓ receives & parses                │
│  Type: Digital, Pin: 2, Value: 1    │
│  ↓ validates                        │
│  Pin 2 is valid ✓                   │
│  ↓ executes                         │
│  digitalWrite(2, HIGH)              │
│  ↓ responds                         │
│  OK\n                               │
└─────────────────────────────────────┘
         ↓ WiFi/Bluetooth
┌─────────────────────────────────────┐
│  Mobile App                         │
│  ↓ receives response                │
│  Shows: "Command sent successfully" │
└─────────────────────────────────────┘
```

### Supported Commands

| Command | Format | Example | Action |
|---------|--------|---------|--------|
| Digital | `D<pin>:<value>\n` | `D2:1\n` | Set pin 2 HIGH |
| Analog | `A<pin>:<value>\n` | `A5:128\n` | Set pin 5 PWM to 128 |
| Read | `R<pin>\n` | `R34\n` | Read analog pin 34 |
| Servo* | `S<pin>:<angle>\n` | `S9:90\n` | Set servo to 90° |
| Config* | `C<cmd>\n` | `CSTATUS\n` | Get status |

*Future implementation

## Board-Specific Details

### ESP32

```
📦 ESP32 Package
├─ Built-in WiFi  ✅
├─ Built-in Bluetooth  ✅
├─ Digital pins: 2, 4, 5, 12-19, 21-27, 32-33
├─ PWM: Most GPIO pins
├─ ADC: 32-39
└─ Voltage: 3.3V logic
```

**Files**:
- `boards/ESP32/Bluetooth/ESP32_Bluetooth_Basic.ino`
- `boards/ESP32/WiFi/ESP32_WiFi_Basic.ino`
- `examples/ESP32_WiFi_Advanced.ino`

### Arduino Uno

```
📦 Arduino Uno Package
├─ Bluetooth: Via HC-05/HC-06 module
├─ WiFi: Not supported
├─ Digital pins: 2-13
├─ PWM pins: 3, 5, 6, 9, 10, 11
├─ Analog: A0-A5
└─ Voltage: 5V logic
```

**Files**:
- `boards/ArduinoUno/Bluetooth/Uno_Bluetooth_HC05.ino`
- `boards/ArduinoUno/Serial/Uno_Serial_USB.ino`

### Arduino Mega (Future)

```
📦 Arduino Mega Package
├─ Bluetooth: Via HC-05/HC-06 module
├─ WiFi: Via ESP8266 shield
├─ Digital pins: 2-53
├─ PWM pins: 2-13, 44-46
├─ Analog: A0-A15
└─ Voltage: 5V logic
```

**Files**: To be created

### NodeMCU/ESP8266 (Future)

```
📦 NodeMCU Package
├─ Built-in WiFi  ✅
├─ Bluetooth: Not supported
├─ Digital pins: D0-D8
├─ PWM: D0-D8
├─ ADC: A0 (single channel)
└─ Voltage: 3.3V logic
```

**Files**: To be created

## Development Workflow

### Creating a New Board Configuration

```
1. Copy template
   cp templates/Template_Basic.ino boards/NewBoard/Connection/Sketch.ino

2. Update configuration
   - Board-specific includes
   - Pin definitions
   - Connection settings

3. Implement functions
   - initializeConnection()
   - isValidDigitalPin()
   - isPWMPin()
   - sendResponse()

4. Test
   - Upload to board
   - Test via Serial Monitor
   - Test via mobile app

5. Document
   - Add to README.md
   - Update this file
```

### Using Common Libraries

```
Option 1: Copy to sketch folder
   cp common/protocol/CommandParser.h MySketch/
   cp common/utils/PinValidator.h MySketch/
   #include "CommandParser.h"

Option 2: Use relative path
   #include "../common/protocol/CommandParser.h"
   #include "../common/utils/PinValidator.h"
```

## Integration Points

### Mobile App Connection

```
Flutter App (my_first_app_1)
├─ Bluetooth: Uses platform channel 'my_channel'
│  └─ Method: sendData(message: "D2:1")
│
└─ WiFi: Uses ConnectionManager (Dart socket)
   └─ Method: sendCommand("D2:1")
         ↓
    TCP Socket to ESP32
         ↓
    Arduino receives command
```

### Pin Configuration Flow

```
Flutter App
├─ User selects board type (ESP32/Uno)
├─ User adds controls in playground
│  - Digital button: Pin 2, Name "LED"
│  - Analog slider: Pin 5, Name "Brightness"
├─ User connects via Bluetooth/WiFi
└─ User interacts with controls
   ↓
   Sends: "D2:1" or "A5:128"
         ↓
    Arduino executes commands
```

## Future Enhancements

### Planned Features

- [ ] Arduino Mega sketches
- [ ] NodeMCU/ESP8266 sketches
- [ ] Sensor reading commands (R prefix)
- [ ] Servo control commands (S prefix)
- [ ] Configuration commands (C prefix)
- [ ] Auto-generated sketches from app config
- [ ] OTA (Over-The-Air) updates for ESP32
- [ ] Multi-client support for WiFi

### Auto-Code Generation

```
Future workflow:
1. User configures pins in Flutter app
2. App generates Arduino code
3. User copies code to Arduino IDE
4. Upload to board
5. Ready to use!
```

## File Maintenance

### When Adding a New Board

1. Create folder: `boards/YourBoard/ConnectionType/`
2. Add sketch based on template
3. Update `README.md` - Supported Boards section
4. Update this file - Board-Specific Details
5. Test thoroughly

### When Adding a Feature

1. Update `common/protocol/CommandParser.h` if new command type
2. Update all affected sketches
3. Update `CLAUDE.md` - Protocol section
4. Update `README.md` - Command Format section
5. Create example in `examples/`

## Quick Reference

### File Sizes (Approximate)

- Basic sketches: ~5-8 KB
- Advanced sketches: ~8-12 KB
- Common libraries: ~3-5 KB each
- Documentation: ~10-15 KB each

### Arduino IDE Board Settings

**ESP32**:
- Board: "ESP32 Dev Module"
- Upload Speed: 921600
- Flash Frequency: 80MHz

**Arduino Uno**:
- Board: "Arduino Uno"
- Processor: ATmega328P

**Arduino Mega**:
- Board: "Arduino Mega or Mega 2560"
- Processor: ATmega2560

---

**Last Updated**: 2025-11-21
**Project**: iot.m.s (IoT Mobile System)
**Version**: 1.0
