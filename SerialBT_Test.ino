#include <TomIBT2.h>
#include "BluetoothSerial.h"

#define LMOTOR_PIN_R_EN 4
#define LMOTOR_PIN_L_EN 5
#define LMOTOR_PIN_RPWM 6
#define LMOTOR_PIN_LPWM 7

#define RMOTOR_PIN_R_EN 8
#define RMOTOR_PIN_L_EN 9
#define RMOTOR_PIN_RPWM 10
#define RMOTOR_PIN_LPWM 11

TomIBT2 motorL(LMOTOR_PIN_R_EN, LMOTOR_PIN_L_EN, LMOTOR_PIN_RPWM, LMOTOR_PIN_LPWM);
TomIBT2 motorR(RMOTOR_PIN_R_EN, RMOTOR_PIN_L_EN, RMOTOR_PIN_RPWM, RMOTOR_PIN_LPWM);

String device_name = "ESP32-BT-Slave";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
}

void loop() {
  bool strainControl;
  char input;
  
  if (SerialBT.available()) {
    input = SerialBT.read();

    RobotIO(input, strainControl);
  }

  delay(20);
}

void RobotIO(char input, bool strain) {
  if (input == 'F') {
    Serial.println("FORWARD");
  }

  if (input == 'R') {
    Serial.println("RIGHT");
  }

  if (input == 'L') {
    Serial.println("LEFT");
  }

  if (input == 'B') {
    Serial.println("BACK");
  }

  if (input == 'P') {
    Serial.println("STOPPED");
  }

  if (input == 'O') {
    Serial.println("STRAIN ON");
    strain = true;
  }

  if (input == 'X') {
    Serial.println("STRAIN OFF");
    strain = false;
  }
}
