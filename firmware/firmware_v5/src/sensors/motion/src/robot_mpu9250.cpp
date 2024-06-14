#include <Wire.h>

#include "robot_mpu9250.h"
#include "config/pins.h"
#include "MPU9250.h"
#include <cmath> // for atan2 and M_PI

MPU9250 mpu;

SW_MPU9250::SW_MPU9250(bool useCalibration)
{
    // TODO: useCalibration config
}

void SW_MPU9250::begin()
{
    Wire.begin();
    delay(2000);
    if (!mpu.setup(I2C_ADDRESS_MPU9250))
    { // change to your own address
        while (1)
        {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }

    Serial.println(">> Motion\t:enabled");
}

void SW_MPU9250::calibrate()
{
    // calibrate anytime you want to
    Serial.println("Accel Gyro calibration will start in 5sec.");
    Serial.println("Please leave the device still on the flat plane.");
    mpu.verbose(true);
    delay(5000);
    mpu.calibrateAccelGyro();

    Serial.println("Mag calibration will start in 5sec.");
    Serial.println("Please Wave device in a figure eight until done.");
    delay(5000);
    mpu.calibrateMag();

    print_calibration();
    mpu.verbose(false);
}

void SW_MPU9250::read()
{
}

void SW_MPU9250::test()
{
    if (mpu.update())
    {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 25)
        {
            print_roll_pitch_yaw();
            prev_ms = millis();
        }
    }
}

void SW_MPU9250::print_calibration()
{
    Serial.println("< calibration parameters >");
    Serial.println("accel bias [g]: ");
    Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.println();
    Serial.println("gyro bias [deg/s]: ");
    Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.println();
    Serial.println("mag bias [mG]: ");
    Serial.print(mpu.getMagBiasX());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasY());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasZ());
    Serial.println();
    Serial.println("mag scale []: ");
    Serial.print(mpu.getMagScaleX());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());
    Serial.println();
}

void SW_MPU9250::print_roll_pitch_yaw()
{
    Serial.printf("%8.2f %8.2f %8.2f\n", mpu.getYaw(), mpu.getPitch(), mpu.getRoll());
}

float SW_MPU9250::get_yaw()
{
    return mpu.getYaw();
}

float SW_MPU9250::get_heading()
{
    double angle = atan2(mpu.getMagX(), mpu.getMagY());
    double angleInDegrees = angle * 180 / M_PI;
    return angleInDegrees;
}

float SW_MPU9250::update()
{
    mpu.update();
}