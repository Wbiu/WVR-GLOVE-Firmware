#include "Dev_FTest.h"

const int speed = 70;

void servoTest()
{
            ledcWrite(THUMB_PWM_CHN_INA, speed);
            ledcWrite(THUMB_PWM_CHN_INB,LOW);
            delay(5000);
            ledcWrite(THUMB_PWM_CHN_INA, LOW);
            ledcWrite(THUMB_PWM_CHN_INB, speed);
            delay(5000);

            ledcWrite(THUMB_PWM_CHN_INA, LOW);
            ledcWrite(THUMB_PWM_CHN_INB, LOW);
            
            ledcWrite(INDEX_PWM_CHN_INA, speed);
            ledcWrite(INDEX_PWM_CHN_INB,LOW);
            delay(5000);
            ledcWrite(INDEX_PWM_CHN_INA, LOW);
            ledcWrite(INDEX_PWM_CHN_INB, speed);
            delay(5000);

            ledcWrite(INDEX_PWM_CHN_INA, LOW);
            ledcWrite(INDEX_PWM_CHN_INB, LOW);


            ledcWrite(MIDDLE_PWM_CHN_INA, speed);
            ledcWrite(MIDDLE_PWM_CHN_INB,LOW);
            delay(5000);
            ledcWrite(MIDDLE_PWM_CHN_INA, LOW);
            ledcWrite(MIDDLE_PWM_CHN_INB, speed);
            delay(5000);

            ledcWrite(MIDDLE_PWM_CHN_INA, LOW);
            ledcWrite(MIDDLE_PWM_CHN_INB, LOW);


            ledcWrite(RING_PWM_CHN_INA, speed);
            ledcWrite(RING_PWM_CHN_INB,LOW);
            delay(5000);
            ledcWrite(RING_PWM_CHN_INA, LOW);
            ledcWrite(RING_PWM_CHN_INB, speed);
            delay(5000);

            ledcWrite(RING_PWM_CHN_INA, LOW);
            ledcWrite(RING_PWM_CHN_INB, LOW);

            ledcWrite(PINKY_PWM_CHN_INA, speed);
            ledcWrite(PINKY_PWM_CHN_INB,LOW);
            delay(5000);
            ledcWrite(PINKY_PWM_CHN_INA, LOW);
            ledcWrite(PINKY_PWM_CHN_INB, speed);
            delay(5000);
            
            ledcWrite(PINKY_PWM_CHN_INA, 0);
            ledcWrite(PINKY_PWM_CHN_INB, 0);
}