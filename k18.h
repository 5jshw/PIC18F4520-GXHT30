//主板K18

#ifndef __k18_h
#define __k18_h

//变量类型标识的宏定义
#define Uchar unsigned char
#define Uint unsigned int
#define OUT 0
#define IN 1

#define Fosc 8000000 //定义晶振频率（单位Hz）

//k 引脚定义

#define SDA		PORTDbits.RD0
#define T_SDA	DDRDbits.RD0

#define SCL		PORTDbits.RD1
#define T_SCL	DDRDbits.RD1

#define L1		PORTBbits.RB0
#define T_L1	DDRBbits.RB0

#define L2		PORTBbits.RB1
#define T_L2	DDRBbits.RB1

#define L3		PORTBbits.RB2
#define T_L3	DDRBbits.RB2

#define L4		PORTBbits.RB3
#define T_L4	DDRBbits.RB3

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

#define C1		PORTEbits.RE1
#define T_C1	DDREbits.RE1

#define C2		PORTEbits.RE2
#define T_C2	DDREbits.RE2

#define C3		PORTCbits.RC0
#define T_C3	DDRCbits.RC0

#define C4		PORTCbits.RC1
#define T_C4	DDRCbits.RC1

//系统初始化函数
void k18_init(void); //HL-K18 主板初始化
void K1(void);
void G1(void);
void SMG_O(void);
void SMG_L(void);
#endif