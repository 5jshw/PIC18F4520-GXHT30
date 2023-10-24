 //������

#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"
//#include <adc.h>
//#include <stdlib.h>

float Temperature;
float Humidity;
const unsigned char TABLE[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F};
int a = 1;

unsigned char p;
unsigned char q;

unsigned int addr;
	
void main(void)
{
	OSCCON = 0b01110011; //8MHz����Ƶ�ʣ��ڲ�������·
	addr = 0x88; //������Ĭ�ϵ�ַ
	k18_init(); //��ʼ��
	IIC_init();
	SMG_O();
	SMG_L();
	
	K1();
	Delay10Ms(20);
	G1();
	
	DDRCbits.RC2 = 0; //����ýӿڵķ���Ĵ��������Խ�CCP1����������Ϊ���
	CCP1CON = 0B00001100; //��CCPģ���޸�ΪPWMģʽ
	PR2 = 0x20; //ͨ��PR2�Ĵ�������PWM������
	CCPR1L = 0X00; //����PWMռ�ձ�
	T2CON = 0B00000101; //����TMR2Ԥ��Ƶֵ��ʹ��Timer2

	while(1)
	{
		
		CCPR1L = CCPR1L + 1*a; //ʹ�������
		Delay10Ms(20);
		if(CCPR1L >= 0x1E || CCPR1L <= 0x00){a = a * (-1);}
		
		GXHT30_write_cmd(addr, 0x2c, 0x0d);
		GXHT30_read_result(addr);
		Delay10Ms(2);
		
		p = ((int)Temperature / 10);
		q = ((int)Temperature % 10);
		
		if(Temperature <= 27)
		{
			
			L1 = 1;
			L2 = 0;
			L3 = 0;
			L4 = 0;
			Delay10Ms(1);
			
		}else if(Temperature > 27 && Temperature <= 28){
		
			L1 = 0;
			L2 = 1;
			L3 = 0;
			L4 = 0;
			Delay10Ms(1);
			
		}else if(Temperature >28 && Temperature <= 29){
		
			L1 = 0;
			L2 = 0;
			L3 = 1;
			L4 = 0;
			Delay10Ms(1);
			
		}else if(Temperature > 29 && Temperature <= 32){
		
			L1 = 0;
			L2 = 0;
			L3 = 0;
			L4 = 1;
			Delay10Ms(1);
			
		}else{
			L1 = 1;
			L2 = 1;
			L3 = 1;
			L4 = 1;
			Delay10Ms(1);
		}
		

		PORTB = TABLE[p];
		C1 = 0;
		C2 = 1;
		Delay10Ms(1);

		PORTB = TABLE[q];
		C2 = 0;
		C1 = 1;
		Delay10Ms(1);
		
		PORTB = TABLE[p];
		C1 = 0;
		C2 = 1;
		Delay10Ms(1);

		PORTB = TABLE[q];
		C2 = 0;
		C1 = 1;
		Delay10Ms(1);
		
		PORTB = TABLE[p];
		C1 = 0;
		C2 = 1;
		Delay10Ms(1);

		PORTB = TABLE[q];
		C2 = 0;
		C1 = 1;
		Delay10Ms(1);
		
		PORTB = TABLE[p];
		C1 = 0;
		C2 = 1;
		Delay10Ms(1);

		PORTB = TABLE[q];
		C2 = 0;
		C1 = 1;
		Delay10Ms(1);
	}

}


void K1(void)
{
	L1 = 1;
	L2 = 1;
	L3 = 1;
	L4 = 1;

}

void G1(void)
{
	L1 = 0;
	L2 = 0;
	L3 = 0;
	L4 = 0;

}

void SMG_O(void)
{
	T_A = 0;
	T_B = 0;
	T_C = 0;
	T_D = 0;
	T_E = 0;
	T_F = 0;
	T_G = 0;
	T_C1 = 0;
	T_C2 = 0;
	T_C3 = 0;
	T_C4 = 0;
}

void SMG_L(void)
{
	A = 0;
	B = 0;
	C = 0;
	D = 0;
	E = 0;
	F = 0;
	G = 0;
	C1 = 0;
	C2 = 0;
	C3 = 0;
	C4 = 0;
}