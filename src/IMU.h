/*
The IMU liblary is from Adafruit
https://github.com/adafruit/Adafruit_BNO055
*/
#include "SPIFFS_Helper.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <imumaths.h>

class IMU{
public :
    IMU(){};
    adafruit_bno055_offsets_t offsets_type;
    Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);

    bool init();
    void setBais();
    void setBais(adafruit_bno055_offsets_t offsets_type);
    void calibration();
    void getRoll_Pitch_Yaw(float ryp[]);
    void getQAUT(float quat[]);
    enum DATATYPE {EU,QUAT};
};


