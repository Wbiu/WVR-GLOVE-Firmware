/*
Pins varies depending on boards that is used.
First check if there is enought pin to use.
Or an extended board must added.
*/


/*Pins that are used to read the finger position values*/
#define THUMB_READING_PIN   36
#define INDEX_READING_PIN   39
#define MIDDLE_READING_PIN  34
#define RING_READING_PIN    35
#define PINKY_READING_PIN   32

/*sets the analog read resolution. ESP32 support up to 12-bit.
In this matter only 8 is enough.
*/
#define ANALOG_READ_RES     8

/*Pins that are used to controll the motors/h-bridge*/
#define THUMB_CTL_PIN_INA   33
#define THUMB_CTL_PIN_INB   25
#define INDEX_CTL_PIN_INA   26
#define INDEX_CTL_PIN_INB   27
#define MIDDLE_CTL_PIN_INA  23
#define MIDDLE_CTL_PIN_INB  12
#define RING_CTL_PIN_INA    19
#define RING_CTL_PIN_INB    18
#define PINKY_CTL_PIN_INA   4
#define PINKY_CTL_PIN_INB   13


/*Assigning each motors/h-bridge controll pins to there own PWM channel.
The eps32 is equipt with 16 pwn channels (0-15)
the first 8 are hight speed channel and the other 8 are low speed.
*/
#define THUMB_PWM_CHN_INA   0
#define THUMB_PWM_CHN_INB   1
#define INDEX_PWM_CHN_INA   2
#define INDEX_PWM_CHN_INB   3
#define MIDDLE_PWM_CHN_INA  4
#define MIDDLE_PWM_CHN_INB  5
#define RING_PWM_CHN_INA    6
#define RING_PWM_CHN_INB    7
#define PINKY_PWM_CHN_INA   8
#define PINKY_PWM_CHN_INB   9

/*PWM settings*/
#define PWM_FREQ            100000 // 1 = 1HZ
#define PWM_RES             8   //upto 16 bit


/*i2c pins*/
#define SDA_PIN             21
#define SCL_PIN             22

#define FILENAME  "/settings.json"

/*UART setting*/
#define BAUNDRATE           250000

#define DEVICE_STATUS_ERR   0
#define DEVICE_STATUS_RDY   1

#define FILE_BUFFER_SIZE    45
#define FINGER_BUFFER_SIZE  25
#define IMU_BUFFER_SIZE     3

/*sample size for reading finger values*/
#define SAMPLE_SIZE         10

#define OUT_PUT_SERIAL      0
#define OUT_PUT_WIRELESS    1

#define CLI_BUFFER_SIZE     25


#define WHITESPACE         0x20

/*CLI Comands*/
#define MACADDR_CMD         0
#define HOLD_CMD            1
#define CONTINUE            3
#define HOME_POS            4
#define FFB_Spin            5


#define MAXFBB              255

#define THB_READING_IDX     0
#define IDX_READING_IDX     1
#define MDL_READING_IDX     2
#define RNG_READING_IDX     3
#define PKY_READING_IDX     4

#define SERIAL_SIZE_RX      4069

#define CMDSIZE             5

#define HOMEPOS_SPEED       120
