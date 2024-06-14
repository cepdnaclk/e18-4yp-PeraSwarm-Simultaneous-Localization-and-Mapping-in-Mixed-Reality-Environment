/**
 * @brief    Swarm Robot Motor Controller Library
 * @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
 * @version  2.0.0
 * @url      N/A
 *
 * ------------------------------------------------------------------------------
 */

#ifndef SW_MOTORS_H
#define SW_MOTORS_H

#include <Arduino.h>

#include "config/definitions.h"
#include "robot_encoder.h"

#define MAX_MOTOR_SPEED 255
#define MIN_MOTOR_SPEED 30

struct EncoderReadings
{
    int left;
    int right;
};

class SW_Motors
{
private:
    uint8_t RIGHT_DEFAULT = 90;
    uint8_t LEFT_DEFAULT = 90;

    boolean leftMotorDir = 1, rightMotorDir = 1;
    boolean leftMotorDirOld = 0, rightMotorDirOld = 0;
    int16_t rightMotorSpeed = 0, leftMotorSpeed = 0;

    RobotEncoder encoderL;
    RobotEncoder encoderR;

public:
    SW_Motors();
    ~SW_Motors();

    int8_t leftCorrection = 0;
    int8_t rightCorrection = 0;

    void begin();
    void write(int16_t left, int16_t right);
    void stop();
    void stop(int16_t delay);

    void pause();
    void resume();

    void test();

    void encoder_print();
    float distance_print();
    void encoder_reset();
    void encoder_read(int32_t *left, int32_t *right);
    EncoderReadings encoder_read();
    void writeLeftMotor(int16_t speed);
    void writeRightMotor(int16_t speed);
    void stopLeftMotor();
    void stopRightMotor();
};

#endif
