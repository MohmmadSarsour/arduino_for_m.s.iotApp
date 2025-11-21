/**
 * PinValidator.h
 *
 * Pin validation utilities for different boards
 */

#ifndef PIN_VALIDATOR_H
#define PIN_VALIDATOR_H

#include <Arduino.h>

class PinValidator {
public:
  /**
   * Check if pin is valid for digital operations
   * @param pin The pin number
   * @return true if valid, false otherwise
   */
  static bool isValidDigitalPin(int pin) {
    #if defined(ESP32)
      // ESP32 valid GPIO pins
      const int validPins[] = {2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};
      for (int i = 0; i < sizeof(validPins) / sizeof(validPins[0]); i++) {
        if (pin == validPins[i]) return true;
      }
      return false;

    #elif defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_UNO)
      // Arduino Uno: pins 2-13
      return (pin >= 2 && pin <= 13);

    #elif defined(__AVR_ATmega2560__) || defined(ARDUINO_AVR_MEGA2560)
      // Arduino Mega: pins 2-53
      return (pin >= 2 && pin <= 53);

    #elif defined(ESP8266)
      // NodeMCU/ESP8266
      const int validPins[] = {0, 2, 4, 5, 12, 13, 14, 15, 16};
      for (int i = 0; i < sizeof(validPins) / sizeof(validPins[0]); i++) {
        if (pin == validPins[i]) return true;
      }
      return false;

    #else
      // Default: allow most pins
      return (pin >= 0 && pin <= 53);
    #endif
  }

  /**
   * Check if pin supports PWM (analog write)
   * @param pin The pin number
   * @return true if PWM capable, false otherwise
   */
  static bool isPWMPin(int pin) {
    #if defined(ESP32)
      // Most ESP32 GPIO pins support PWM
      return isValidDigitalPin(pin);

    #elif defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_UNO)
      // Arduino Uno PWM pins: 3, 5, 6, 9, 10, 11
      const int pwmPins[] = {3, 5, 6, 9, 10, 11};
      for (int i = 0; i < sizeof(pwmPins) / sizeof(pwmPins[0]); i++) {
        if (pin == pwmPins[i]) return true;
      }
      return false;

    #elif defined(__AVR_ATmega2560__) || defined(ARDUINO_AVR_MEGA2560)
      // Arduino Mega PWM pins: 2-13, 44-46
      return ((pin >= 2 && pin <= 13) || (pin >= 44 && pin <= 46));

    #elif defined(ESP8266)
      // NodeMCU PWM pins
      const int pwmPins[] = {0, 2, 4, 5, 12, 13, 14, 15};
      for (int i = 0; i < sizeof(pwmPins) / sizeof(pwmPins[0]); i++) {
        if (pin == pwmPins[i]) return true;
      }
      return false;

    #else
      return true; // Assume capable if unknown
    #endif
  }

  /**
   * Check if pin is valid for analog read
   * @param pin The pin number
   * @return true if valid ADC pin, false otherwise
   */
  static bool isValidAnalogPin(int pin) {
    #if defined(ESP32)
      // ESP32 ADC pins: 32-39
      const int adcPins[] = {32, 33, 34, 35, 36, 39};
      for (int i = 0; i < sizeof(adcPins) / sizeof(adcPins[0]); i++) {
        if (pin == adcPins[i]) return true;
      }
      return false;

    #elif defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_UNO)
      // Arduino Uno: A0-A5 (14-19 as digital)
      return (pin >= A0 && pin <= A5) || (pin >= 14 && pin <= 19);

    #elif defined(__AVR_ATmega2560__) || defined(ARDUINO_AVR_MEGA2560)
      // Arduino Mega: A0-A15
      return (pin >= A0 && pin <= A15);

    #elif defined(ESP8266)
      // NodeMCU has only A0
      return (pin == A0 || pin == 0);

    #else
      return true; // Assume capable if unknown
    #endif
  }

  /**
   * Get board name
   * @return String with board name
   */
  static String getBoardName() {
    #if defined(ESP32)
      return "ESP32";
    #elif defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_UNO)
      return "Arduino Uno";
    #elif defined(__AVR_ATmega2560__) || defined(ARDUINO_AVR_MEGA2560)
      return "Arduino Mega";
    #elif defined(ESP8266)
      return "NodeMCU/ESP8266";
    #else
      return "Unknown Board";
    #endif
  }
};

#endif // PIN_VALIDATOR_H
