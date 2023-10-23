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

#define COL1			PORTAbits.RA0
#define TRIS_COL1		DDRAbits.RA0

#define SDA				PORTAbits.RA1
#define TRIS_SDA		DDRAbits.RA1

#define SCL				PORTAbits.RA2
#define TRIS_SCL		DDRAbits.RA2

#define L1				PORTBbits.RB0
#define TRIS_L1			DDRBbits.RB0

#define L2				PORTBbits.RB1
#define TRIS_L2			DDRBbits.RB1

#define L3				PORTBbits.RB2
#define TRIS_L3			DDRBbits.RB2

#define L4				PORTBbits.RB3
#define TRIS_L4			DDRBbits.RB3

#define L5				PORTBbits.RB4
#define TRIS_L5			DDRBbits.RB4

#define L6				PORTBbits.RB5
#define TRIS_L6			DDRBbits.RB5

#define L7				PORTBbits.RB6
#define TRIS_L7			DDRBbits.RB6

//ϵͳ��ʼ������
void k18_init(void); //HL-K18 �����ʼ��

#endif