#ifndef MPU9250_h
#define MPU9250_h

#include <Arduino.h>

class SW_MPU9250
{
public:
    SW_MPU9250(bool useCalibration);

    void begin();

    // TODO
    void read();

    void calibrate();
    void test();
    float get_yaw();

    float get_heading();

    float update();

private:
    void print_calibration();
    void print_roll_pitch_yaw();
};

#endif