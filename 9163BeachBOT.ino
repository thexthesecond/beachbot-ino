#include <TomIBT2.h>
#include "BluetoothSerial.h"

#define L_R_EN 27
#define L_L_EN 26
#define L_RPWM 33
#define L_LPWM 32

String device_name = "ESP32-BT-Slave";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
TomIBT2 LMOTOR(L_R_EN, L_L_EN, L_RPWM, L_LPWM);

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  LMOTOR.begin();
}

void loop() {
  if (SerialBT.available()) {
    char input = SerialBT.read();

    digitalWrite(L_R_EN, HIGH);
    digitalWrite(L_L_EN, HIGH);

    if (input == 'F') {
      Serial.println("Forward");

      LMOTOR.rotate(100, TomIBT2::CCW);
    }

    if (input == 'B') {
      Serial.println("Back");
      LMOTOR.rotate(130, TomIBT2::CW);
    }

    if (input == '0') {
      Serial.println("Stop");
      LMOTOR.brake();
    }
  }

  delay(20); 
}
