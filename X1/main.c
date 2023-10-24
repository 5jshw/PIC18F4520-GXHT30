 //������

#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"
#include <adc.h>
#include <stdlib.h>
float Temperature;
float Humidity;
const unsigned char TABLE[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F};
int a = 1;
unsigned char p, q;
unsigned char k, l;
unsigned int addr;
int result;
int f1 = 1;

void main(void)
{

	T_L1 = 0;
	L1 = 1;
	Delay10Ms(200);
	L1 = 0;
	OSCCON = 0b01110011; //8MHz����Ƶ�ʣ��ڲ�������·
	addr = 0x88; //������Ĭ�ϵ�ַ
	GXHT30_init(); //��ʼ��

	DDRCbits.RC2 = 0; //����ýӿڵķ���Ĵ��������Խ�CCP1����������Ϊ���
	CCP1CON = 0B00001100; //��CCPģ���޸�ΪPWMģʽ
	PR2 = 0x20; //ͨ��PR2�Ĵ�������PWM������
	CCPR1L = 0X00; //����PWMռ�ձ�
	T2CON = 0B00000101; //����TMR2Ԥ��Ƶֵ��ʹ��Timer2

	OpenADC(ADC_FOSC_8 & ADC_RIGHT_JUST & ADC_2_TAD , ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS , 0);
	Delayus(10);
	//������ģת��
	
	INTCONbits.GIE=0;/*��ȫ���ж�*/
	RCONbits.IPEN=1; /*ʹ���ж����ȼ�*/
	T0CON=0b01001000;/*TMR0���ã�ֹͣ���С�8λ��ʱ��F=FOSC����Ԥ��Ƶ*/
	TMR0L=20;/*TMR0�ó�ֵ*/
	INTCONbits.TMR0IF=0;/*Timer0�����־����*/
	INTCONbits.TMR0IE=1;/*����Timer0����ж�*/
	INTCON2bits.TMR0IP=1;/*Timer0�ж�Ϊ�����ȼ�*/
	T0CONbits.TMR0ON=1;/*����TMR0*/
	INTCONbits.GIE=1;/*��ȫ���ж�*/

	while(1)
	{
		C1 = 0;
		C2 = 0;
		C3 = 0;
		C4 = 0;
		XS1();
		CCPR1L = CCPR1L + 4 * a; //ʹ�������
		XS1();
		if(CCPR1L >= 0x1E || CCPR1L <= 0x00){a = a * (-1);}
		XS1();
		GXHT30_write_cmd(addr, 0x2c, 0x0d);
		XS1();
		GXHT30_read_result(addr);
		XS1();
		Delay10us(10);
		XS1();
		p = ((int)Temperature / 10);
		q = ((int)Temperature % 10);
		k = ((int)Humidity / 10);
		l = ((int)Humidity % 10);
		
		
			ConvertADC(); //����ADCת��
			while(BusyADC()); //ѭ���ȴ�ADCת����ɣ����ADC�Ƿ���æµ
			result = ReadADC(); //��ȡADCת�����������洢�ڱ���result��
			result = (result /4) | 0x80; //����ȡ���Ľ������4����12λ���ŵ�8λ
			PORTB = TABLE[result];
			C1 = 1;
			C2 = 1;
			C3 = 1;
			C4 = 1;
			Delay10Ms(200);
	}
}


void XS1(void)
{
	C1 = 0;
	C2 = 0;
	C3 = 0;
	C4 = 0;
	PORTB = TABLE[p];
	C2 = 1;
	Delayus(2);
	
	C1 = 0;
	C2 = 0;
	C3 = 0;
	C4 = 0;
	PORTB = TABLE[q];
	C1 = 1;
	Delayus(2);
	
	C1 = 0;
	C2 = 0;
	C3 = 0;
	C4 = 0;
	PORTB = TABLE[k];
	C4 = 1;
	Delayus(2);
	
	C1 = 0;
	C2 = 0;
	C3 = 0;
	C4 = 0;
	PORTB = TABLE[l];
	C3 = 1;
	Delayus(2);
}

void PIC18F_High_isr(void);/*�жϷ���������*/
void PIC18F_Low_isr(void);

#pragma code high_vector_section=0x8
void high_vector (void)
{
	_asm goto PIC18F_High_isr _endasm/*ͨ��һ����תָ��(���ָ�����ת���жϷ��������жϷ�����򣩴�*/
}

#pragma code low_vector_section=0x18
void low_vector (void)
{
	_asm goto PIC18F_Low_isr _endasm
}

#pragma code

#pragma interrupt PIC18F_High_isr
void PIC18F_High_isr (void)
{
	TMR0L=20;/*TMR0�����ó�ֵ*/
	INTCONbits.TMR0IF=0;	/*TMR0�����־����*/ 
}


#pragma interruptlow PIC18F_Low_isr
void PIC18F_Low_isr (void)
{

}