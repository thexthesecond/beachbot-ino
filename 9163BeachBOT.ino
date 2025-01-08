#include <TomIBT2.h>
#include "BluetoothSerial.h"

#define LMOTOR_PIN_R_EN 25
#define LMOTOR_PIN_L_EN 26
#define LMOTOR_PIN_RPWM 33
#define LMOTOR_PIN_LPWM 32

#define RMOTOR_PIN_R_EN 16
#define RMOTOR_PIN_L_EN 5
#define RMOTOR_PIN_RPWM 18
#define RMOTOR_PIN_LPWM 19

String device_name = "ESP32-BT-Slave";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

TomIBT2 L_MOTOR(LMOTOR_PIN_R_EN, LMOTOR_PIN_L_EN, LMOTOR_PIN_RPWM, LMOTOR_PIN_LPWM);
TomIBT2 R_MOTOR(RMOTOR_PIN_R_EN, RMOTOR_PIN_L_EN, RMOTOR_PIN_RPWM, RMOTOR_PIN_LPWM);

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  StartMotors();
}

void loop() {

  if (!SerialBT.hasClient()) {
    R_MOTOR.brake();
    L_MOTOR.brake();
  }

  if (SerialBT.available()) {
    char input = SerialBT.read();

    if (input == 'A') {
      StartMotors();
    }
    if (input == 'P') {
      StopMotors();
    }
    RobotIO(input);
  }
  delay(20);
}

void Drive(int speed, TomIBT2::Direction LeftDir, TomIBT2::Direction RightDir) {
  L_MOTOR.rotate(speed, LeftDir);
  R_MOTOR.rotate(speed, RightDir);
}

void StartMotors() {
  L_MOTOR.begin();
  R_MOTOR.begin();
}

void StopMotors() {
  R_MOTOR.stop();
  L_MOTOR.stop();
}

void RobotIO(char input) {
  bool StrainControl;

  if (input == 'F') {
    Serial.println("FORWARD");
    Drive(127, TomIBT2::CW, TomIBT2::CCW);
  }

  if (input == 'B') {
    Serial.println("BACKWARDS");
    Drive(127, TomIBT2::CCW, TomIBT2::CW);
  }

  if (input == 'L') {
    Serial.println("LEFT");
    Drive(127, TomIBT2::CW, TomIBT2::CW);
  }

  if (input == 'R') {
    Serial.println("RIGHT");
    Drive(127, TomIBT2::CCW, TomIBT2::CCW);
  }

  if (input == 'T') {
    StrainControl = true;
    Serial.println("STRAIN ON");
  }

  if (input == 'S') {
    StrainControl = false;
    Serial.println("STRAIN OFF");
  }

  if (input == '0') {
    Serial.println("STOP");
    L_MOTOR.brake();
    R_MOTOR.brake();
  }
}
