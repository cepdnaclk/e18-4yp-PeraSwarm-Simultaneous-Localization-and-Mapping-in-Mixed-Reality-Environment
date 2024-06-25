

// ------------------------------------------------------------------- GPIO Pins
#define PIN_LED 2
#define PIN_SW_0 19
#define PIN_SW_1 18

// --------------------------------------------------------- Motors and Encoders
#define PIN_SERVO_LEFT 12
#define PIN_SERVO_RIGHT 13

#define PIN_MOT_A 26
#define PIN_MOT_B 25

#define PIN_PWM_A 13
#define PIN_PWM_B 12

#define PIN_ENCODER_L 5
#define PIN_ENCODER_R 17

// ----------------------------------------------------------- Neopixel LED Ring
#define PIN_NEOPIXEL_LED 23

// ------------------------------------------------------------- Distance Sensor
#define PIN_DIST_SENSOR 32

// ------------------------------------------------------------ IR communication
#define PIN_IR_TX 16

#define PIN_IR_RX_0 39
#define PIN_IR_RX_1 36
#define PIN_IR_RX_2 34
#define PIN_IR_RX_3 33

// ----------------------------------------------------- Unused or reserved pins
#define PIN_TX 1
#define PIN_RX 3

#define PIN_SCL 22
#define PIN_SDA 21

// ------------------------------------------------------------------ Other Pins
#define PIN_BATTERY_SENSE 35

// --------------------------------------------------------- I2C  Port addresses
#define TCA9548A_ADDR 0x70       // [8-CHANNEL I2C SWITCH WITH RESET] (112)

#define I2C_COMPASS_ACC 0x19 // (25)
#define I2C_COMPASS_MAG 0x1E // (30)
#define I2C_PCF8574 0x27     // [REMOTE 8-BIT I/O EXPANDER FOR I2C BUS] (39)


#define I2C_VLX53L0X 0x2A    // (42)


#define I2C_TCS34725 0x29    // [COLOR LIGHT-TO-DIGITAL CONVERTER] (41) , wire2 interface
#define I2C_SSD1306 0x3C     //  [OLED/PLED DRIVER WITH CONTROLLER] (60)
