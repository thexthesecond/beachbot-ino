#include <TomIBT2.h>
#include "BluetoothSerial.h"

//Definicao dos pinos
#define RMOTOR_PIN_R_EN 25
#define RMOTOR_PIN_L_EN 26
#define RMOTOR_PIN_RPWM 33
#define RMOTOR_PIN_LPWM 32

#define LMOTOR_PIN_R_EN 16
#define LMOTOR_PIN_L_EN 5
#define LMOTOR_PIN_RPWM 18
#define LMOTOR_PIN_LPWM 19

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

const int speed = 127;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  L_MOTOR.begin();
  R_MOTOR.begin();
}

void loop() {
  if (!SerialBT.hasClient()) {
    R_MOTOR.brake();
    L_MOTOR.brake();
  }

  if (SerialBT.available()) {
    char input = SerialBT.read();

    RobotIO(input);
  }
  delay(20);
}

// Metodo de controle de motores, para simplificar o codigo
void Drive(TomIBT2::Direction LeftDir, TomIBT2::Direction RightDir) {
  L_MOTOR.rotate(speed, LeftDir);
  R_MOTOR.rotate(speed, RightDir);
}


// Metodo de input/output do Serial
void RobotIO(char input) {
  if (input == 'F') {
    Serial.println("FORWARD");
    Drive(TomIBT2::CW, TomIBT2::CCW);
  }

  if (input == 'B') {
    Serial.println("BACKWARDS");
    Drive(TomIBT2::CCW, TomIBT2::CW);
  }

  if (input == 'L') {
    Serial.println("LEFT");
    Drive(TomIBT2::CW, TomIBT2::CW);
  }

  if (input == 'R') {
    Serial.println("RIGHT");
    Drive(TomIBT2::CCW, TomIBT2::CCW);
  }

  if (input == '0') {
    Serial.println("STOP");
    L_MOTOR.brake();
    R_MOTOR.brake();
  }
}
