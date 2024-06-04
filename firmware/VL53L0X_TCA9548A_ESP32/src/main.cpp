#include "Adafruit_VL53L0X.h"
#include <Wire.h>

Adafruit_VL53L0X lox0 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();


// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  Serial.print(bus);
}


void printValues(Adafruit_VL53L0X lox, int bus) {
  TCA9548A (bus);
  // Serial.print("Sensor number on bus");
  // Serial.println(bus);

  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100);
}


void setup() {
  Serial.begin(115200);

  // Start I2C communication with the Multiplexer
  Wire.begin();

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  // Init sensor on bus number 0
  TCA9548A(0);
  Serial.println("Adafruit VL53L0X test 0");
  if (!lox0.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  Serial.println();
  
  // Init sensor on bus number 1
  TCA9548A(1);
  Serial.println("Adafruit VL53L0X test 1");
  if (!lox1.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  Serial.println();

  // Init sensor on bus number 2
  TCA9548A(2);
  Serial.println("Adafruit VL53L0X test 2");
  if (!lox2.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  Serial.println();

  // Init sensor on bus number 3
  TCA9548A(3);
  Serial.println("Adafruit VL53L0X test 3");
  if (!lox3.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  Serial.println(); 
  
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  //Print values for sensors
  printValues(lox0, 0);
  printValues(lox1, 1);
  printValues(lox2, 2);
  printValues(lox3, 3);
  Serial.println();
  delay(1000);
}
