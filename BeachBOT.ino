#include <TomIBT2.h>
#include "BluetoothSerial.h"

#define LMOTOR_PIN_R_EN 25
#define LMOTOR_PIN_L_EN 35
#define LMOTOR_PIN_RPWM 33
#define LMOTOR_PIN_LPWM 32

#define RMOTOR_PIN_R_EN 16
#define RMOTOR_PIN_L_EN 5
#define RMOTOR_PIN_RPWM 18
#define RMOTOR_PIN_LPWM 19

String device_name = "BeachBOT-ESP32";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

TomIBT2 L_MOTOR(LMOTOR_PIN_R_EN, LMOTOR_PIN_L_EN, LMOTOR_PIN_RPWM, LMOTOR_PIN_LPWM);
TomIBT2 R_MOTOR(RMOTOR_PIN_R_EN, RMOTOR_PIN_L_EN, RMOTOR_PIN_RPWM, RMOTOR_PIN_LPWM);

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  L_MOTOR.begin();
  R_MOTOR.begin();
}

void loop() {
  if (!SerialBT.hasClient()) {
    L_MOTOR.brake();
    R_MOTOR.brake();
  }

  if (SerialBT.available()) {
    char input = SerialBT.read();
    bool strainControl;

    RobotIO(input, strainControl);
  }
  delay(20);
}

void RobotIO(char input, bool strain) {

  if (input == 'F') {
    Serial.println("FORWARD");
    L_MOTOR.rotate(255, TomIBT2::CW);
    R_MOTOR.rotate(255, TomIBT2::CCW);
  }

  if (input == 'B') {
    Serial.println("BACKWARDS");
    L_MOTOR.rotate(255, TomIBT2::CCW);
    R_MOTOR.rotate(255, TomIBT2::CW);
  }

  if (input == 'L') {
    Serial.println("LEFT");
    L_MOTOR.rotate(255, TomIBT2::CCW);
    R_MOTOR.rotate(255, TomIBT2::CCW);
  }
  if (input == 'R') {
    Serial.println("RIGHT");
    L_MOTOR.rotate(255, TomIBT2::CW);
    R_MOTOR.rotate(255, TomIBT2::CW);
  }

  if (input == 'T') {
    strain = true;
    Serial.println("Strain rotating");
  }

  if (input == 'S') {
    strain = false;
    Serial.println("Strain stopped");
  }

  if (input == '0') {
    Serial.println("STOPPED");
    L_MOTOR.brake();
    R_MOTOR.brake();
  }
}
