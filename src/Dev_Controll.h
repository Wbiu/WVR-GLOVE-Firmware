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
void readIMU();
bool initDev();
void sendSerial(void * parameter);
void sendSerial_no_IMU(void * parameter);
void initSPIFFS();
void ffb(void * parameter);
void initTasks();
#endif

