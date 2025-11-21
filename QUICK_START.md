# Quick Start Guide - Arduino Code

Get your board running with iot.m.s in 5 minutes!

## Step 1: Choose Your Setup

Pick the combination that matches your hardware:

| I have... | Use this file |
|-----------|---------------|
| ESP32 + Want WiFi | `boards/ESP32/WiFi/ESP32_WiFi_Basic.ino` |
| ESP32 + Want Bluetooth | `boards/ESP32/Bluetooth/ESP32_Bluetooth_Basic.ino` |
| Arduino Uno + HC-05 Bluetooth | `boards/ArduinoUno/Bluetooth/Uno_Bluetooth_HC05.ino` |
| Arduino Uno + USB Cable | `boards/ArduinoUno/Serial/Uno_Serial_USB.ino` |

## Step 2: Install Board Support

### For ESP32:
1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to **Tools → Board → Board Manager**
5. Search "ESP32" and install "esp32 by Espressif Systems"

### For Arduino Uno/Mega:
- Already supported in Arduino IDE, no installation needed

## Step 3: Configure the Sketch

### ESP32 WiFi:
Open the file and change these lines:
```cpp
const char* WIFI_SSID = "YOUR_WIFI_SSID";        // Your WiFi name
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD"; // Your WiFi password
```

### ESP32 Bluetooth:
No configuration needed! Device name is "ESP32_LED", PIN is "1234"

### Arduino Uno Bluetooth:
Wire the HC-05 module:
- HC-05 VCC → Arduino 5V
- HC-05 GND → Arduino GND
- HC-05 TX → Arduino Pin 10
- HC-05 RX → Arduino Pin 11 (via voltage divider!)

**Voltage Divider (IMPORTANT)**:
```
Arduino Pin 11 → 1K Resistor → HC-05 RX
                              → 2K Resistor → GND
```

## Step 4: Upload Code

1. **Connect board** to computer via USB
2. **Select board**:
   - Tools → Board → ESP32 Dev Module (for ESP32)
   - Tools → Board → Arduino Uno (for Uno)
3. **Select port**: Tools → Port → (your board's port)
4. Click **Upload** button (→)
5. Wait for "Done uploading" message

## Step 5: Get Connection Info

### ESP32 WiFi:
1. Open **Tools → Serial Monitor**
2. Set baud rate to **115200**
3. Look for the IP address:
   ```
   WiFi connected!
   IP Address: 192.168.1.100  ← Use this in the app
   ```

### ESP32 Bluetooth:
1. Open phone Bluetooth settings
2. Look for "ESP32_LED"
3. Pair with PIN "1234"
4. Connect via iot.m.s app

### Arduino Uno Bluetooth:
1. Open phone Bluetooth settings
2. Look for "HC-05" (or similar)
3. Pair with PIN "1234" or "0000"
4. Connect via iot.m.s app

## Step 6: Test It!

### Serial Monitor Test:
Open Serial Monitor (115200 baud) and type:
```
D2:1    → Turn ON pin 2
D2:0    → Turn OFF pin 2
D13:1   → Turn ON built-in LED
```

### Mobile App Test:
1. Open iot.m.s Flutter app
2. Select your board type
3. Tap "Connect"
4. Choose Bluetooth or WiFi
5. Add a digital button for pin 2
6. Tap the button - it should work!

## Troubleshooting

### "Board not found"
- Check USB cable is data cable (not charge-only)
- Try different USB port
- Install CH340/CP210x drivers if needed

### "Upload failed"
- **ESP32**: Hold BOOT button while uploading
- Check correct board is selected
- Close Serial Monitor before uploading

### WiFi won't connect
- Double-check SSID and password
- Make sure router is 2.4GHz (not 5GHz)
- Try moving closer to router

### Bluetooth won't pair
- **ESP32**: Check Bluetooth is enabled in phone settings
- **HC-05**: Module LED should be blinking
- Try PIN "0000" if "1234" doesn't work

### Commands not working
- Check Serial Monitor is set to 115200 baud
- Verify commands end with newline (use Serial Monitor's newline setting)
- Make sure pin numbers are valid for your board

## Next Steps

✅ **It works!** Now you can:
- Try different pins (see pin reference in README.md)
- Add analog sliders (A5:128 commands)
- Test with the mobile app playground
- Customize the code for your project

📚 **Learn more**:
- [README.md](README.md) - Full documentation
- [CLAUDE.md](CLAUDE.md) - Developer guide
- `examples/` - Advanced examples

## Pin Quick Reference

### ESP32
```
Safe digital pins: 2, 4, 5, 12-19, 21-27, 32-33
Built-in LED: Pin 2
```

### Arduino Uno
```
Digital pins: 2-13
PWM pins: 3, 5, 6, 9, 10, 11
Built-in LED: Pin 13
```

## Common Commands

```bash
# Test commands in Serial Monitor:
D13:1          # Turn on built-in LED
D13:0          # Turn off built-in LED
A9:128         # Set pin 9 to 50% PWM (128/255)
A9:255         # Set pin 9 to 100% PWM
A9:0           # Set pin 9 to 0% PWM
```

---

**Need help?** Check the full [README.md](README.md) or [CLAUDE.md](CLAUDE.md) for detailed documentation.
