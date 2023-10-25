//����K18

#ifndef __k18_h
#define __k18_h

//�������ͱ�ʶ�ĺ궨��
#define Uchar unsigned char
#define Uint unsigned int
#define OUT 0
#define IN 1

#define Fosc 8000000 //���徧��Ƶ�ʣ���λHz��

//k ���Ŷ���

#define D1		PORTAbits.RA0
#define T_D1	DDRAbits.RA0

#define D2		PORTAbits.RA1
#define T_D2	DDRAbits.RA1

#define L1		PORTCbits.RC2
#define T_L1	DDRCbits.RC2

#define SDA		PORTDbits.RD0
#define T_SDA	DDRDbits.RD0

#define SCL		PORTDbits.RD1
#define T_SCL	DDRDbits.RD1

#define A		PORTBbits.RB0
#define T_A		DDRBbits.RB0

#define B		PORTBbits.RB1
#define T_B		DDRBbits.RB1

#define C		PORTBbits.RB2
#define T_C		DDRBbits.RB2

#define D		PORTBbits.RB3
#define T_D		DDRBbits.RB3

#define E		PORTBbits.RB4
#define T_E		DDRBbits.RB4

#define F		PORTBbits.RB5
#define T_F		DDRBbits.RB5

#define G		PORTBbits.RB6
#define T_G		DDRBbits.RB6

#define C1		PORTDbits.RD2
#define T_C1	DDRDbits.RD2

#define C2		PORTDbits.RD3
#define T_C2	DDRDbits.RD3

#define C3		PORTCbits.RC4
#define T_C3	DDRCbits.RC4

#define C4		PORTCbits.RC5
#define T_C4	DDRCbits.RC5

//ϵͳ��ʼ������
void GXHT30_init(void); //HL-K18 �����ʼ��

#endif