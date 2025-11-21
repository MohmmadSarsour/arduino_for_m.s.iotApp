# Contributing to iot.m.s Arduino Code

Thank you for your interest in contributing to the iot.m.s Arduino project! 🎉

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Submitting Changes](#submitting-changes)
- [Adding New Boards](#adding-new-boards)
- [Testing Guidelines](#testing-guidelines)

---

## 📜 Code of Conduct

- Be respectful and inclusive
- Provide constructive feedback
- Focus on the code, not the person
- Help others learn and grow

---

## 🤝 How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues. When creating a bug report, include:

- **Board type** (ESP32, Arduino Uno, etc.)
- **Connection method** (WiFi, Bluetooth, Serial)
- **Arduino IDE version**
- **Error messages** from Serial Monitor
- **Steps to reproduce**
- **Expected vs actual behavior**

**Bug Report Template:**

```markdown
**Board:** ESP32
**Connection:** WiFi
**Arduino IDE:** 2.0.3
**Sketch:** boards/ESP32/WiFi/ESP32_WiFi_Basic.ino

**Description:**
WiFi connection fails after 10 attempts

**Steps to Reproduce:**
1. Upload sketch with correct SSID/password
2. Open Serial Monitor
3. Observe connection attempts

**Expected:**
WiFi should connect

**Actual:**
Connection fails with "WiFi connection failed!"

**Serial Output:**
[Paste Serial Monitor output]
```

### Suggesting Enhancements

Enhancement suggestions are welcome! Include:

- **Clear description** of the feature
- **Use case** - why is it needed?
- **Examples** - how would it work?
- **Compatibility** - which boards would support it?

### Pull Requests

We love pull requests! Here's how:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Make your changes
4. Test thoroughly
5. Commit with clear messages
6. Push to your fork
7. Open a Pull Request

---

## 🛠️ Development Setup

### Prerequisites

1. **Arduino IDE** 1.8.x or 2.x
2. **Board Support**:
   - ESP32: Add board manager URL
   - Arduino: Built-in support
3. **Git** for version control

### Setting Up

```bash
# Clone the repository
git clone https://github.com/yourusername/iot.m.s.git
cd iot.m.s/my_first_app_toArduino

# Create a new branch for your feature
git checkout -b feature/my-new-feature
```

### Testing Your Changes

1. Upload sketch to actual hardware
2. Test all supported commands
3. Verify Serial Monitor output
4. Test with mobile app
5. Document any issues

---

## 📝 Coding Standards

### File Structure

Every sketch should follow this structure:

```cpp
/**
 * Sketch Name
 *
 * Description of what this sketch does
 *
 * Board: [Board Name]
 * Connection: [Connection Type]
 * Protocol: D<pin>:<value> for digital, A<pin>:<value> for analog
 */

// ========== INCLUDES ==========
#include <RequiredLibrary.h>

// ========== CONFIGURATION ==========
// User-configurable settings here
const char* WIFI_SSID = "YOUR_WIFI_SSID";

// ========== GLOBAL VARIABLES ==========
// Global variables here

// ========== SETUP ==========
void setup() {
  // Initialization code
}

// ========== MAIN LOOP ==========
void loop() {
  // Main logic
}

// ========== HELPER FUNCTIONS ==========
void helperFunction() {
  // Helper code
}
```

### Naming Conventions

- **Constants**: `UPPER_CASE` (e.g., `LED_PIN`, `BAUD_RATE`)
- **Variables**: `camelCase` (e.g., `commandBuffer`, `isConnected`)
- **Functions**: `camelCase` (e.g., `processCommand`, `connectToWiFi`)
- **Pin numbers**: Use `const int` (e.g., `const int LED_PIN = 2`)

### Comments

```cpp
// Single line comment for brief explanations

/**
 * Multi-line comment for functions
 * @param pin The GPIO pin number
 * @return true if valid, false otherwise
 */
bool isValidPin(int pin) {
  // Implementation
}

// ========== SECTION SEPARATOR ==========
// Use for major sections
```

### Serial Output

Use emojis for clarity (optional but encouraged):

```cpp
Serial.println("✅ Success message");
Serial.println("❌ Error message");
Serial.println("📩 Received data");
Serial.println("📤 Sending data");
Serial.println("💡 Pin state change");
Serial.println("🎚️ Analog/PWM change");
Serial.println("⚠️ Warning message");
```

### Error Handling

Always validate and provide feedback:

```cpp
// Good
if (!isValidPin(pin)) {
  Serial.println("❌ Invalid pin: " + String(pin));
  sendResponse("ERROR:Invalid pin");
  return;
}

// Bad
digitalWrite(pin, HIGH); // No validation!
```

---

## 🚀 Submitting Changes

### Commit Messages

Use clear, descriptive commit messages:

```bash
# Good
git commit -m "Add NodeMCU WiFi support with pin validation"
git commit -m "Fix analog command parsing for negative values"
git commit -m "Update ESP32 Bluetooth sketch documentation"

# Bad
git commit -m "Fixed stuff"
git commit -m "Updates"
git commit -m "WIP"
```

### Pull Request Process

1. **Update documentation** if needed
2. **Test on real hardware**
3. **Follow code style** guidelines
4. **Write clear PR description**:

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Code refactoring

## Testing
- [ ] Tested on ESP32
- [ ] Tested on Arduino Uno
- [ ] Works with mobile app
- [ ] Serial Monitor output verified

## Checklist
- [ ] Code follows style guidelines
- [ ] Documentation updated
- [ ] No compilation errors
- [ ] Tested on actual hardware
```

---

## 🔌 Adding New Boards

### Step-by-Step Guide

1. **Create Directory Structure**:
   ```bash
   mkdir -p boards/YourBoard/WiFi
   mkdir -p boards/YourBoard/Bluetooth
   mkdir -p boards/YourBoard/Serial
   ```

2. **Copy Template**:
   ```bash
   cp templates/Template_Basic.ino boards/YourBoard/WiFi/YourBoard_WiFi.ino
   ```

3. **Modify Template**:
   - Update includes for your board
   - Set correct pin definitions
   - Implement connection setup
   - Implement pin validation

4. **Update Documentation**:
   - Add board to `README.md`
   - Update `GIT_README.md`
   - Add to `QUICK_START.md`
   - Update `CLAUDE.md`

5. **Test Thoroughly**:
   - All digital pins
   - All PWM pins
   - Connection stability
   - Command parsing
   - Mobile app integration

6. **Submit PR** with:
   - Working sketch
   - Documentation updates
   - Pin mapping details
   - Testing notes

### Board Support Checklist

- [ ] Create board folder structure
- [ ] Implement WiFi sketch (if supported)
- [ ] Implement Bluetooth sketch (if supported)
- [ ] Implement Serial sketch
- [ ] Add pin validation function
- [ ] Test all commands
- [ ] Update README.md
- [ ] Update GIT_README.md
- [ ] Add to QUICK_START.md
- [ ] Update CLAUDE.md
- [ ] Include wiring diagrams (if external modules needed)
- [ ] Test with mobile app

---

## 🧪 Testing Guidelines

### Unit Testing

Test each function independently:

```cpp
void testCommandParsing() {
  Serial.println("Testing command parsing...");

  // Test digital command
  processCommand("D2:1");
  // Expected: Pin 2 HIGH

  // Test analog command
  processCommand("A5:128");
  // Expected: Pin 5 PWM 128

  // Test invalid command
  processCommand("X99:1");
  // Expected: Error message

  Serial.println("✅ All tests passed!");
}
```

### Integration Testing

1. **Upload sketch** to board
2. **Open Serial Monitor** (correct baud rate)
3. **Test commands manually**:
   ```
   D2:1
   D2:0
   A5:0
   A5:128
   A5:255
   ```
4. **Connect mobile app**
5. **Test all controls**
6. **Verify responses**

### Hardware Testing

- [ ] WiFi connection (if applicable)
- [ ] Bluetooth pairing (if applicable)
- [ ] Serial communication
- [ ] All digital pins
- [ ] All PWM pins
- [ ] LED control
- [ ] Response time
- [ ] Stability (30+ minutes)
- [ ] Reconnection after disconnect

---

## 📚 Documentation Standards

### Code Documentation

Every sketch must include:

```cpp
/**
 * File Name
 *
 * Detailed description of what this sketch does
 * and any special requirements
 *
 * Board: Specific board name
 * Connection: WiFi/Bluetooth/Serial
 * Protocol: Command format
 * Author: Your name (optional)
 * Date: YYYY-MM-DD (optional)
 */
```

### README Updates

When adding features, update:

1. **README.md** - User-facing documentation
2. **GIT_README.md** - Repository README
3. **QUICK_START.md** - If setup process changes
4. **CLAUDE.md** - Architecture changes
5. **PROJECT_STRUCTURE.md** - Structure changes

---

## 🏗️ Architecture Guidelines

### Use Common Libraries

When possible, use shared code:

```cpp
#include "common/protocol/CommandParser.h"
#include "common/utils/PinValidator.h"

// Better than duplicating code in each sketch
```

### Keep It Simple

- Avoid over-engineering
- Use clear variable names
- Comment complex logic only
- Prefer readability over cleverness

### Follow Protocol

Maintain protocol compatibility:

- Commands: `D<pin>:<value>`, `A<pin>:<value>`
- Responses: `OK`, `ERROR:<message>`
- Always newline-terminated (`\n`)
- Case-insensitive command parsing

---

## 🐛 Debugging Tips

### Serial Output

Always use descriptive Serial output:

```cpp
#define DEBUG_SERIAL  // Enable debug mode

#ifdef DEBUG_SERIAL
  Serial.println("📩 Received: " + command);
  Serial.println("Pin: " + String(pin) + ", Value: " + String(value));
#endif
```

### Common Issues

**WiFi won't connect:**
- Check SSID/password spelling
- Verify 2.4GHz network
- Check router settings

**Bluetooth pairing fails:**
- Ensure correct PIN
- Check module wiring
- Verify baud rate

**Commands ignored:**
- Check newline termination
- Verify pin validation
- Monitor Serial output

---

## 📞 Getting Help

If you're stuck:

1. Check existing documentation
2. Search existing issues
3. Ask in Discussions
4. Create detailed issue with:
   - What you're trying to do
   - What you've tried
   - Error messages
   - Code snippets

---

## 🙏 Recognition

Contributors will be:

- Listed in CONTRIBUTORS.md
- Mentioned in release notes
- Credited in relevant documentation

---

## 📜 License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to iot.m.s! 🚀
