#include "IMU.h"
//extern MPU9250 mpu;

bool IMU::init()
{   
    bool success = !IMU::bno.begin()? 0 : 1;
    delay(1000);
    if (success){
        setBais();
        bno.setExtCrystalUse(true);
    }
    return success;
}

void IMU::setBais()
{   
    /*
    These are pre-calibrated values.
    */
    offsets_type.accel_offset_x = -14;
    offsets_type.accel_offset_y = -77;
    offsets_type.accel_offset_z = 6;

    offsets_type.gyro_offset_x = -2;
    offsets_type.gyro_offset_y = 1;
    offsets_type.gyro_offset_z = 1;

    offsets_type.mag_offset_x = 104;
    offsets_type.mag_offset_y = -673;
    offsets_type.mag_offset_z = -616;

    IMU::bno.setSensorOffsets(offsets_type);
}

void IMU::setBais( adafruit_bno055_offsets_t offsets_type)
{   
    bno.setSensorOffsets(offsets_type);
}

void IMU::calibration()
{
    
}

void IMU::getRoll_Pitch_Yaw(float rpy[])
{   
        imu::Vector<3> euler = IMU::bno.getVector(Adafruit_BNO055::VECTOR_EULER);
        rpy[0] = euler.x();
        rpy[1] = euler.y();
        rpy[2] = euler.z();
}

void IMU::getQAUT(float quat[])
{
    imu::Quaternion quats = IMU::bno.getQuat();

 //     double &w() { return _w; }
//  double &x() { return _x; }
  //double &y() { return _y; }
 // double &z() { return _z; }

  quat[0] = quats.w();
  quat[1] = quats.x();
  quat[2] = quats.y();
  quat[3] = quats.z();

}


