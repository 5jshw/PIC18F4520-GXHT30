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
	OSCCON = 0b01110011; //8MHz振荡器频率，内部振荡器电路
	addr = 0x88; //传感器默认地址
	GXHT30_init(); //初始化
	L1 = 1;
	

	T2CON = 0x01;   //postscale 1:1 prescale 1:4
    PR2 = 250;      //匹配寄存器   1*4*250*200ns = 200us
    TMR2 = 0x00;    //计数器清零
    RCONbits.IPEN = 0;
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    INTCONbits.PEIE = 1;           //开外设中断
    INTCONbits.GIE = 1;            //开总中断
	T2CONbits.TMR2ON = 1; 

/*
	OpenADC(ADC_FOSC_8 & ADC_RIGHT_JUST & ADC_2_TAD , ADC_CH0 & ADC_INT_ON & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS , 0);
	Delayus(10);

	//INTCONbits.GIE=0; //关全局中断
	//RCONbits.IPEN=1; //使能中断优先级
	T0CON=0b01001000; //TMR0设置：停止运行、8位定时，F=FOSC，无预分频
	TMR0L=20; //TMR0置初值
	INTCONbits.TMR0IF=0; //Timer0溢出标志清零
	INTCONbits.TMR0IE=1; //允许Timer0溢出中断
	INTCON2bits.TMR0IP=1; //Timer0中断为高优先级
	T0CONbits.TMR0ON=1; //启动TMR0
	//INTCONbits.GIE=1; //开全局中断
*/
/*	呼吸灯配置
	DDRCbits.RC2 = 0; //清零该接口的方向寄存器，用以将CCP1的引脚配置为输出
	CCP1CON = 0B00001100; //将CCP模块修改为PWM模式
	PR2 = 0x20; //通过PR2寄存器设置PWM的周期
	CCPR1L = 0X00; //设置PWM占空比
	T2CON = 0B00000101; //设置TMR2预分频值并使能Timer2
*/

	while(1)
	{
/*		呼吸灯运行
		CCPR1L = CCPR1L + 1 * a; //使脉宽递增
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
		ConvertADC(); //启动ADC转换
		while(BusyADC()); //循环等待ADC转换完成，检查ADC是否在忙碌
		result = ReadADC(); //读取ADC转换结果，将其存储在变量result中
		result = (result /4); //将读取到的结果除以4，将12位缩放到8位
		PORTB = result;
*/
	}
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
	if(PIR1bits.TMR2IF == 1)              //定时器2计数值与pr2匹配
    {
        PIE1bits.TMR2IE = 0;
        T2CONbits.TMR2ON = 0;         //停止计数
        PIR1bits.TMR2IF = 0;
        count++;            //计数器加1
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
                C1 = 1;;  //选通第一位数码
                PORTB = TABLE[q];      //送字型
                chosebit = 1;
                break;
            case 1:
                C2 = 1;  //选通第二位数码
                PORTB = TABLE[p];      //送字型
                chosebit = 2;
                break;
            case 2:
                C3 = 1;  //选通第三位数码
                PORTB = TABLE[l];      //送字型
                chosebit = 3;
                break;
            case 3:
                C4 = 1;  //选通第四位数码
                PORTB = TABLE[k];      //送字型
                chosebit = 0;
                break;
            default:
                break;
            }
        }
        if(count2 >= 5000) //当前显示数字驻留 5000/100*(4*5)mS=1S
        {
            i++;
            if(i == 4)
            {
                i = 0;
            }
            count2 = 0;
        }
        TMR2 = 0x00;    //计数器清零
        T2CONbits.TMR2ON = 1;
        PIE1bits.TMR2IE = 1;
    }	
}


#pragma interruptlow PIC18F_Low_isr
void PIC18F_Low_isr (void)
{
	
}
	
    
