
typedef struct dev_conf{
    const char * dev_name;
    int onboot_servo;
    int imu_state;
    IMU::DATATYPE gyrodata;
    connectionType conType;
    uint8_t collector_addr[6];
}dev_conf;

