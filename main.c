 //主程序

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
	GXHT30_init(); //初始化
	OSCCON = 0b01110011; //8MHz振荡器频率，内部振荡器电路
	addr = 0x88; //传感器默认地址
	k = 0;
	l = 0;

	ADCON0 = 0x01;					//选择AN0通道，开启AD转换器
	ADCON1 = 0x0D;					//参考电压设置为VDD、VSS，AN1,AN2设置为模拟输入口
	ADCON2 = 0x21;					//AD结果左对齐，采集时间 8TAD，转换时钟FOSC/8	
	
	DDRCbits.RC2 = 0; 				//清零该接口的方向，将CCP1的引脚配置为输出
	CCP1CON = 0B00001100; 			//将CCP模块修改为PWM模式
	CCPR1L = 0X00; 					//设置PWM初始占空比
	
	T0CON = 0b00001111; 			//16位定时，内部时钟源，256预分频
	T2CON = 0x01;  					//postscale 1:1 prescale 1:4
	
    PR2 = 150;      				//匹配寄存器, 时钟2匹配中断，PWM周期
    
    TMR2 = 0x00;    				//计数器清零
    
    RCONbits.IPEN = 1;				//开启中断优先级
    
    PIR1bits.ADIF = 0;				//初始化AD转换中断标志
	PIE1bits.ADIE = 1;				//允许AD转换中断
    PIE1bits.TMR2IE = 1;			//允许时钟2匹配中断
    PIR1bits.TMR2IF = 0;			//初始化匹配中断标志
    
    IPR1bits.TMR2IP = 0;			//设置时钟2为低优先级中断
	INTCON2bits.TMR0IP = 0;			//设置时钟0为低高优先级中断
	
	INTCONbits.TMR0IF = 0; 			//Timer0溢出标志清零
	INTCONbits.TMR0IE = 1;			//允许Timer0溢出中断
	
	
	INTCONbits.PEIE = 1;        	//开外设中断
    INTCONbits.GIE = 1;         	//开总中断
	
	T0CONbits.TMR0ON = 1; 			//启动时钟0
	T2CONbits.TMR2ON = 1; 			//开启时钟2
	

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
			ADCON0bits.GO=1; 		//开启AD转换过程
			j = 0;
		}
		
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


#pragma interrupt PIC18F_High_isr		//高优先级中断
void PIC18F_High_isr (void)	
{
	if(PIR1bits.ADIF == 1)
	{
		PIE1bits.ADIE = 0;
		PIR1bits.ADIF = 0;
		
		if(w >= 3) //多次测量，稳定数据
		{
			ad10 = cost1[0] + cost1[1] + cost1[2];
			ad10 = ad10 / 3;
			k = ad10 /10;
			l = ad10 % 10;
			w = 0;
			
			PIR1bits.ADIF = 0;				//初始化AD转换中断标志
			PIE1bits.ADIE = 1;				//允许AD转换中断
		}
		else //单次测量
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
			
			PIR1bits.ADIF = 0;				//初始化AD转换中断标志
			PIE1bits.ADIE = 1;				//允许AD转换中断
		}
	}
	
	
}


#pragma interruptlow PIC18F_Low_isr		//低优先级中断
void PIC18F_Low_isr (void)
{
	if(PIR1bits.TMR2IF == 1)              //定时器2计数值与pr2匹配
    {
        PIE1bits.TMR2IE = 0;
        T2CONbits.TMR2ON = 0;         //停止计数
        PIR1bits.TMR2IF = 0;
        count++;            //计数器加1
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
                C1 = 0;  //选通第一位数码
                PORTB = TABLE[q];      //送字型
                chosebit = 1;
                break;
            case 1:
                C2 = 0;  //选通第二位数码
                PORTB = TABLE[p];      //送字型
                chosebit = 2;
                break;
            case 2:
                C3 = 0;  //选通第三位数码
                PORTB = TABLE[l];      //送字型
                chosebit = 3;
                break;
            case 3:
                C4 = 0;  //选通第四位数码
                PORTB = TABLE[k];      //送字型
                chosebit = 0;
                break;
            default:
                break;
            }
        }
        if(count2 >= 500) //当前显示数字驻留 5000/100*(4*5)mS=1S
        {
            count2 = 0;
        }
        TMR2 = 0x00;    //计数器清零
        T2CONbits.TMR2ON = 1;
        PIE1bits.TMR2IE = 1;
    }
    
    if(INTCONbits.TMR0IF == 1)
	{
		INTCONbits.TMR0IF = 0; 			//Timer0溢出标志清零
		INTCONbits.TMR0IE = 0;			//允许Timer0溢出中断
	
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
			CCPR1L = CCPR1L + 2; //使脉宽递增
		}
		else if(a == -1)
		{
			CCPR1L = CCPR1L - 2; //使脉宽递增
		}
		
		INTCONbits.TMR0IF = 0; 			//Timer0溢出标志清零
		INTCONbits.TMR0IE = 1;			//允许Timer0溢出中断
	}
	
}
	
    
