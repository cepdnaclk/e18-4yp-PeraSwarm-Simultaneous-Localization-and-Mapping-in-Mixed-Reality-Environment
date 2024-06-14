/**
 * @brief    Swarm Robot Motor Controller Library
 * @author   Nuwan Jaliyagoda
 * @version  2.0.0
 * @url      N/A
 *
 * ------------------------------------------------------------------------------
 */

#include "robot_motors.h"
#include "config/pins.h"

#define LEDC_RESOLUTION_BITS 8
#define LEDC_BASE_FREQ 5000
#define LEDC_CHANNEL_A 8
#define LEDC_CHANNEL_B 9

/*
 * Motor module
 */
SW_Motors::SW_Motors()
{
    encoderL.setCount(0);
    encoderL.attachHalfQuad(PIN_ENC_LA, PIN_ENC_LB);

    encoderR.setCount(0);
    encoderR.attachHalfQuad(PIN_ENC_RA, PIN_ENC_RB);
}

SW_Motors::~SW_Motors()
{
    ledcDetachPin(PIN_PWM_R);
    ledcDetachPin(PIN_PWM_L);
}

/**
 * Setup the motor module
 */
void SW_Motors::begin()
{
    // TODO: Try drive the robot using 1 directional pin per motor
    pinMode(PIN_MOT_R1, OUTPUT);
    pinMode(PIN_MOT_R2, OUTPUT);
    pinMode(PIN_MOT_L1, OUTPUT);
    pinMode(PIN_MOT_L2, OUTPUT);

    ledcSetup(LEDC_CHANNEL_A, LEDC_BASE_FREQ, LEDC_RESOLUTION_BITS);
    ledcSetup(LEDC_CHANNEL_B, LEDC_BASE_FREQ, LEDC_RESOLUTION_BITS);

    ledcAttachPin(PIN_PWM_R, LEDC_CHANNEL_A);
    ledcAttachPin(PIN_PWM_L, LEDC_CHANNEL_B);

    ledcWrite(LEDC_CHANNEL_A, 0);
    ledcWrite(LEDC_CHANNEL_B, 0);

    Serial.println(">> Motors\t:enabled,pwm");

    this->write(0, 0);
}

/**
 * Core motor controlling function
 * @param leftSpeed speed in range [-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED]
 * @param rightSpeed speed in range [-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED]
 */
void SW_Motors::write(int16_t leftSpeed, int16_t rightSpeed)
{
    // Adjustment to remove the drift
    // TODO implement a linear correction
    if (leftSpeed > 30)
        leftSpeed += leftCorrection;
    else if (leftSpeed < -30)
        leftSpeed -= leftCorrection;

    if (rightSpeed > 30)
        rightSpeed += rightCorrection;
    else if (rightSpeed < -30)
        rightSpeed -= rightCorrection;

    // map the speed with correct & acceptable range
    leftSpeed = constrain(leftSpeed, -1 * MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
    rightSpeed = constrain(rightSpeed, -1 * MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    // Serial.printf("M: %d %d\n", leftSpeed, rightSpeed);

    // motor rotating directions
    this->leftMotorDir = (leftSpeed >= 0) ? 1 : 0;
    this->rightMotorDir = (rightSpeed >= 0) ? 1 : 0;

    // check motor directions
    if (this->leftMotorDir != this->leftMotorDirOld)
    {
        // Direction changed
        digitalWrite(PIN_MOT_L1, (this->leftMotorDir) ? LOW : HIGH);
        digitalWrite(PIN_MOT_L2, (this->leftMotorDir) ? HIGH : LOW);
        this->leftMotorDirOld = this->leftMotorDir;
    }
    if (this->rightMotorDir != this->rightMotorDirOld)
    {
        // Direction changed
        digitalWrite(PIN_MOT_R1, (this->rightMotorDir) ? LOW : HIGH);
        digitalWrite(PIN_MOT_R2, (this->rightMotorDir) ? HIGH : LOW);
        this->rightMotorDirOld = this->rightMotorDir;
    }

    this->rightMotorSpeed = abs(rightSpeed);
    this->leftMotorSpeed = abs(leftSpeed);

    // Analog write function for ESP32
    ledcWrite(LEDC_CHANNEL_A, this->rightMotorSpeed);
    ledcWrite(LEDC_CHANNEL_B, this->leftMotorSpeed);
}

/**
 * Stop motors immediately
 */
void SW_Motors::stop()
{
    this->write(0, 0);
}

/**
 * Stop motors after a delay
 * @param delay delay in milliseconds
 */
void SW_Motors::stop(int16_t d)
{
    delay(d);
    this->write(0, 0);
}

/**
 * Pause motors for while
 */
void SW_Motors::pause()
{
    ledcWrite(LEDC_CHANNEL_A, 0);
    ledcWrite(LEDC_CHANNEL_B, 0);
}

/**
 * Resume motors
 */
void SW_Motors::resume()
{
    ledcWrite(LEDC_CHANNEL_A, this->leftMotorSpeed);
    ledcWrite(LEDC_CHANNEL_B, this->rightMotorSpeed);
}

/**
 * A test function to test motors
 */
void SW_Motors::test()
{
    // Clockwise rotation
    Serial.println(F("robot: CCW"));
    this->write(-200, 200);
    delay(500);
    this->stop(1500);

    // Counter Clockwise rotation
    Serial.println(F("robot: CW"));
    this->write(200, -200);
    delay(500);
    this->stop(1500);

    // Forward movement
    Serial.println(F("robot: forward++"));
    for (int i = 0; i < 255; i++)
    {
        this->write(i, i);
        delay(25);
    }
    delay(500);

    Serial.println(F("robot: forward--"));
    for (int i = 255; i > 0; i--)
    {
        this->write(i, i);
        delay(25);
    }
    this->stop(500);
    delay(2000);

    // Backward movement
    Serial.println(F("robot: backward++"));
    for (int i = 0; i < 255; i++)
    {
        this->write(-i, -i);
        delay(25);
    }
    delay(500);

    Serial.println(F("robot: backward--"));
    for (int i = 255; i > 0; i--)
    {
        this->write(-i, -i);
        delay(25);
    }
    this->stop(500);
    delay(2000);
}

/**
 * Print the encoder readings
 */
void SW_Motors::encoder_print()
{
    int32_t countL = encoderL.getCount();
    int32_t countR = encoderR.getCount();
    Serial.printf("Encoder L: %d, R: %d\n", countL, countR);
    delay(10);
}

float SW_Motors::distance_print()
{
    int32_t countL = encoderL.getCount();
    int32_t countR = encoderR.getCount();
    Serial.printf("Distance L: %f, R: %f\n", (countL/434.0)*(3.14*34), (countR/434.0)*(3.14*34));
    delay(10);
    return (countL/434.0)*(3.14*34);
}

/**
 * Reset the encoder readings to 0
 */
void SW_Motors::encoder_reset()
{
    encoderL.clearCount();
    encoderR.clearCount();
}

/**
 * Read encoder values and return as a pointers
 * @param *left Pointer variable to receive left encoder reading
 * @param *right Pointer variable to receive right encoder reading
 */
void SW_Motors::encoder_read(int32_t *left, int32_t *right)
{
    *left = encoderL.getCount();
    *right = encoderR.getCount();
}

/**
 * Read encoder values and return as a Struct
 *
 * @return EncoderReadings
 */
EncoderReadings SW_Motors::encoder_read()
{
    struct EncoderReadings readings;
    readings.left = encoderL.getCount();
    readings.right = encoderR.getCount();

    return readings;
}



/**
 * Function to control the left motor
 * @param speed speed in range [-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED]
 */
void SW_Motors::writeLeftMotor(int16_t speed)
{
    // Adjustment to remove the drift
    if (speed > 30)
        speed += leftCorrection;
    else if (speed < -30)
        speed -= leftCorrection;

    // map the speed with correct & acceptable range
    speed = constrain(speed, -1 * MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    // Set motor rotating direction
    bool motorDir = (speed >= 0) ? 1 : 0;

    digitalWrite(PIN_MOT_L1, motorDir ? LOW : HIGH);
    digitalWrite(PIN_MOT_L2, motorDir ? HIGH : LOW);

    // Set motor speed
    this->leftMotorSpeed = abs(speed);

    // Analog write function for ESP32
    ledcWrite(LEDC_CHANNEL_A, this->leftMotorSpeed);
}

/**
 * Function to control the right motor
 * @param speed speed in range [-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED]
 */
void SW_Motors::writeRightMotor(int16_t speed)
{
    // Adjustment to remove the drift
    if (speed > 30)
        speed += rightCorrection;
    else if (speed < -30)
        speed -= rightCorrection;

    // map the speed with correct & acceptable range
    speed = constrain(speed, -1 * MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    // Set motor rotating direction
    bool motorDir = (speed >= 0) ? 1 : 0;

    digitalWrite(PIN_MOT_R1, motorDir ? LOW : HIGH);
    digitalWrite(PIN_MOT_R2, motorDir ? HIGH : LOW);

    // Set motor speed
    this->rightMotorSpeed = abs(speed);

    // Analog write function for ESP32
    ledcWrite(LEDC_CHANNEL_B, this->rightMotorSpeed);
}


/**
 * Function to stop the left motor immediately
 */
void SW_Motors::stopLeftMotor()
{
    // Stop the left motor by setting its speed to 0
    this->writeLeftMotor(0);
}

/**
 * Function to stop the right motor immediately
 */
void SW_Motors::stopRightMotor()
{
    // Stop the right motor by setting its speed to 0
    this->writeRightMotor(0);
}
