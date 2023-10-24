 //主程序

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
	OSCCON = 0b01110011; //8MHz振荡器频率，内部振荡器电路
	addr = 0x88; //传感器默认地址
	GXHT30_init(); //初始化

	DDRCbits.RC2 = 0; //清零该接口的方向寄存器，用以将CCP1的引脚配置为输出
	CCP1CON = 0B00001100; //将CCP模块修改为PWM模式
	PR2 = 0x20; //通过PR2寄存器设置PWM的周期
	CCPR1L = 0X00; //设置PWM占空比
	T2CON = 0B00000101; //设置TMR2预分频值并使能Timer2

	OpenADC(ADC_FOSC_8 & ADC_RIGHT_JUST & ADC_2_TAD , ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS , 0);
	Delayus(10);
	//进行数模转换
	
	INTCONbits.GIE=0;/*关全局中断*/
	RCONbits.IPEN=1; /*使能中断优先级*/
	T0CON=0b01001000;/*TMR0设置：停止运行、8位定时，F=FOSC，无预分频*/
	TMR0L=20;/*TMR0置初值*/
	INTCONbits.TMR0IF=0;/*Timer0溢出标志清零*/
	INTCONbits.TMR0IE=1;/*允许Timer0溢出中断*/
	INTCON2bits.TMR0IP=1;/*Timer0中断为高优先级*/
	T0CONbits.TMR0ON=1;/*启动TMR0*/
	INTCONbits.GIE=1;/*开全局中断*/

	while(1)
	{
		C1 = 0;
		C2 = 0;
		C3 = 0;
		C4 = 0;
		XS1();
		CCPR1L = CCPR1L + 4 * a; //使脉宽递增
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
		
		
			ConvertADC(); //启动ADC转换
			while(BusyADC()); //循环等待ADC转换完成，检查ADC是否在忙碌
			result = ReadADC(); //读取ADC转换结果，将其存储在变量result中
			result = (result /4) | 0x80; //将读取到的结果除以4，将12位缩放到8位
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

void PIC18F_High_isr(void);/*中断服务函数声明*/
void PIC18F_Low_isr(void);

#pragma code high_vector_section=0x8
void high_vector (void)
{
	_asm goto PIC18F_High_isr _endasm/*通过一条跳转指令(汇编指令），跳转到中断服务函数（中断服务程序）处*/
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
	TMR0L=20;/*TMR0重新置初值*/
	INTCONbits.TMR0IF=0;	/*TMR0溢出标志清零*/ 
}


#pragma interruptlow PIC18F_Low_isr
void PIC18F_Low_isr (void)
{

}