/**
 * @brief    Swarm Robot Memory Manager Library
 * @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
 * @version  1.0.0
 * @url      N/A
 *
 * ------------------------------------------------------------------------------
 */

#include <Arduino.h>
#include "EEPROM.h"
#include "robot_memory.h"

#include "config/pins.h"
#include "config/definitions.h"

#include "MPU9250.h"
const uint8_t EEPROM_SIZE = 1 + sizeof(float) * 3 * 4 + 10;
extern MPU9250 mpu;


#define MIN_ROBOT_ID 0
#define MAX_ROBOT_ID 31
// #define EEPROM_SIZE 10






enum EEP_ADDR {
    EEP_CALIB_FLAG = 0x00,
    EEP_ACC_BIAS = 0x01,
    EEP_GYRO_BIAS = 0x0D,
    EEP_MAG_BIAS = 0x19,
    EEP_MAG_SCALE = 0x25
};

void SW_Memory::writeByte(int address, byte value) {
    EEPROM.put(address, value);
}

void SW_Memory::writeFloat(int address, float value) {
    EEPROM.put(address, value);
}

byte SW_Memory::readByte(int address) {
    byte valueIn = 0;
    EEPROM.get(address, valueIn);
    return valueIn;
}

float SW_Memory::readFloat(int address) {
    float valueIn = 0;
    EEPROM.get(address, valueIn);
    return valueIn;
}

void SW_Memory::clearCalibration() {
    writeByte(EEP_CALIB_FLAG, 0);
}

bool SW_Memory::isCalibrated() {
    return (readByte(EEP_CALIB_FLAG) == 0x01);
}

void SW_Memory::saveCalibration() {
    Serial.println("Write calibrated parameters to EEPROM");
    writeByte(EEP_CALIB_FLAG, 1);
    writeFloat(EEP_ACC_BIAS + 0, mpu.getAccBias(0));
    writeFloat(EEP_ACC_BIAS + 4, mpu.getAccBias(1));
    writeFloat(EEP_ACC_BIAS + 8, mpu.getAccBias(2));
    writeFloat(EEP_GYRO_BIAS + 0, mpu.getGyroBias(0));
    writeFloat(EEP_GYRO_BIAS + 4, mpu.getGyroBias(1));
    writeFloat(EEP_GYRO_BIAS + 8, mpu.getGyroBias(2));
    writeFloat(EEP_MAG_BIAS + 0, mpu.getMagBias(0));
    writeFloat(EEP_MAG_BIAS + 4, mpu.getMagBias(1));
    writeFloat(EEP_MAG_BIAS + 8, mpu.getMagBias(2));
    writeFloat(EEP_MAG_SCALE + 0, mpu.getMagScale(0));
    writeFloat(EEP_MAG_SCALE + 4, mpu.getMagScale(1));
    writeFloat(EEP_MAG_SCALE + 8, mpu.getMagScale(2));
#if defined(ESP_PLATFORM) || defined(ESP8266)
    EEPROM.commit();
#endif
}

void SW_Memory::loadCalibration() {
    Serial.println("Load calibrated parameters from EEPROM");
    if (isCalibrated()) {
        Serial.println("calibrated? : YES");
        Serial.println("load calibrated values");
        mpu.setAccBias(
            readFloat(EEP_ACC_BIAS + 0),
            readFloat(EEP_ACC_BIAS + 4),
            readFloat(EEP_ACC_BIAS + 8));
        mpu.setGyroBias(
            readFloat(EEP_GYRO_BIAS + 0),
            readFloat(EEP_GYRO_BIAS + 4),
            readFloat(EEP_GYRO_BIAS + 8));
        mpu.setMagBias(
            readFloat(EEP_MAG_BIAS + 0),
            readFloat(EEP_MAG_BIAS + 4),
            readFloat(EEP_MAG_BIAS + 8));
        mpu.setMagScale(
            readFloat(EEP_MAG_SCALE + 0),
            readFloat(EEP_MAG_SCALE + 4),
            readFloat(EEP_MAG_SCALE + 8));
    } else {
        Serial.println("calibrated? : NO");
        Serial.println("load default values");
        mpu.setAccBias(0., 0., 0.);
        mpu.setGyroBias(0., 0., 0.);
        mpu.setMagBias(0., 0., 0.);
        mpu.setMagScale(1., 1., 1.);
    }
}

void SW_Memory::printCalibration() {
    Serial.println("< calibration parameters >");
    Serial.print("calibrated? : ");
    Serial.println(readByte(EEP_CALIB_FLAG) ? "YES" : "NO");
    Serial.print("acc bias x  : ");
    Serial.println(readFloat(EEP_ACC_BIAS + 0) * 1000.f / MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print("acc bias y  : ");
    Serial.println(readFloat(EEP_ACC_BIAS + 4) * 1000.f / MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print("acc bias z  : ");
    Serial.println(readFloat(EEP_ACC_BIAS + 8) * 1000.f / MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print("gyro bias x : ");
    Serial.println(readFloat(EEP_GYRO_BIAS + 0) / MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print("gyro bias y : ");
    Serial.println(readFloat(EEP_GYRO_BIAS + 4) / MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print("gyro bias z : ");
    Serial.println(readFloat(EEP_GYRO_BIAS + 8) / MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print("mag bias x  : ");
    Serial.println(readFloat(EEP_MAG_BIAS + 0));
    Serial.print("mag bias y  : ");
    Serial.println(readFloat(EEP_MAG_BIAS + 4));
    Serial.print("mag bias z  : ");
    Serial.println(readFloat(EEP_MAG_BIAS + 8));
    Serial.print("mag scale x : ");
    Serial.println(readFloat(EEP_MAG_SCALE + 0));
    Serial.print("mag scale y : ");
    Serial.println(readFloat(EEP_MAG_SCALE + 4));
    Serial.print("mag scale z : ");
    Serial.println(readFloat(EEP_MAG_SCALE + 8));
}

void SW_Memory::printBytes() {
    for (size_t i = 0; i < EEPROM_SIZE; ++i)
        Serial.println(readByte(i), HEX);
}

void SW_Memory::setupEEPROM() {
    Serial.println("EEPROM start");

    if (!isCalibrated()) {
        Serial.println("Need Calibration!!");
    }
    Serial.println("EEPROM calibration value is : ");
    printCalibration();
    Serial.println("Loaded calibration value is : ");
    loadCalibration();
}




















SW_Memory::SW_Memory()
{ // Memory constructor
}

void SW_Memory::begin()
{
    this->displayMemoryDetails();
    EEPROM.begin(EEPROM_SIZE);
    Serial.println(F(">> Memory\t:enabled"));
}

void SW_Memory::displayMemoryDetails()
{
    Serial.printf("\n\nThe program uses %d kB and has free space of %d kB\n\n", ESP.getSketchSize() / 1000, ESP.getFreeSketchSpace() / 1000);
}

int SW_Memory::getRobotId()
{ // TODO: implement a variable as cache for reduce the number of memory read occurances
    return (int)this->read(EEPROM_ROBOT_ID);
}

boolean SW_Memory::getMemoryStatus()
{
    return (this->read(EEPROM_PROGRAMMED) == 1);
}

uint8_t SW_Memory::read(int adr)
{
    return EEPROM.read(adr);
}
void SW_Memory::write(uint16_t address, uint8_t data)
{ // address: [0-511], data: [0-255]
    EEPROM.write(address, data);
    delay(200);
    EEPROM.commit();
}
void SW_Memory::test()
{ // A dummy Function for now
}

void SW_Memory::setupRobotWithId(uint8_t id)
{
    this->setRobotId(id);
}

void SW_Memory::setRobotId(uint8_t id)
{
    if (id >= MIN_ROBOT_ID && id <= MAX_ROBOT_ID)
    {
        EEPROM.write(EEPROM_ROBOT_ID, id);
        delay(200);
        EEPROM.write(EEPROM_PROGRAMMED, 1);
        delay(200);
        EEPROM.commit();
        Serial.printf("Mem_Done\t: %d is written as Robot Id\n", id);
    }
    else
    {
        Serial.printf("Mem_Error\t: %d is an invalid Robot Id\n", id);
    }
}