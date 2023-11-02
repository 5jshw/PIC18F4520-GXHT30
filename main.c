 //������

#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

float Temperature;
float Humidity;
const unsigned char TABLE[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};
unsigned int cost1[3];
int a = 1, j = 0, w = 0;
int ad1, ad2, ad3, ad4, ad10;
unsigned char p, q;
unsigned char k, l;
unsigned int addr;
int result;
int f1 = 1;
unsigned int count = 0;
unsigned int count2 = 0;
unsigned char chosebit = 0;
unsigned float i = 1;

void main(void)
{
	GXHT30_init(); //��ʼ��
	OSCCON = 0b01110011; //8MHz����Ƶ�ʣ��ڲ�������·
	addr = 0x88; //������Ĭ�ϵ�ַ
	k = 0;
	l = 0;

	ADCON0 = 0x01;					//ѡ��AN0ͨ��������ADת����
	ADCON1 = 0x0D;					//�ο���ѹ����ΪVDD��VSS��AN1,AN2����Ϊģ�������
	ADCON2 = 0x21;					//AD�������룬�ɼ�ʱ�� 8TAD��ת��ʱ��FOSC/8	
	
	DDRCbits.RC2 = 0; 				//����ýӿڵķ��򣬽�CCP1����������Ϊ���
	CCP1CON = 0B00001100; 			//��CCPģ���޸�ΪPWMģʽ
	CCPR1L = 0X00; 					//����PWM��ʼռ�ձ�
	
	T0CON = 0b00001111; 			//16λ��ʱ���ڲ�ʱ��Դ��256Ԥ��Ƶ
	T2CON = 0x01;  					//postscale 1:1 prescale 1:4
	
    PR2 = 150;      				//ƥ��Ĵ���, ʱ��2ƥ���жϣ�PWM����
    
    TMR2 = 0x00;    				//����������
    
    RCONbits.IPEN = 1;				//�����ж����ȼ�
    
    PIR1bits.ADIF = 0;				//��ʼ��ADת���жϱ�־
	PIE1bits.ADIE = 1;				//����ADת���ж�
    PIE1bits.TMR2IE = 1;			//����ʱ��2ƥ���ж�
    PIR1bits.TMR2IF = 0;			//��ʼ��ƥ���жϱ�־
    
    IPR1bits.TMR2IP = 0;			//����ʱ��2Ϊ�����ȼ��ж�
	INTCON2bits.TMR0IP = 0;			//����ʱ��0Ϊ�͸����ȼ��ж�
	
	INTCONbits.TMR0IF = 0; 			//Timer0�����־����
	INTCONbits.TMR0IE = 1;			//����Timer0����ж�
	
	
	INTCONbits.PEIE = 1;        	//�������ж�
    INTCONbits.GIE = 1;         	//�����ж�
	
	T0CONbits.TMR0ON = 1; 			//����ʱ��0
	T2CONbits.TMR2ON = 1; 			//����ʱ��2
	

	while(1)
	{
		
		
		GXHT30_write_cmd(addr, 0x2c, 0x0d);
		GXHT30_read_result(addr);
		Delay10us(5);
		p = ((int)Temperature / 10); //10
		q = ((int)Temperature % 10); //1
		//k = ((int)Humidity / 10);  //10
		//l = ((int)Humidity % 10); //1
		
		if((ad10 <= (int)Temperature) && (ad10 != 0))
		{
			T1 = 1;
		}
		else if(ad10 - 2 >= (int)Temperature)
		{
			T1 = 0;
		}


		j++;
		if(j = 5000)
		{
			ADCON0bits.GO=1; 		//����ADת������
			j = 0;
		}
		
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


#pragma interrupt PIC18F_High_isr		//�����ȼ��ж�
void PIC18F_High_isr (void)	
{
	if(PIR1bits.ADIF == 1)
	{
		PIE1bits.ADIE = 0;
		PIR1bits.ADIF = 0;
		
		if(w >= 3) //��β������ȶ�����
		{
			ad10 = cost1[0] + cost1[1] + cost1[2];
			ad10 = ad10 / 3;
			k = ad10 /10;
			l = ad10 % 10;
			w = 0;
			
			PIR1bits.ADIF = 0;				//��ʼ��ADת���жϱ�־
			PIE1bits.ADIE = 1;				//����ADת���ж�
		}
		else //���β���
		{
			ad1 = 0;
			ad1 = ADRESH;
			ad1 = ad1 << 2;
			ad2 = ad1 | (ADRESL >> 6);
			ad3 = (ad2 / 10) * 0.781 - 20;
			if(ad3 < 0xFF00)
			{
				ad4 = ad3;
			}
			else if(ad3 > 0xFF00)
			{
				ad4 = - (ad3 & 0x001E) ^ 0x001F;
			}
			
			cost1[w] = (int)ad4;
			w++;
			
			PIR1bits.ADIF = 0;				//��ʼ��ADת���жϱ�־
			PIE1bits.ADIE = 1;				//����ADת���ж�
		}
	}
	
	
}


#pragma interruptlow PIC18F_Low_isr		//�����ȼ��ж�
void PIC18F_Low_isr (void)
{
	if(PIR1bits.TMR2IF == 1)              //��ʱ��2����ֵ��pr2ƥ��
    {
        PIE1bits.TMR2IE = 0;
        T2CONbits.TMR2ON = 0;         //ֹͣ����
        PIR1bits.TMR2IF = 0;
        count++;            //��������1
        count2++;
        if(count >= 15)     //200us*25=5ms(200HZ)
        {
            count = 0;
            C1 = 1;
            C2 = 1;
            C3 = 1;
            C4 = 1;
            switch(chosebit)
            {
            case 0:
                C1 = 0;  //ѡͨ��һλ����
                PORTB = TABLE[q];      //������
                chosebit = 1;
                break;
            case 1:
                C2 = 0;  //ѡͨ�ڶ�λ����
                PORTB = TABLE[p];      //������
                chosebit = 2;
                break;
            case 2:
                C3 = 0;  //ѡͨ����λ����
                PORTB = TABLE[l];      //������
                chosebit = 3;
                break;
            case 3:
                C4 = 0;  //ѡͨ����λ����
                PORTB = TABLE[k];      //������
                chosebit = 0;
                break;
            default:
                break;
            }
        }
        if(count2 >= 500) //��ǰ��ʾ����פ�� 5000/100*(4*5)mS=1S
        {
            count2 = 0;
        }
        TMR2 = 0x00;    //����������
        T2CONbits.TMR2ON = 1;
        PIE1bits.TMR2IE = 1;
    }
    
    if(INTCONbits.TMR0IF == 1)
	{
		INTCONbits.TMR0IF = 0; 			//Timer0�����־����
		INTCONbits.TMR0IE = 0;			//����Timer0����ж�
	
		if(CCPR1L >= PR2)
		{
			a = -1;
		}
		else if(CCPR1L <= 0x00)
		{
			a = 1;
		}
		
		if(a == 1)
		{
			CCPR1L = CCPR1L + 2; //ʹ�������
		}
		else if(a == -1)
		{
			CCPR1L = CCPR1L - 2; //ʹ�������
		}
		
		INTCONbits.TMR0IF = 0; 			//Timer0�����־����
		INTCONbits.TMR0IE = 1;			//����Timer0����ж�
	}
	
}
	
    
