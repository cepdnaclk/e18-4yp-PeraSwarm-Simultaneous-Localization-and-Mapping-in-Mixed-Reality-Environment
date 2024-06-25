#pragma once

#include "Adafruit_VL53L0X.h"
#include "config/pins.h"


extern Adafruit_VL53L0X lox0;
extern Adafruit_VL53L0X lox1;
extern Adafruit_VL53L0X lox2;
extern Adafruit_VL53L0X lox3;

void setTCA9548A(uint8_t bus);
void setVL53L0X(Adafruit_VL53L0X lox, int bus);
void checkI2CSensors();

int getDistanceVal(Adafruit_VL53L0X lox, int bus);
