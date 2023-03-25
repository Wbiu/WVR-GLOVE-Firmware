#include "WirelessCon.h"
extern int PosZero;
extern int closePos;
extern int tmb_ffb_buff;
extern int idx_ffb_buff;
extern int mdl_ffb_buff;
extern int rng_ffb_buff;
extern int pky_ffb_buff;

void setFFB(volatile int* tmb_ffb ,volatile int* idx_ffb,volatile int* mdl_ffb,volatile int* rng_ffb,volatile int* pky_ffb);
void homePos();
void checkServo();
