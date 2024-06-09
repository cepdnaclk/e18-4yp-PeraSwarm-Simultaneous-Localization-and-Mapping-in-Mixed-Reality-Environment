// this library forms part of the ESP32  Boards Manager file
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED)  || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please  run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error  Serial Bluetooth not available or not enabled. It is only available for the ESP32  chip.
#endif

// set variables and constants
BluetoothSerial SerialBT;                   // set the Object SerialBT
String text = "";                           //  define the text variable to receive BT input

float kp_turn = 0.7;
float kd_turn = 1;

void setup() {
  Serial.begin(115200);                      //  Serial Monitor
  SerialBT.begin("ESP32test");               // Bluetooth device  name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void  loop() {
  // This line sets the output from the PC to the BT device
  if  (Serial.available()) {SerialBT.write(Serial.read()); }

  // This line sets  the input from the device to the PC
  if (SerialBT.available() > 0) {
    //  stripping transfer characters to get the actual text
    text = SerialBT.readStringUntil('\n');     // get the BT input upto CRLF
    text.trim();                               //  remove whitespace

    char delimiter = ' '; // delimiter character
    int delimiterIndex = text.indexOf(delimiter); // find index of delimiter

    String parameter = text.substring(0, delimiterIndex); // get first variable
    float value = (text.substring(delimiterIndex + 1)).toFloat(); // get second variable

    // // Now you have your two variables
    // Serial.print("parameter: "); Serial.println(parameter);
    // Serial.print("value: "); Serial.println(value);

    if (parameter == "kp_turn"){ kp_turn = value; Serial.print("kp_turn: "); Serial.println(kp_turn);}       
    else if (parameter == "kd_turn"){ kd_turn = value; Serial.print("kd_turn: "); Serial.println(kd_turn);}
    
  }
}
