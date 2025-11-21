/**
 * CommandParser.h
 *
 * Common protocol parser for iot.m.s project
 * Handles parsing of commands from mobile app
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <Arduino.h>

enum CommandType {
  CMD_DIGITAL,   // D<pin>:<value>
  CMD_ANALOG,    // A<pin>:<value>
  CMD_READ,      // R<pin> (future)
  CMD_SERVO,     // S<pin>:<angle> (future)
  CMD_CONFIG,    // C<command> (future)
  CMD_UNKNOWN
};

struct Command {
  CommandType type;
  int pin;
  int value;
  String raw;
  bool valid;
};

class CommandParser {
public:
  /**
   * Parse a command string
   * @param commandStr The command string (e.g., "D2:1")
   * @return Parsed Command struct
   */
  static Command parse(String commandStr) {
    Command cmd;
    cmd.raw = commandStr;
    cmd.valid = false;

    commandStr.trim();

    if (commandStr.length() < 3) {
      cmd.type = CMD_UNKNOWN;
      return cmd;
    }

    char prefix = commandStr.charAt(0);
    int colonIndex = commandStr.indexOf(':');

    if (colonIndex < 0) {
      cmd.type = CMD_UNKNOWN;
      return cmd;
    }

    // Extract pin number
    String pinStr = commandStr.substring(1, colonIndex);
    cmd.pin = pinStr.toInt();

    // Extract value
    String valueStr = commandStr.substring(colonIndex + 1);
    cmd.value = valueStr.toInt();

    // Determine command type
    switch (prefix) {
      case 'D':
      case 'd':
        cmd.type = CMD_DIGITAL;
        cmd.valid = true;
        break;

      case 'A':
      case 'a':
        cmd.type = CMD_ANALOG;
        cmd.valid = true;
        // Constrain analog value to 0-255
        cmd.value = constrain(cmd.value, 0, 255);
        break;

      case 'R':
      case 'r':
        cmd.type = CMD_READ;
        cmd.valid = true;
        break;

      case 'S':
      case 's':
        cmd.type = CMD_SERVO;
        cmd.valid = true;
        // Constrain servo angle to 0-180
        cmd.value = constrain(cmd.value, 0, 180);
        break;

      case 'C':
      case 'c':
        cmd.type = CMD_CONFIG;
        cmd.valid = true;
        break;

      default:
        cmd.type = CMD_UNKNOWN;
        break;
    }

    return cmd;
  }

  /**
   * Execute a digital command
   * @param cmd The parsed command
   */
  static void executeDigital(const Command& cmd) {
    if (!cmd.valid || cmd.type != CMD_DIGITAL) {
      return;
    }

    pinMode(cmd.pin, OUTPUT);
    digitalWrite(cmd.pin, cmd.value ? HIGH : LOW);

    #ifdef DEBUG_SERIAL
    Serial.print("Digital Pin ");
    Serial.print(cmd.pin);
    Serial.print(" set to ");
    Serial.println(cmd.value ? "HIGH" : "LOW");
    #endif
  }

  /**
   * Execute an analog/PWM command
   * @param cmd The parsed command
   */
  static void executeAnalog(const Command& cmd) {
    if (!cmd.valid || cmd.type != CMD_ANALOG) {
      return;
    }

    pinMode(cmd.pin, OUTPUT);
    analogWrite(cmd.pin, cmd.value);

    #ifdef DEBUG_SERIAL
    Serial.print("Analog Pin ");
    Serial.print(cmd.pin);
    Serial.print(" set to ");
    Serial.println(cmd.value);
    #endif
  }

  /**
   * Get command type as string
   * @param type The command type
   * @return String representation
   */
  static String getTypeName(CommandType type) {
    switch (type) {
      case CMD_DIGITAL: return "Digital";
      case CMD_ANALOG: return "Analog";
      case CMD_READ: return "Read";
      case CMD_SERVO: return "Servo";
      case CMD_CONFIG: return "Config";
      default: return "Unknown";
    }
  }
};

#endif // COMMAND_PARSER_H
