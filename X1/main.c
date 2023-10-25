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
//int a = 1;
unsigned char p, q;
unsigned char k, l;
unsigned int addr;
int result;
int f1 = 1;
unsigned int count = 0;
unsigned int count2 = 0;
unsigned char chosebit = 0;
unsigned int i = 0;

void main(void)
{
	OSCCON = 0b01110011; //8MHz����Ƶ�ʣ��ڲ�������·
	addr = 0x88; //������Ĭ�ϵ�ַ
	GXHT30_init(); //��ʼ��
	L1 = 1;
	

	T2CON = 0x01;   //postscale 1:1 prescale 1:4
    PR2 = 250;      //ƥ��Ĵ���   1*4*250*200ns = 200us
    TMR2 = 0x00;    //����������
    RCONbits.IPEN = 0;
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    INTCONbits.PEIE = 1;           //�������ж�
    INTCONbits.GIE = 1;            //�����ж�
	T2CONbits.TMR2ON = 1; 

/*
	OpenADC(ADC_FOSC_8 & ADC_RIGHT_JUST & ADC_2_TAD , ADC_CH0 & ADC_INT_ON & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS , 0);
	Delayus(10);

	//INTCONbits.GIE=0; //��ȫ���ж�
	//RCONbits.IPEN=1; //ʹ���ж����ȼ�
	T0CON=0b01001000; //TMR0���ã�ֹͣ���С�8λ��ʱ��F=FOSC����Ԥ��Ƶ
	TMR0L=20; //TMR0�ó�ֵ
	INTCONbits.TMR0IF=0; //Timer0�����־����
	INTCONbits.TMR0IE=1; //����Timer0����ж�
	INTCON2bits.TMR0IP=1; //Timer0�ж�Ϊ�����ȼ�
	T0CONbits.TMR0ON=1; //����TMR0
	//INTCONbits.GIE=1; //��ȫ���ж�
*/
/*	����������
	DDRCbits.RC2 = 0; //����ýӿڵķ���Ĵ��������Խ�CCP1����������Ϊ���
	CCP1CON = 0B00001100; //��CCPģ���޸�ΪPWMģʽ
	PR2 = 0x20; //ͨ��PR2�Ĵ�������PWM������
	CCPR1L = 0X00; //����PWMռ�ձ�
	T2CON = 0B00000101; //����TMR2Ԥ��Ƶֵ��ʹ��Timer2
*/

	while(1)
	{
/*		����������
		CCPR1L = CCPR1L + 1 * a; //ʹ�������
		if(CCPR1L >= 0x1E || CCPR1L <= 0x00){a = a * (-1);}
*/
	
		GXHT30_write_cmd(addr, 0x2c, 0x0d);
		GXHT30_read_result(addr);
		Delay10us(5);
		p = ((int)Temperature / 10);
		q = ((int)Temperature % 10);
		k = ((int)Humidity / 10);
		l = ((int)Humidity % 10);
	
/*
		ConvertADC(); //����ADCת��
		while(BusyADC()); //ѭ���ȴ�ADCת����ɣ����ADC�Ƿ���æµ
		result = ReadADC(); //��ȡADCת�����������洢�ڱ���result��
		result = (result /4); //����ȡ���Ľ������4����12λ���ŵ�8λ
		PORTB = result;
*/
	}
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
	if(PIR1bits.TMR2IF == 1)              //��ʱ��2����ֵ��pr2ƥ��
    {
        PIE1bits.TMR2IE = 0;
        T2CONbits.TMR2ON = 0;         //ֹͣ����
        PIR1bits.TMR2IF = 0;
        count++;            //��������1
        count2++;
        if(count >= 10)     //200us*25=5ms(200HZ)
        {
            count = 0;
            C1 = 0;
            C2 = 0;
            C3 = 0;
            C4 = 0;
            switch(chosebit)
            {
            case 0:
                C1 = 1;;  //ѡͨ��һλ����
                PORTB = TABLE[q];      //������
                chosebit = 1;
                break;
            case 1:
                C2 = 1;  //ѡͨ�ڶ�λ����
                PORTB = TABLE[p];      //������
                chosebit = 2;
                break;
            case 2:
                C3 = 1;  //ѡͨ����λ����
                PORTB = TABLE[l];      //������
                chosebit = 3;
                break;
            case 3:
                C4 = 1;  //ѡͨ����λ����
                PORTB = TABLE[k];      //������
                chosebit = 0;
                break;
            default:
                break;
            }
        }
        if(count2 >= 5000) //��ǰ��ʾ����פ�� 5000/100*(4*5)mS=1S
        {
            i++;
            if(i == 4)
            {
                i = 0;
            }
            count2 = 0;
        }
        TMR2 = 0x00;    //����������
        T2CONbits.TMR2ON = 1;
        PIE1bits.TMR2IE = 1;
    }	
}


#pragma interruptlow PIC18F_Low_isr
void PIC18F_Low_isr (void)
{
	
}
	
    
