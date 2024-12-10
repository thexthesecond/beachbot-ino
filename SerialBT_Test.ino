#include "BluetoothSerial.h"
#include <TomIBT2.h>

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

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif   // PWM 490.20Hz

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  motorL.begin();
  motorR.begin();
}

void loop() {

  String input;

  if (!SerialBT.hasClient()) {
    ForceStop(); // Metódo que para os motores se a conexão for perdida com o CLIENT.
  }

  if (SerialBT.available()) {
    input = SerialBT.readString();
    input.trim();

    robotIO(input);
  }
  
  delay(20);
}

// Método que manda mensagem tanto para o Serial quanto para o Terminal
void GlobalPrint(String line) {
  SerialBT.println(line);
  Serial.println(line);
}

//Input/Output do Serial
void robotIO(String input) {
  
  if(input.equals("F")) {
    GlobalPrint("FORWARD");
    motorL.rotate(255, TomIBT2::CW);
    motorR.rotate(255, TomIBT2::CW);
  }

  if(input.equals("B")) {
    GlobalPrint("BACKWARDS");
    motorL.rotate(255, TomIBT2::CCW);
    motorR.rotate(255, TomIBT2::CCW);
  }

  if(input.equals("L")) {
    GlobalPrint("LEFT");
    motorL.rotate(255, TomIBT2::CCW);
    motorR.rotate(255, TomIBT2::CW);
  }

  if(input.equals("R")) {
    GlobalPrint("RIGHT");
    motorL.rotate(255, TomIBT2::CW);
    motorR.rotate(255, TomIBT2::CCW);
  }

  if(input.equals("P")) {
    GlobalPrint("STOPPED");
    motorL.rampDown(2000);
    motorR.rampDown(2000);
  }
}
