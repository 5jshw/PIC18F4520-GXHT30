//�����ʼ���ļ�
#include <p18cxxx.h>
#include "k18.h"
#include "GXHT30.h"

void GXHT30_init(void)		//��ʼ��
{
	SDA = 0;
	T_SDA = 0;
	SCL = 0;
	T_SCL = 0;
	
	T_AD = 1;
	T_T1 = 0;
	T_L1 = 0;
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
	
	AD = 0;
	T1 = 0;
	L1 = 0;
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
	
	OSCCON = 0x73; 			//8MHz����Ƶ�ʣ��ڲ�������·
	ADCON0 = 0x01;			//ѡ��AN0ͨ��������ADת����
	ADCON1 = 0x0D;			//�ο���ѹ����ΪVDD��VSS��AN1,AN2����Ϊģ�������
	ADCON2 = 0x21;			//AD�������룬�ɼ�ʱ�� 8TAD��ת��ʱ��FOSC/8	

	DDRCbits.RC2 = 0; 		//����ýӿڵķ��򣬽�CCP1����������Ϊ���
	CCP1CON = 0x0C; 		//��CCPģ���޸�ΪPWMģʽ
	CCPR1L = 0x00; 			//����PWM��ʼռ�ձ�
	
	T0CON = 0x0F; 			//16λ��ʱ���ڲ�ʱ��Դ��256Ԥ��Ƶ
	T2CON = 0x01;  			//postscale 1:1 prescale 1:4
	
	PR2 = 150;      		//ƥ��Ĵ���, ʱ��2ƥ���жϣ�PWM����
    
	TMR2 = 0x00;    		//����������
    
	RCONbits.IPEN = 1;		//�����ж����ȼ�
    
	PIR1bits.ADIF = 0;		//��ʼ��ADת���жϱ�־
	PIE1bits.ADIE = 1;		//����ADת���ж�
	PIE1bits.TMR2IE = 1;	//����ʱ��2ƥ���ж�
	PIR1bits.TMR2IF = 0;	//��ʼ��ƥ���жϱ�־
  
	IPR1bits.TMR2IP = 0;	//����ʱ��2Ϊ�����ȼ��ж�
	INTCON2bits.TMR0IP = 0;	//����ʱ��0Ϊ�͸����ȼ��ж�
	
	INTCONbits.TMR0IF = 0; 	//Timer0�����־����
	INTCONbits.TMR0IE = 1;	//����Timer0����ж�
	
	INTCONbits.PEIE = 1;       //�������ж�
	INTCONbits.GIE = 1;        //�����ж�
}