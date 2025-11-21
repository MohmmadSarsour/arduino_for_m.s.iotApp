#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  if(!SerialBT.begin("ESP32_LED", true)){
    Serial.println("❌ فشل تهيئة البلوتوث!");
    while(1);
  }

  SerialBT.setPin("1234", 4); // التعديل هنا

  Serial.println("✅ البلوتوث جاهز، يمكنك الاتصال الآن...");
}

void loop() {
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    command.trim();

    Serial.print("📩 تم استلام الأمر: ");
    Serial.println(command);

    if (command == "D2:1") {
      digitalWrite(ledPin, HIGH);
      SerialBT.println("💡 LED ON");
      Serial.println("LED ON");
    }
    else if (command == "D2:0") {
      digitalWrite(ledPin, LOW);
      SerialBT.println("💤 LED OFF");
      Serial.println("LED OFF");
    }
    else {
      SerialBT.println("❓ أمر غير معروف");
    }
  }

  delay(20);
}
