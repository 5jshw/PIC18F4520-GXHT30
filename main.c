 //主程序

#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"
#include <math.h>

float Temperature;					//外部变量：温度
float Humidity;						//外部变量：湿度
const unsigned char TABLE[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F}; 	//0~9数码管字符
unsigned int cost1[3];				//AD转换变量数组
int a = 1, j = 0, w = 0;			//a：PWM脉宽递增递减指示器， j：AD转换启用指示器， w：AD转换数据稳定指示器
unsigned long ad10 = 0, ad11 = 0;		//ad1：单次AD转换寄存器， ad10：AD转换最终输出寄存器
unsigned int ad1;
unsigned int p = 0, q = 0;			//p：十位数变量， q：个位数变量
unsigned int k = 0, l = 0;			//k：十位数变量， l：个位数变量
unsigned int count = 0;				//数码管刷新指示器
unsigned int chosebit = 0;			//数码管选择指示器
int t, v, m;                		//t 电位器转换设定温度值， v NTC转换实际温度值， m 温度表查表指示器
unsigned long VR;           			//VR 当前电压值
unsigned long Rt;            		//Rt 当前动态电阻值
unsigned int const TABLE1[] = {9712, 9166, 8654, 8172, 7722, 7298, 6900, 6526, 6176, 5534, 
							   5242, 4966, 4708, 4464, 4234, 4016, 3812, 3620, 3438, 3266, 
                               3104, 2950, 2806, 2668, 2540, 2418, 2302, 2192, 2088, 1990, 
                               1897, 1809, 1726, 1646, 1571, 1500, 1432, 1368, 1307, 1249, 
                               1194, 1142, 1092, 1045, 1000, 957, 916, 877, 840, 805, 772, 
                               740, 709, 680, 653, 626, 601, 577, 554, 532, 511, 491, 472, 
                               454, 436, 420, 404, 388, 374, 360, 346, 334, 321, 309, 298, 
                               287, 277, 267, 258, 248
                              };  	//-20 ~ 60 温度对应电阻值，为了优化空间，所有值缩小一位数

void main(void)
{
	int i, b = 0, d = 1;
	GXHT30_init(); 					//初始化
	T0CONbits.TMR0ON = 1; 			//启动时钟0
	T2CONbits.TMR2ON = 1; 			//开启时钟2

	while(1)
	{
		//GXHT30_write_cmd(addr, 0x2c, 0x0d);		//向GXHT30发送控制命令（单次转换命令）
		//GXHT30_read_result(addr);					//向GXHT30发送读取数据命令
		//Delay10us(1);
		
		if(b >= 20)
		{
			if(d == 1)
			{
				ADCON0 = 0x01;			//AN0通道，接电位器
					d = -1;
			}
			else if(d == -1)
			{
				ADCON0 = 0x05;			//AN1通道，接NTC
					d = 1;
			}
			DelayMs(5);
			ADCON0bits.GO = 1; 			//开启AD转换，需要在读取完温度并显示后在开始AD转换， 否则温度报警灯异常
			DelayMs(5);
			
			if(v <= t)                  //当v实际温度小于等于t设定温度
			{
    			T1 = 1;                 //点亮报警灯
			}
			else if(v > t + 2)        	//当v实际温度大于t设定温度 - 2 度， 设定回差，避免温度临界跳动
			{
    			T1 = 0;                 //关闭报警灯
			}
			b = 0;
		}
		else
		{
			Delay10Ms(1);
			b++;
		}
		
	}
}


void PIC18F_High_isr(void);				//高优先级中断服务函数声明
void PIC18F_Low_isr(void);				//低优先级中断服务函数声明

#pragma code high_vector_section=0x8	//高优先级中断地址
void high_vector (void)
{
	_asm goto PIC18F_High_isr _endasm	//通过一条跳转指令(汇编指令），跳转到中断服务函数（中断服务程序）处
}

#pragma code low_vector_section=0x18	//低优先级中断地址
void low_vector (void)
{
	_asm goto PIC18F_Low_isr _endasm	//通过一条跳转指令(汇编指令），跳转到中断服务函数（中断服务程序）处
}


#pragma code							//指示编译器回到默认的代码段,确保后续代码被正确放置


#pragma interrupt PIC18F_High_isr		//高优先级中断
void PIC18F_High_isr (void)	
{
	if(PIR1bits.ADIF == 1)				//检测是否发生了AD转换中断
	{
		PIE1bits.ADIE = 0;				//禁止AD转换中断
		PIR1bits.ADIF = 0;				//清除AD转换中断标志
		if(ADCON0 == 0x01)				//电位器
		{
			ad1 = ADRESH;
			ad1 = (ad1 << 2) | (ADRESL >> 6);
			t = (unsigned int)(ad1 / 10 * 0.785);
			p = t / 10;
			q = t % 10;
			
		}
		else if (ADCON0 == 0x05)		//NTC
		{
			ad10 = ADRESH;
			ad10 = (ad10 << 2) | (ADRESL >> 6);
			ad11 = 1024 - ad10;
			VR = (unsigned long)(ad11 * 500 / 1024);		//转换成电压值
			Rt = (unsigned long)((500 - VR) * 1000 / VR);      //计算NTC当前的动态电阻值

			for(m = 0; m < 80; m++)     //将转换后的动态电阻值与TABLE数组中的元素依次比较
			{
				if(Rt >= TABLE1[m])		//温度越低，电阻越高，因此，在数据表中依据电阻值从高到低进行查找
				{
					v = m;              //另存变量，防止覆盖
					break;              //查找到所需数据后跳出循环
				}
			}
			k = v / 10;
			l = v % 10;
/*
			k = (unsigned int)ad10 / 1000; //1000 
			l = (unsigned int)ad10 % 1000 / 100; //100
			p = (unsigned int)ad10 % 100 / 10; //10
			q = (unsigned int)ad10 % 10; //1
*/
		}
	}
	PIE1bits.ADIE = 1;				//允许AD转换中断
}


#pragma interruptlow PIC18F_Low_isr		//低优先级中断
void PIC18F_Low_isr (void)
{
	if(PIR1bits.TMR2IF == 1)			//判断定时器2计数值与pr2是否匹配中断		LED数码管
    {
        PIE1bits.TMR2IE = 0;			//禁止时钟2匹配中断
        T2CONbits.TMR2ON = 0;			//停止计数
        PIR1bits.TMR2IF = 0;			//清除时钟2匹配中断标志
        count++;						//计数器加1， 
        
        if(count >= 10)					//200us*25=5ms(200HZ), 每十五次中断更换一次字符与数码管
        {
            count = 0;					//清空count计数
            C1 = 1;						//对于数码管，数据接口与公共端都连接到单片机上
            C2 = 1;						//所以， 一端输入， 另一端就为输出
            C3 = 1;						//在这个程序中， 数据端输出为1
            C4 = 1;						//因此， 公共端接通需为0， 为1则关闭
            switch(chosebit)			//多选择函数
            {
            case 0:
                C1 = 0;					//选通第一位数码
                PORTB = TABLE[q];		//送字型
                chosebit = 1;
                break;
            case 1:
                C2 = 0;					//选通第二位数码
                PORTB = TABLE[p];		//送字型
                chosebit = 2;
                break;
            case 2:
                C3 = 0;					//选通第三位数码
                PORTB = TABLE[l];		//送字型
                chosebit = 3;
                break;
            case 3:
                C4 = 0;					//选通第四位数码
                PORTB = TABLE[k];		//送字型
                chosebit = 0;
                break;
            default:
                break;
            }
        }
        TMR2 = 0x00;					//时钟2清零
        T2CONbits.TMR2ON = 1;			//开启时钟2
        PIE1bits.TMR2IE = 1;			//允许时钟2匹配中断
    }

    if(INTCONbits.TMR0IF == 1)			//检测时钟0是否溢出中断						PWM脉冲
	{
		INTCONbits.TMR0IF = 0; 			//时钟0溢出标志清零
		INTCONbits.TMR0IE = 0;			//允许时钟0溢出中断
		
		if(CCPR1L >= PR2)				//检测脉宽是否超过周期最大值
		{
			a = -1;						//是， 则开始递减
		}
		else if(CCPR1L <= 0x00)			//检测脉宽是否低于周期最小值
		{
			a = 1;						//是， 则开始递增递减
		}
		
		if(a == 1)
		{
			CCPR1L = CCPR1L + 5;		//使脉宽递增
		}
		else if(a == -1)
		{
			CCPR1L = CCPR1L - 5;		//使脉宽递减
		}
		
		INTCONbits.TMR0IF = 0; 			//Timer0溢出标志清零
		INTCONbits.TMR0IE = 1;			//允许Timer0溢出中断
	}
}