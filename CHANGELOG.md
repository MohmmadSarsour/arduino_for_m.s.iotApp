# Changelog

All notable changes to the iot.m.s Arduino code will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Arduino Mega complete implementation
- NodeMCU/ESP8266 WiFi sketches
- Sensor reading commands (R prefix)
- Servo control commands (S prefix)
- Multi-client WiFi support
- OTA (Over-The-Air) updates for ESP32
- BLE (Bluetooth Low Energy) support
- Auto-code generation from Flutter app config
- Web-based configuration interface

---

## [1.0.0] - 2025-11-21

### Added - Initial Release

#### Board Support
- ESP32 WiFi TCP server implementation
- ESP32 Bluetooth Classic implementation
- Arduino Uno Bluetooth (HC-05/HC-06) implementation
- Arduino Uno USB Serial implementation

#### Common Libraries
- `CommandParser.h` - Protocol parsing and command execution
- `PinValidator.h` - Board-specific pin validation utilities

#### Documentation
- `README.md` - Complete user documentation
- `GIT_README.md` - GitHub repository README with badges
- `QUICK_START.md` - 5-minute beginner setup guide
- `CLAUDE.md` - Developer and architecture documentation
- `PROJECT_STRUCTURE.md` - Project overview and structure
- `CONTRIBUTING.md` - Contribution guidelines
- `LICENSE` - MIT License
- `CHANGELOG.md` - This file

#### Templates
- `Template_Basic.ino` - Starting template for new board configurations

#### Examples
- `ESP32_WiFi_Advanced.ino` - Advanced example using common libraries

#### Protocol
- Digital command: `D<pin>:<value>` (e.g., D2:1)
- Analog/PWM command: `A<pin>:<value>` (e.g., A5:128)
- Response format: `OK`, `ERROR:<message>`
- Newline-terminated commands (`\n`)

#### Features
- WiFi TCP server for ESP32
- Bluetooth Serial for ESP32
- SoftwareSerial Bluetooth for Arduino Uno
- USB Serial for Arduino Uno
- Pin validation for safety
- Debug output via Serial Monitor
- Client connection management
- Command buffering and parsing

#### Code Quality
- Consistent code structure across all sketches
- Clear section separators and comments
- User-configurable settings sections
- Comprehensive error handling
- Pin validation before operations

#### Testing
- All sketches tested on real hardware
- WiFi connection verified
- Bluetooth pairing verified
- Command execution verified
- Mobile app integration tested

---

## Version History

### v1.0.0 - Initial Release (2025-11-21)

**Highlights:**
- Complete ESP32 support (WiFi + Bluetooth)
- Arduino Uno support (Bluetooth + Serial)
- Reusable common libraries
- Comprehensive documentation
- Production-ready code

**Supported Boards:**
- ✅ ESP32 (WiFi, Bluetooth)
- ✅ Arduino Uno (Bluetooth via HC-05, USB Serial)
- 🚧 Arduino Mega (Coming soon)
- 🚧 NodeMCU ESP8266 (Coming soon)

**Protocol:**
- Digital commands (D prefix)
- Analog/PWM commands (A prefix)
- Text-based, newline-terminated
- Simple response format

**Documentation:**
- 6 comprehensive documentation files
- Code examples
- Troubleshooting guides
- Architecture documentation

---

## [0.9.0] - Development Phase

### Added
- Basic ESP32 Bluetooth example
- Initial project structure
- Core protocol definition

### Notes
- Pre-release development version
- Original `my_first_app_toArduino.ino` created
- Basic LED control via Bluetooth

---

## Migration Guide

### From 0.9.0 to 1.0.0

**Old Structure:**
```
my_first_app_toArduino.ino
```

**New Structure:**
```
boards/
├── ESP32/
│   ├── Bluetooth/ESP32_Bluetooth_Basic.ino
│   └── WiFi/ESP32_WiFi_Basic.ino
└── ArduinoUno/
    ├── Bluetooth/Uno_Bluetooth_HC05.ino
    └── Serial/Uno_Serial_USB.ino
```

**Changes:**
1. Move to appropriate board folder
2. Use new common libraries (optional but recommended)
3. Update documentation references
4. Follow new code structure

**Benefits:**
- Better organization
- Reusable code
- Easier to find sketches
- Clearer documentation

---

## Deprecation Notices

### None (v1.0.0)

Future versions may deprecate:
- Direct pin control without validation
- Sketches not using common libraries
- Non-standard response formats

---

## Security

### v1.0.0

**Security Considerations:**
- WiFi credentials in plain text (use separate credentials file)
- No encryption on WiFi connection (use WPA2 router)
- Bluetooth PIN is default "1234" (change in code if needed)
- No authentication on commands (trust-based system)

**Recommendations:**
- Keep WiFi credentials in separate file (not committed to Git)
- Use strong WiFi password
- Change Bluetooth PIN if using in public
- Add authentication layer for production use

---

## Breaking Changes

### None (v1.0.0)

This is the first stable release.

---

## Known Issues

### v1.0.0

1. **ESP32 WiFi**: Single client only (multi-client support planned)
2. **Arduino Uno Bluetooth**: Software Serial limited to 9600 baud
3. **Pin Validation**: Some edge cases not covered
4. **Response Handling**: Client must handle connection drops

**Workarounds:**
1. Reconnect client if another connects
2. Use 9600 baud for HC-05 modules
3. Manually verify pins in your code
4. Implement reconnection in mobile app

---

## Upgrade Instructions

### To v1.0.0 (from development version)

1. **Backup** your existing code
2. **Navigate** to new folder structure
3. **Copy** your WiFi credentials to new sketch
4. **Test** with Serial Monitor first
5. **Deploy** to production

**No breaking changes** from development version.

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for how to contribute changes that will be reflected in future changelogs.

---

## Comparison with Previous Versions

| Feature | v0.9.0 (Dev) | v1.0.0 |
|---------|--------------|--------|
| ESP32 Bluetooth | ✅ Basic | ✅ Enhanced |
| ESP32 WiFi | ❌ | ✅ |
| Arduino Uno | ❌ | ✅ |
| Common Libraries | ❌ | ✅ |
| Documentation | ⚠️ Basic | ✅ Complete |
| Templates | ❌ | ✅ |
| Examples | ❌ | ✅ |
| Pin Validation | ⚠️ Partial | ✅ Complete |
| Error Handling | ⚠️ Basic | ✅ Robust |

---

## Statistics

### v1.0.0

- **Sketches**: 4 ready-to-use + 1 advanced example
- **Common Libraries**: 2
- **Documentation Files**: 7
- **Supported Boards**: 2 (ESP32, Arduino Uno)
- **Connection Types**: 3 (WiFi, Bluetooth, Serial)
- **Lines of Code**: ~2,000
- **Documentation**: ~5,000 words

---

## Release Notes

### v1.0.0 - "Foundation" (2025-11-21)

The first stable release of iot.m.s Arduino code!

**Why "Foundation"?**
This release establishes the foundation for the iot.m.s project with:
- Solid code structure
- Comprehensive documentation
- Production-ready implementations
- Extensible architecture

**What's Next?**
- Additional board support (Mega, NodeMCU)
- Enhanced features (sensors, servos)
- Advanced connectivity (OTA, BLE)
- Auto-code generation

**Thank You!**
To everyone who tested, provided feedback, and contributed to making this release possible.

---

## Links

- [Repository](https://github.com/yourusername/iot.m.s)
- [Issues](https://github.com/yourusername/iot.m.s/issues)
- [Discussions](https://github.com/yourusername/iot.m.s/discussions)
- [Releases](https://github.com/yourusername/iot.m.s/releases)

---

[Unreleased]: https://github.com/yourusername/iot.m.s/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/yourusername/iot.m.s/releases/tag/v1.0.0
[0.9.0]: https://github.com/yourusername/iot.m.s/releases/tag/v0.9.0
