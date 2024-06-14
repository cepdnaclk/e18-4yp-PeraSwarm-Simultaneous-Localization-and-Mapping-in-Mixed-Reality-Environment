#include <Arduino.h>
#include "define.h"

//************************************************************** */
#include "BluetoothSerial.h"
#include "Adafruit_VL53L0X.h"
#include <Wire.h>

//************************************************************** */
#if !defined(CONFIG_BT_ENABLED)  || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please  run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error  Serial Bluetooth not available or not enabled. It is only available for the ESP32  chip.
#endif

// set variables and constants
BluetoothSerial SerialBT;                   // set the Object SerialBT
String text = "";                           //  define the text variable to receive BT input

//************************************************************** */
Adafruit_VL53L0X lox0 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();

// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  // Serial.print(bus);
}

int getValue(Adafruit_VL53L0X lox, int bus) {
  TCA9548A (bus);

  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    // Serial.print("Distance (mm): "); 
    if (bus==0){Serial.print("Distance North (mm): "); SerialBT.print("Distance North (mm): ");} 
    if (bus==1){Serial.print("Distance East  (mm): "); SerialBT.print("Distance East  (mm): ");} 
    if (bus==2){Serial.print("Distance South (mm): "); SerialBT.print("Distance South (mm): ");} 
    if (bus==3){Serial.print("Distance West  (mm): "); SerialBT.print("Distance West  (mm): ");} 

    Serial.println(measure.RangeMilliMeter); SerialBT.println(measure.RangeMilliMeter);
    return measure.RangeMilliMeter;
  } else {
    Serial.println(" out of range "); SerialBT.println(" out of range ");
    return -1; // return -1 or any other value to indicate out of range
  }
}

//************************************************************** */
void vl53l0x_setup() {
  // Start I2C communication with the Multiplexer
  Wire.begin();

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  // Init sensor on bus number 0
  TCA9548A(0);
  Serial.println("VL53L0X 0 Setup");
  if (!lox0.begin()) {
    Serial.println(F("Failed to boot VL53L0X 0"));
    while(1);
  }
  Serial.println();
  
  // Init sensor on bus number 1
  TCA9548A(1);
  Serial.println("VL53L0X 1 Setup");
  if (!lox1.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  Serial.println();

  // Init sensor on bus number 2
  TCA9548A(2);
  Serial.println("VL53L0X 2 Setup");
  if (!lox2.begin()) {
    Serial.println(F("Failed to boot VL53L0X 2"));
    while(1);
  }
  Serial.println();

  // Init sensor on bus number 3
  TCA9548A(3);
  Serial.println("VL53L0X 3 Setup");
  if (!lox3.begin()) {
    Serial.println(F("Failed to boot VL53L0X 3"));
    while(1);
  }
  Serial.println(); 
}

//************************************************************** */
void vl53l0x_read() {
  //Print values for sensors
  getValue(lox0, 0);
  getValue(lox1, 1);
  getValue(lox2, 2);
  getValue(lox3, 3);
  Serial.println(); SerialBT.println();
  delay(1000);
}

//************************************************************** */
int GRID_SPACE = 300;

int numRows=9;
int numCols=9;
int occupancyGrid[9][9] = {0};

int robotRow = 4;  // Visualiser positions
int robotCol = 4;

int rRow;          // Matrix positions
int rCol;

int rightTurns=0;
int leftTurns=0;

void printMatrix(int arr[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            char buffer[4];
            sprintf(buffer, "%3d", arr[i][j]);
            Serial.print(buffer); SerialBT.print(buffer);
        }
        Serial.println(); SerialBT.println();
    }
    Serial.println(); SerialBT.println();
}

//************************************************************** */
int ROBOT_ID;

unsigned long previousMillis0 = 0; 

// pwm props
const int PWMm1a = 0;
const int PWMm1b = 2;
const int PWMm2a = 4;
const int PWMm2b = 6;
int maxpwm = 255;
int minpwm = 50; // for 10 bit resolution

// encoders
volatile int count1 = 0; // right motor
volatile int count2 = 0; // left motor

// TURN ENCODER PARAMS
float kp_turn = 0.7;
float kd_turn = 1;
float kp_gyro_turn = 4;
float kd_gyro_turn = 2;
int turnCount = 345;

// MOVE ENCODER PARAMS
float kp_move = 1;
float kd_move = 0;
int moveCount = 1175;









void setup()
{
    // Enables Serial Communication with baudRate of 115200
    Serial.begin(115200);                      //  Serial Monitor
    SerialBT.begin("ESP32_Robot");               // Bluetooth device  name
    Serial.println("PeraSwarm Robot v5");

    memory.begin(); // NOTE: This should be run as the first thing.

    // This commands should be run 'ONLY' at the first run to assign a ID for robot
    // memory.setupRobotWithId(RobotId)
    // memory.setupRobotWithId(0);
    ROBOT_ID = memory.getRobotId();

    pinMode(PIN_LED_INBUILT, OUTPUT);

    // // Scan available I2C devices
    // i2c_scan();

    motors.begin();
    motion.begin();

    motion.calibrate();
    memory.saveCalibration();

    memory.loadCalibration();

    // vl53l0x_setup();

    // rRow = numRows-1-robotRow;
    // rCol = robotCol;
    // // occupancyGrid[4][1] = -2;
    // occupancyGrid[rRow][rCol] = 3;

    // printMatrix(occupancyGrid);

    // delay(3000);
}

void robotPosUpdate(){

}

void mapFunc1(){
  int PROXIMITY_FRONT = getValue(lox0, 0) - 10;
  int PROXIMITY_RIGHT = getValue(lox1, 1) - 10;
  int PROXIMITY_BACK = getValue(lox2, 2) - 10;
  int PROXIMITY_LEFT = getValue(lox3, 3) - 10;

  if (PROXIMITY_FRONT > 650) {PROXIMITY_FRONT = 650;}
  if (PROXIMITY_RIGHT > 650) {PROXIMITY_RIGHT = 650;}
  if (PROXIMITY_BACK > 650) {PROXIMITY_BACK = 650;}
  if (PROXIMITY_LEFT > 650) {PROXIMITY_LEFT = 650;}

  //right
  if (PROXIMITY_RIGHT > GRID_SPACE) {
    int count=0;
    for (int i = 1; i <= (PROXIMITY_RIGHT / GRID_SPACE); i++) {
        occupancyGrid[rRow][rCol+(i)] = 1;
        count++;
    }
  } else {
    occupancyGrid[rRow][rCol+1] = 2;
  }

  //front
  if (PROXIMITY_FRONT > GRID_SPACE) {
    int count=0;
    for (int i = 1; i <= PROXIMITY_FRONT / GRID_SPACE; i++) {
        occupancyGrid[rRow-i][rCol] = 1;
        count++;
    }
  } else {
    occupancyGrid[rRow-1][rCol] = 2;
  }

  // left
  if (PROXIMITY_LEFT > GRID_SPACE) {
    int count=0;
    for (int i = 1; i <= PROXIMITY_LEFT / GRID_SPACE; i++) {
        occupancyGrid[rRow][rCol-i] = 1;
        count++;
    }    
  } else {
    occupancyGrid[rRow][rCol-1] = 2;  
  }

  //back
  if (PROXIMITY_BACK > GRID_SPACE) {
    int count=0;
    for (int i = 1; i <= PROXIMITY_BACK / GRID_SPACE; i++) {
        occupancyGrid[rRow+i][rCol] = 1;
        count++;
    }   
  } else {
    occupancyGrid[rRow+1][rCol] = 2;
  }
}



void mapFunc2(){
  int PROXIMITY_FRONT = getValue(lox0, 0) - 10;
  int PROXIMITY_RIGHT = getValue(lox1, 1) - 10;
  int PROXIMITY_BACK = getValue(lox2, 2) - 10;
  int PROXIMITY_LEFT = getValue(lox3, 3) - 10;

  if (PROXIMITY_FRONT > 650) {PROXIMITY_FRONT = 650;}
  if (PROXIMITY_RIGHT > 650) {PROXIMITY_RIGHT = 650;}
  if (PROXIMITY_BACK > 650) {PROXIMITY_BACK = 650;}
  if (PROXIMITY_LEFT > 650) {PROXIMITY_LEFT = 650;}

  //right
  if (PROXIMITY_RIGHT > GRID_SPACE) {
    int count=0;
    for (int i = 1; i <= PROXIMITY_RIGHT / GRID_SPACE; i++) {
        occupancyGrid[rRow+i][rCol] = 1;
        count++;
    }
  } else {
    occupancyGrid[rRow+1][rCol] = 2;
  }

  //front
  if (PROXIMITY_FRONT > GRID_SPACE) {
    int count=0;
    for (int i = 1; i <= PROXIMITY_FRONT / GRID_SPACE; i++) {
        occupancyGrid[rRow][rCol+i] = 1;
        count++;
    }  
  } else {
    occupancyGrid[rRow][rCol+1] = 2;
  }

  // left
  if (PROXIMITY_LEFT > GRID_SPACE) {
    int count=0;
    for (int i = 1; i <= PROXIMITY_LEFT / GRID_SPACE; i++) {
        occupancyGrid[rRow-i][rCol] = 1;
        count++;
    }      
  } else {
    occupancyGrid[rRow-1][rCol] = 2;  
  }

  //back
  if (PROXIMITY_BACK > GRID_SPACE) {
    int count=0;
    for (int i = 1; i <= PROXIMITY_BACK / GRID_SPACE; i++) {
        occupancyGrid[rRow][rCol-i] = 1;
        count++;
    }
  } else {
    occupancyGrid[rRow][rCol-1] = 2;
  }
}





void move(int dir)
{
  Serial.println("move");

  // dir=1 for forward
  // dir=-1 for backward
  // using encoders
  int right_dir, left_dir;

  if (dir == 1)
  {
    right_dir = 1, left_dir = 1; // both motors move in the same direction for forward motion
  }
  else
  {
    right_dir = -1, left_dir = -1; // both motors move in the opposite direction for backward motion
  }

  int move_speed1 = 0, move_speed2 = 0;
  float pre_error1 = 0, pre_error2 = 0;
  motors.encoder_reset();
  count1 = 0;
  count2 = 0;
  float error1 = right_dir * moveCount, error2 = left_dir * moveCount;
  
  while (abs(error1) > 2 || abs(error2) > 2)
  {
    
    EncoderReadings readings = motors.encoder_read();
    count1 = readings.right;
    count2 = readings.left;

    float move_speed_f1 = kp_move * abs(error1) + kd_move * (error1 - pre_error1);
    float move_speed_f2 = kp_move * abs(error2) + kd_move * (error2 - pre_error2);
    
    if (move_speed_f1 < minpwm)
    {
      move_speed1 = minpwm;
    }
    else if (move_speed_f1 > maxpwm)
    {
      move_speed1 = maxpwm;
    }
    else
    {
      move_speed1 = (int)move_speed_f1;
    }

    if (move_speed_f2 < minpwm)
    {
      move_speed2 = minpwm;
    }
    else if (move_speed_f2 > maxpwm)
    {
      move_speed2 = maxpwm;
    }
    else
    {
      move_speed2 = (int)move_speed_f2;
    }

    int speed1 = (error1 > 0) ? move_speed1 : -move_speed1;
    int speed2 = (error2 > 0) ? move_speed2 : -move_speed2;

    motors.write(speed2, speed1);

    
    delay(5);
    pre_error1 = error1;
    error1 = (right_dir * moveCount - count1);

    pre_error2 = error2;
    error2 = (left_dir * moveCount - count2);

    // Serial.print("error1: "); Serial.print(error1); Serial.print(" error2: "); Serial.println(error2);
  }

  motors.stop();
  
  count1 = 0;
  count2 = 0;
}




void turn90(int dir)
{
  Serial.println("turn90");

  // dir=1 for left turn
  // dir=-1 for right turn
  // using encoders
  int right_dir, left_dir;

  if (dir == 1)
  {
    right_dir = 1, left_dir = -1;
  }
  else
  {
    right_dir = -1, left_dir = 1;
  }

  int turn_speed1 = 0, turn_speed2 = 0;
  float pre_error1 = 0, pre_error2 = 0;
  motors.encoder_reset();
  count1 = 0;
  count2 = 0;
  float error1 = right_dir * turnCount, error2 = left_dir * turnCount;
  
  Serial.print("error1: "); Serial.println(error1);
  Serial.print("error2: "); Serial.println(error2);


//   mpu.update();
//   float initial_yaw = mpu.getAngleZ();
  motion.update();
  float initial_yaw = motion.get_heading();
  Serial.print("Initial Yaw: "); Serial.println(initial_yaw);
  float desired_yaw;

  if (dir == 1)
  {
    desired_yaw = initial_yaw - 90;
  }
  else
  {
    desired_yaw = initial_yaw + 90;
  }

  while (abs(error1) > 2 || abs(error2) > 2)
  {
    
    EncoderReadings readings = motors.encoder_read();
    count1 = readings.right;
    count2 = readings.left;

    // Serial.println("turn90 loop 1");
    // Serial.print("count1: "); Serial.print(count1); Serial.print(" count2: "); Serial.println(count2);

    // mpu.update();
    motion.update();
    float turn_speed_f1 = kp_turn * abs(error1) + kd_turn * (error1 - pre_error1);
    float turn_speed_f2 = kp_turn * abs(error2) + kd_turn * (error2 - pre_error2);
    
    if (turn_speed_f1 < minpwm)
    {
      turn_speed1 = minpwm;
    }
    else if (turn_speed_f1 > maxpwm)
    {
      turn_speed1 = maxpwm;
    }
    else
    {
      turn_speed1 = (int)turn_speed_f1;
    }

    if (turn_speed_f2 < minpwm)
    {
      turn_speed2 = minpwm;
    }
    else if (turn_speed_f2 > maxpwm)
    {
      turn_speed2 = maxpwm;
    }
    else
    {
      turn_speed2 = (int)turn_speed_f2;
    }

    // int speed1 = (error1 > 0) ? 80 : -80;
    // int speed2 = (error2 > 0) ? 80 : -80;

    int speed1 = (error1 > 0) ? turn_speed1 : -turn_speed1;
    int speed2 = (error2 > 0) ? turn_speed2 : -turn_speed2;

    motors.write(speed2, speed1);

    
    delay(5);
    pre_error1 = error1;
    error1 = (right_dir * turnCount - count1);

    pre_error2 = error2;
    error2 = (left_dir * turnCount - count2);

    // Serial.print("error1: "); Serial.print(error1); Serial.print(" error2: "); Serial.println(error2);
  }

  // ledcWrite(PWMm1a, 0);
  // ledcWrite(PWMm1b, 0);
  // ledcWrite(PWMm2a, 0);
  // ledcWrite(PWMm2b, 0);
  motors.stop();
  
  count1 = 0;
  count2 = 0;
  
  delay(50);

  Serial.println("turn90 loop 2 ********************************************************************************************************");
  // motion.update();
  // if (abs(motion.get_heading() - desired_yaw) > 5)
  // {
  //   //      SerialBT.println(mpu.getAngleZ() - desired_yaw);
  //   // mpu.update();
  //   motion.update();
  //   while (abs(motion.get_heading() - desired_yaw) > 0.1)
  //   {
  //     float pre_yaw_error = 0;
  //     while (true)
  //     {
  //       // mpu.update();
  //       motion.update();
  //       float yaw_error = desired_yaw - motion.get_heading();

  //       if (abs(yaw_error) < 0.1)
  //       {
  //         // ledcWrite(PWMm1a, 0);
  //         // ledcWrite(PWMm1b, 0);
  //         // ledcWrite(PWMm2a, 0);
  //         // ledcWrite(PWMm2b, 0);
  //         motors.stop();
  //         break;
  //       }

  //       float speedR = (kp_gyro_turn * abs(yaw_error) + kd_gyro_turn * (yaw_error - pre_yaw_error));
  //       float speedL = (kp_gyro_turn * abs(yaw_error) + kd_gyro_turn * (yaw_error - pre_yaw_error));

  //       if (speedR < minpwm)
  //       {
  //         speedR = minpwm;
  //       }
  //       else if (speedR > maxpwm)
  //       {
  //         speedR = maxpwm;
  //       }
  //       else
  //       {
  //         speedR = (int)speedR;
  //       }
  //       if (speedL < minpwm)
  //       {
  //         speedL = minpwm;
  //       }
  //       else if (speedL > maxpwm)
  //       {
  //         speedL = maxpwm;
  //       }
  //       else
  //       {
  //         speedL = (int)speedL;
  //       }

  //       if (yaw_error > 0)
  //       {
  //         // ledcWrite(PWMm1a, abs(speedL));
  //         // ledcWrite(PWMm1b, 0);
  //         // ledcWrite(PWMm2a, 0);
  //         // ledcWrite(PWMm2b, abs(speedR));
  //         motors.write(abs(speedL), -abs(speedR));
  //         // delay(10);
  //       }
  //       else
  //       {
  //         // ledcWrite(PWMm1a, 0);
  //         // ledcWrite(PWMm1b, abs(speedL));
  //         // ledcWrite(PWMm2a, abs(speedR));
  //         // ledcWrite(PWMm2b, 0);
  //         motors.write(-abs(speedL), abs(speedR));
  //         // delay(10);
  //       }
  //       pre_yaw_error = yaw_error;
  //     }
  //   //   mpu.update();
  //     motion.update();
  //   }
  // }
}


void turn90_imu(int dir)
{
  Serial.println("turn90_imu");

  // dir=1 for left turn
  // dir=-1 for right turn
  int right_dir, left_dir;

  if (dir == 1) {
    right_dir = 1, left_dir = -1;
  } else {
    right_dir = -1, left_dir = 1;
  }

  motion.update();
  float initial_yaw = motion.get_heading();
  Serial.print("Initial Yaw: "); Serial.println(initial_yaw);
  float desired_yaw;

  if (dir == 1) {
    desired_yaw = initial_yaw - 90;
  } else {
    desired_yaw = initial_yaw + 90;
  }

  //      SerialBT.println(mpu.getAngleZ() - desired_yaw);
  motion.update();
  while (abs(motion.get_heading() - desired_yaw) > 0.1)
  {
    float pre_yaw_error = 0;
    while (true)
    {
      motion.update();
      float yaw_error = desired_yaw - motion.get_heading();

      if (abs(yaw_error) < 0.1)
      {
        motors.stop();
        break;
      }

      float speedR = (kp_gyro_turn * abs(yaw_error) + kd_gyro_turn * (yaw_error - pre_yaw_error));
      float speedL = (kp_gyro_turn * abs(yaw_error) + kd_gyro_turn * (yaw_error - pre_yaw_error));

      if (speedR < minpwm) {
        speedR = minpwm;
      } else if (speedR > maxpwm) {
        speedR = maxpwm;
      } else {
        speedR = (int)speedR;
      }

      if (speedL < minpwm) {
        speedL = minpwm;
      } else if (speedL > maxpwm) {
        speedL = maxpwm;
      } else {
        speedL = (int)speedL;
      }

      if (yaw_error > 0) {
        motors.write(abs(speedL), -abs(speedR));
        // delay(10);
      } else {
        motors.write(-abs(speedL), abs(speedR));
        // delay(10);
      }
      
      pre_yaw_error = yaw_error;
    }
  }
}

void bluetoothCommunication() {
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

    if (parameter == "kp_turn"){ kp_turn = value; Serial.print("kp_turn: "); Serial.println(kp_turn);}       
    else if (parameter == "kd_turn"){ kd_turn = value; Serial.print("kd_turn: "); Serial.println(kd_turn);}
    else if (parameter == "turnCount"){ turnCount = int(value); Serial.print("turnCount: "); Serial.println(kd_turn);}
  }
}


void odometryBasedLocalizationTest(){
 // 1 1
  // mapFunc1();
  printMatrix(occupancyGrid);
  delay(3000);
  
  // 1 2
  occupancyGrid[4][4] = 1;
  occupancyGrid[3][4] = 3;
  move(1);
  // mapFunc1();
  printMatrix(occupancyGrid);
  delay(3000);

  // 2 2 
  occupancyGrid[3][4] = 1;
  occupancyGrid[3][5] = 3;
  turn90(-1);
  delay(3000);
  move(1);
  // mapFunc2();
  printMatrix(occupancyGrid);
  delay(3000);
  
  // 2 3
  occupancyGrid[3][5] = 1;
  occupancyGrid[2][5] = 3;
  turn90(1);
  delay(3000);
  move(1);
  // mapFunc1();
  printMatrix(occupancyGrid);
  delay(3000);

  // 3 3
  occupancyGrid[2][5] = 1;
  occupancyGrid[2][6] = 3;
  turn90(-1);
  delay(3000);
  move(1);
  // mapFunc2();
  printMatrix(occupancyGrid);
  delay(3000);
  
  // 3 4
  occupancyGrid[2][6] = 1;
  occupancyGrid[1][6] = 3;
  turn90(1);
  delay(3000);
  move(1);
  // mapFunc1();
  printMatrix(occupancyGrid);
  delay(3000);

  // 4 4
  occupancyGrid[1][6] = 1;
  occupancyGrid[1][7] = 3;
  turn90(-1);
  delay(3000);
  move(1);
  // mapFunc2();
  printMatrix(occupancyGrid);
  delay(3000);
}

void headingTest(){
  if (motion.update()) {
    float heading = motion.get_heading();

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis0 >= 500) {
      previousMillis0 = currentMillis;
      Serial.printf("%8.2f\n", heading);
    }
  }
}

void loop()
{
  // bluetoothCommunication();
  // odometryBasedLocalizationTest();
  
  // vl53l0x_read();
  // motors.encoder_print();

  headingTest();




  // unsigned long currentMillis0 = millis();
  // if (currentMillis0 - previousMillis0 >= 3000) {
  //   previousMillis0 = currentMillis0;
  //   turnTest();
  // }






  // if (motion.update()) {
  //   float heading = motion.get_heading();
  //   Serial.printf("%8.2f\n", heading);
  //   motors.encoder_print();
  // }





  // turnTest();
  // motors.encoder_print();
  // delay(3000);



  // motors.write(50, 50);
  // motors.test();



  // digitalWrite(PIN_MOT_R1, motorDir ? LOW : HIGH);
  // digitalWrite(PIN_MOT_R2, motorDir ? HIGH : LOW);

  // // Set motor speed
  // this->rightMotorSpeed = abs(speed);

  // // Analog write function for ESP32
  // ledcWrite(LEDC_CHANNEL_B, this->rightMotorSpeed);


  // digitalWrite(PIN_MOT_R1, HIGH);
  // digitalWrite(PIN_MOT_R2, LOW );

  // digitalWrite(PIN_MOT_R1, LOW);
  // digitalWrite(PIN_MOT_R2, HIGH );

  // // Analog write function for ESP32
  // ledcWrite(8, 80);
  // delay(3000);

  // digitalWrite(PIN_MOT_R1, HIGH);
  // digitalWrite(PIN_MOT_R2, LOW );

  // ledcWrite(8, 80);
  // delay(3000);

}