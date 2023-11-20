//��ʱ
#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"

void Delay10Ms(unsigned char ms) //10����
{
	Delay10KTCYx(((Fosc/4000) * ms)/1000);
}

void DelayMs(unsigned char ms) //1����
{
	Delay1KTCYx(((Fosc/4000) * ms)/1000);
}

void Delay10us(unsigned char us) //10΢��
{
	Delay10TCYx((Fosc/1000000) * us);
}

void Delayus(unsigned char us) //1΢��
{
	Delay10TCYx((Fosc/10000000) * us);
}
