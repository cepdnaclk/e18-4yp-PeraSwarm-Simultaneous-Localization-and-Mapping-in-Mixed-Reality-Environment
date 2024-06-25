#include "setup_other.h"

Adafruit_VL53L0X lox0 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();

// Select I2C Bus
void setTCA9548A(uint8_t bus){
    // Serial.print("Sensor number on bus");
    Serial.print(bus);
    Serial.print("\t");
    try
    {
        Wire.beginTransmission(TCA9548A_ADDR);  // TCA9548A_ADDR = 0x70
        Wire.write(1 << bus);          // send byte to select bus
        Wire.endTransmission();
    }
    catch(int e)
    {
        Serial.println("Error in setTCA9548A");
    }
}

void setVL53L0X(Adafruit_VL53L0X lox, int bus){
    setTCA9548A (bus);
    Serial.printf("VL53L0X %d connectivity test...\n\n", bus);

    try
    {
        if (!lox.begin()) {
            Serial.printf("Failed to boot VL53L0X %d...\n", bus);
            while(1);
        }
    }
    catch(int e)
    {
        Serial.println("Error in setVL53L0X");
    }
}

void checkI2CSensors(){
    // Init sensor on bus number 0
    setVL53L0X(lox0, 0);
  
    // Init sensor on bus number 1
    setVL53L0X(lox1, 1);

    // Init sensor on bus number 2
    setVL53L0X(lox2, 2);

    // Init sensor on bus number 3
    setVL53L0X(lox3, 3);
}

int getDistanceVal(Adafruit_VL53L0X lox, int bus) {
  setTCA9548A (bus);
  VL53L0X_RangingMeasurementData_t measure;

  try
  {
    Serial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        Serial.print("Distance (mm): "); 
        Serial.println(measure.RangeMilliMeter);
    } else {
        Serial.println(" out of range ");
    }
  }
  catch(int e)
  {
    Serial.println("Error in getDistanceVal");
  }
    
  delay(100);
  return measure.RangeMilliMeter;
}
