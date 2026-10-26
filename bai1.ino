#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);

  SerialBT.begin("ESP32_BT");

  Serial.println("ESP32 da khoi dong Bluetooth!");
  Serial.println("Ket noi bang dien thoai.");
}

void loop() {
  if (SerialBT.available()) {
    String tinNhan = SerialBT.readStringUntil('\n');

    Serial.print("Dien thoai: ");
    Serial.println(tinNhan);

    SerialBT.print("ESP32 da nhan: ");
    SerialBT.println(tinNhan);
  }

  if (Serial.available()) {
    String tinNhan = Serial.readStringUntil('\n');
    SerialBT.println(tinNhan);
  }
}
