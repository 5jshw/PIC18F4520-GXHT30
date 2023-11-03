//主板初始化文件
#include <p18cxxx.h>
#include "k18.h"
#include "GXHT30.h"

void GXHT30_init(void)		//初始化
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
	
	OSCCON = 0x73; 			//8MHz振荡器频率，内部振荡器电路
	ADCON0 = 0x01;			//选择AN0通道，开启AD转换器
	ADCON1 = 0x0D;			//参考电压设置为VDD、VSS，AN1,AN2设置为模拟输入口
	ADCON2 = 0x21;			//AD结果左对齐，采集时间 8TAD，转换时钟FOSC/8	

	DDRCbits.RC2 = 0; 		//清零该接口的方向，将CCP1的引脚配置为输出
	CCP1CON = 0x0C; 		//将CCP模块修改为PWM模式
	CCPR1L = 0x00; 			//设置PWM初始占空比
	
	T0CON = 0x0F; 			//16位定时，内部时钟源，256预分频
	T2CON = 0x01;  			//postscale 1:1 prescale 1:4
	
	PR2 = 150;      		//匹配寄存器, 时钟2匹配中断，PWM周期
    
	TMR2 = 0x00;    		//计数器清零
    
	RCONbits.IPEN = 1;		//开启中断优先级
    
	PIR1bits.ADIF = 0;		//初始化AD转换中断标志
	PIE1bits.ADIE = 1;		//允许AD转换中断
	PIE1bits.TMR2IE = 1;	//允许时钟2匹配中断
	PIR1bits.TMR2IF = 0;	//初始化匹配中断标志
  
	IPR1bits.TMR2IP = 0;	//设置时钟2为低优先级中断
	INTCON2bits.TMR0IP = 0;	//设置时钟0为低高优先级中断
	
	INTCONbits.TMR0IF = 0; 	//Timer0溢出标志清零
	INTCONbits.TMR0IE = 1;	//允许Timer0溢出中断
	
	INTCONbits.PEIE = 1;       //开外设中断
	INTCONbits.GIE = 1;        //开总中断
}