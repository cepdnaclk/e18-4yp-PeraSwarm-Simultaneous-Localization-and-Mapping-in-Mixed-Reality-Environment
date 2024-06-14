/**
 * @brief    Swarm Robot Memory Manager Library
 * @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
 * @version  1.0.0
 * @url      N/A
 *
 * ------------------------------------------------------------------------------
 */

#ifndef SW_Memory_h
#define SW_Memory_h

#include <Arduino.h>

class SW_Memory
{
public:
    void writeByte(int address, byte value);
    void writeFloat(int address, float value);
    byte readByte(int address);
    float readFloat(int address);
    void clearCalibration();
    bool isCalibrated();
    void saveCalibration();
    void loadCalibration();
    void printCalibration();
    void printBytes();
    void setupEEPROM();

    SW_Memory();

    int getRobotId();
    int8_t getErrorCorrection(uint8_t id);

    void begin();
    void test();

    void setupRobotWithId(uint8_t id);

    void setRobotId(uint8_t id);

    boolean getMemoryStatus();

    // REM: Better to keep following 2 private
    uint8_t read(int adr);
    void write(uint16_t address, uint8_t data);

    enum addr
    {
        EEPROM_PROGRAMMED,
        EEPROM_ROBOT_ID
    };

private:
    void displayMemoryDetails();
};

#endif
