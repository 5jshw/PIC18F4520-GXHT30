 //主程序

#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"
//#include <adc.h>
//#include <stdlib.h>

void K(void);
void G(void);

float Temperature;
float Humidity;

int a = 1;

unsigned int addr;
	
void main(void)
{
	OSCCON = 0b01110011; //8MHz振荡器频率，内部振荡器电路
	addr = 0x88; //传感器默认地址
	k18_init(); //初始化
	IIC_init();
	K();
	Delay10Ms(20);

	DDRCbits.RC2 = 0; //清零该接口的方向寄存器，用以将CCP1的引脚配置为输出
	CCP1CON = 0B00001100; //将CCP模块修改为PWM模式
	PR2 = 0x80; //通过PR2寄存器设置PWM的周期
	CCPR1L = 0X00; //设置PWM占空比
	T2CON = 0B00000101; //设置TMR2预分频值并使能Timer2

	while(1)
	{
		
		CCPR1L = CCPR1L + 8*a; //使脉宽递增
		Delay10Ms(20);
		if(CCPR1L >= 0x70 || CCPR1L <= 0x00){a = a * (-1);}
		
		GXHT30_write_cmd(addr, 0x2c, 0x0d);
		GXHT30_read_result(addr);
		Delay10Ms(2);
		if(Temperature <= 27){
			
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
	}

}


void K(void)
{
	L1 = 1;
	L2 = 1;
	L3 = 1;
	L4 = 1;
	L5 = 1;
	L6 = 1;
	L7 = 1;
}

void G(void)
{
	L1 = 0;
	L2 = 0;
	L3 = 0;
	L4 = 0;
	L5 = 0;
	L6 = 0;
	L7 = 0;
}


