#pragma once
#include "FFB_Ctrl.h"

#ifndef DEVICE_SETUP_H
#define DEVICE_SETUP_H
/*if not get correctly implemented change to class with virtual funktions*/
enum connectionType {WIRED,WIRELESS}; 

extern volatile int tmb_ffb;
extern volatile int idx_ffb;
extern volatile int mdl_ffb;
extern volatile int rng_ffb;
extern volatile int pky_ffb;

void pinSetup();
void imuSetup();
void espNowSetup();
bool SPIFFSSetup();
void validateSettings();
void serialSetup();
void serialOnRecv_Cb();
void readFingerPos();
void readIMU_EU();
void readIMU_QUAT();
bool initDev();
void sendSerial(void * parameter);
void sendSerial_with_IMU_EU();
void sendSerial_with_IMU_QUAT();
void sendSerial_no_IMU();
void sendWireless(void * parameter);
void sendWireless_with_IMU_EU();
void sendWireless_with_IMU_QUAT();
void sendWireless_no_IMU();
void initSPIFFS();
void ffb(void * parameter);
void initTasks();
#endif

