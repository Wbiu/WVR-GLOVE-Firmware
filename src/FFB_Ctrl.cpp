#include "FFB_Ctrl.h"

int PosZero = 0;
int closePos = 50;
int tmb_ffb_buff;
int idx_ffb_buff;
int mdl_ffb_buff;
int rng_ffb_buff;
int pky_ffb_buff;

void homePos()
{

    while (map(analogRead(THUMB_READING_PIN), 0, 255, 255, 0) != PosZero)
    {
        ledcWrite(THUMB_PWM_CHN_INA, HOMEPOS_SPEED);
        ledcWrite(THUMB_PWM_CHN_INB, 0);
    }
    ledcWrite(THUMB_PWM_CHN_INA, 0);
    ledcWrite(THUMB_PWM_CHN_INB, 0);
    while (map(analogRead(INDEX_READING_PIN), 0, 255, 255, 0) != PosZero)
    {
        ledcWrite(INDEX_PWM_CHN_INA, HOMEPOS_SPEED);
        ledcWrite(INDEX_PWM_CHN_INB, 0);
    }
    ledcWrite(INDEX_PWM_CHN_INA, 0);
    ledcWrite(INDEX_PWM_CHN_INB, 0);

    while (map(analogRead(MIDDLE_READING_PIN), 0, 255, 255, 0) != PosZero)
    {
        ledcWrite(MIDDLE_PWM_CHN_INA, HOMEPOS_SPEED + 50);
        ledcWrite(MIDDLE_PWM_CHN_INB, 0);
    }
    ledcWrite(MIDDLE_PWM_CHN_INA, 0);
    ledcWrite(MIDDLE_PWM_CHN_INB, 0);

    while (analogRead(RING_READING_PIN) != PosZero)
    {
        ledcWrite(RING_PWM_CHN_INA, HOMEPOS_SPEED);
        ledcWrite(RING_PWM_CHN_INB, 0);
    }
    ledcWrite(RING_PWM_CHN_INB, 0);
    ledcWrite(RING_PWM_CHN_INA, 0);

    while (analogRead(PINKY_READING_PIN) != PosZero)
    {
        ledcWrite(PINKY_PWM_CHN_INB, HOMEPOS_SPEED);
        ledcWrite(PINKY_PWM_CHN_INA, 0);
    }
    ledcWrite(PINKY_PWM_CHN_INB, 0);
    ledcWrite(PINKY_PWM_CHN_INA, 0);
}
void setFFB(volatile int* tmb_ffb ,volatile int* idx_ffb,volatile int* mdl_ffb,volatile int* rng_ffb,volatile int* pky_ffb)
{

    if ((*tmb_ffb) != tmb_ffb_buff)
    {
        tmb_ffb_buff = *tmb_ffb;
        if(tmb_ffb_buff<0)
        {   
            //tmb_ffb_buff = -(*tmb_ffb);
            ledcWrite(THUMB_PWM_CHN_INA, LOW);
            ledcWrite(THUMB_PWM_CHN_INB, -(tmb_ffb_buff));
        }
        else
        {
            ledcWrite(THUMB_PWM_CHN_INA, tmb_ffb_buff);
            ledcWrite(THUMB_PWM_CHN_INB, LOW);
        }
    }

    if ((*idx_ffb) != idx_ffb_buff)
    {
        idx_ffb_buff = *idx_ffb;
        if(idx_ffb_buff<0)
        {
            //idx_ffb_buff = -(*idx_ffb);
            ledcWrite(INDEX_PWM_CHN_INA, LOW);
            ledcWrite(INDEX_PWM_CHN_INB, -(idx_ffb_buff));
        }
        else
        {
            ledcWrite(INDEX_PWM_CHN_INA, idx_ffb_buff);
            ledcWrite(INDEX_PWM_CHN_INB, LOW);
        }
    }

    if ((*mdl_ffb) != mdl_ffb_buff)
    {
        mdl_ffb_buff = *mdl_ffb;
        if(mdl_ffb_buff<0)
        {   
            //mdl_ffb_buff = -(*mdl_ffb);
            ledcWrite(MIDDLE_PWM_CHN_INA, LOW);
            ledcWrite(MIDDLE_PWM_CHN_INB, -(mdl_ffb_buff));
        }
        else
        {
            ledcWrite(MIDDLE_PWM_CHN_INA, mdl_ffb_buff);
            ledcWrite(MIDDLE_PWM_CHN_INB, LOW);
        }
    }

    if ((*rng_ffb) != rng_ffb_buff)
    {   
        rng_ffb_buff = *rng_ffb;
        if(rng_ffb_buff<0)
        {   
            //rng_ffb_buff = -(*rng_ffb);
            ledcWrite(RING_PWM_CHN_INA, LOW);
            ledcWrite(RING_PWM_CHN_INB, -(rng_ffb_buff));
        }
        else
        {
            ledcWrite(RING_PWM_CHN_INA, rng_ffb_buff);
            ledcWrite(RING_PWM_CHN_INB,LOW);
        }
    }

    if ((*pky_ffb) != pky_ffb_buff)
    {
        pky_ffb_buff = *pky_ffb;
        if(pky_ffb_buff<0)
        {
            ledcWrite(PINKY_PWM_CHN_INA, LOW);
            ledcWrite(PINKY_PWM_CHN_INB, -(pky_ffb_buff));
        }
        else
        {
            ledcWrite(PINKY_PWM_CHN_INA, pky_ffb_buff);
            ledcWrite(PINKY_PWM_CHN_INB, LOW);
        }
    }
}
void checkServo()
{
    for (int i = 0; i < 10; i++)
    {
        homePos();
    }

    while (map(analogRead(THUMB_READING_PIN), 0, 255, 255, 0) != closePos)
    {
        ledcWrite(THUMB_PWM_CHN_INA, HOMEPOS_SPEED);
        ledcWrite(THUMB_PWM_CHN_INB, 0);
    }
    ledcWrite(THUMB_PWM_CHN_INA, 0);
    ledcWrite(THUMB_PWM_CHN_INB, 0);
    while (map(analogRead(INDEX_READING_PIN), 0, 255, 255, 0) != closePos)
    {
        ledcWrite(INDEX_PWM_CHN_INA, 0);
        ledcWrite(INDEX_PWM_CHN_INB, HOMEPOS_SPEED);
    }
    ledcWrite(INDEX_PWM_CHN_INA, 0);
    ledcWrite(INDEX_PWM_CHN_INB, 0);

    while (map(analogRead(MIDDLE_READING_PIN), 0, 255, 255, 0) != closePos)
    {
        ledcWrite(MIDDLE_PWM_CHN_INA, 0);
        ledcWrite(MIDDLE_PWM_CHN_INB, HOMEPOS_SPEED);
    }
    ledcWrite(MIDDLE_PWM_CHN_INA, 0);
    ledcWrite(MIDDLE_PWM_CHN_INB, 0);

    while (analogRead(RING_READING_PIN) != closePos)
    {
        ledcWrite(RING_PWM_CHN_INA, 0);
        ledcWrite(RING_PWM_CHN_INB, HOMEPOS_SPEED);
    }
    ledcWrite(RING_PWM_CHN_INB, 0);
    ledcWrite(RING_PWM_CHN_INA, 0);

    while (analogRead(PINKY_READING_PIN) != closePos)
    {
        ledcWrite(PINKY_PWM_CHN_INB, 0);
        ledcWrite(PINKY_PWM_CHN_INA, HOMEPOS_SPEED);
    }
    ledcWrite(PINKY_PWM_CHN_INB, 0);
    ledcWrite(PINKY_PWM_CHN_INA, 0);
    for (int i = 0; i < 10; i++)
    {
        homePos();
    }
}
