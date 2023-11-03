 //主程序

#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"

float Temperature;					//外部变量：温度
float Humidity;						//外部变量：湿度
const unsigned char TABLE[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F}; 	//0~9数码管字符
unsigned int cost1[3];				//AD转换变量数组
int a = 1, j = 0, w = 0;			//a：PWM脉宽递增递减指示器， j：AD转换启用指示器， w：AD转换数据稳定指示器
int ad1 = 0, ad10 = 0;				//ad1：单次AD转换寄存器， ad10：AD转换最终输出寄存器
unsigned char p = 0, q = 0;			//p：十位数变量， q：个位数变量
unsigned char k = 0, l = 0;			//k：十位数变量， l：个位数变量
unsigned int addr;					//GXHT30在I2C总线上的地址
unsigned int count = 0;				//数码管刷新指示器
unsigned char chosebit = 0;			//数码管选择指示器

void main(void)
{
	GXHT30_init(); 					//初始化
	addr = 0x88; 					//传感器默认地址

	T0CONbits.TMR0ON = 1; 			//启动时钟0
	T2CONbits.TMR2ON = 1; 			//开启时钟2

	while(1)
	{
		GXHT30_write_cmd(addr, 0x2c, 0x0d);		//向GXHT30发送控制命令（单次转换命令）	
		GXHT30_read_result(addr);				//向GXHT30发送读取数据命令
		Delay10us(1);
		p = ((int)Temperature / 10); //10		//取出温度值的十位数
		q = ((int)Temperature % 10); //1		//取出温度值的个位数
		
		if((ad10 <= (int)Temperature) && (ad10 != 0))	//判断当前温度值是否超过设定温度值， 在未设定温度值时无效
		{
			T1 = 1;								//开启报警灯
		}
		else if(ad10 - 2 >= (int)Temperature)			//判断当前温度值是否低于设定温度值
		{
			T1 = 0;								//关闭报警灯
		}

		ADCON0bits.GO=1; 					//开启AD转换，需要在读取完温度并显示后在开始AD转换， 否则温度报警灯异常
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
		
		if(w >= 3) 						//多次测量，稳定数据
		{
			ad10 = cost1[0] + cost1[1] + cost1[2];		//总和
			ad10 = ad10 / 3;			//平均
			k = ad10 /10;				//取AD值的十位数
			l = ad10 % 10;				//取AD值的个位数
			w = 0;						//重置检测次数
			
			PIR1bits.ADIF = 0;			//初始化AD转换中断标志
			PIE1bits.ADIE = 1;			//允许AD转换中断
		}
		else 							//单次测量
		{
			ad1 = 0;					//初始化AD1变量
			ad1 = ADRESH;				//将AD值的高8位置入变量
			ad1 = ad1 << 2;				//将置入的变量向左移动两位，以容纳低两位AD值
			ad1 = ad1 | (ADRESL >> 6);	//合并低两位的AD值
			ad1 = (ad1 / 10) * 0.781;	//将AD转换后的值进行缩放调整
			cost1[w] = (int)ad1;		//存放每一次读取到的设定温度值
			w++;						//数组计数标志加一
			
			PIR1bits.ADIF = 0;			//初始化AD转换中断标志
			PIE1bits.ADIE = 1;			//允许AD转换中断
		}
	}
}


#pragma interruptlow PIC18F_Low_isr		//低优先级中断
void PIC18F_Low_isr (void)
{
	if(PIR1bits.TMR2IF == 1)			//判断定时器2计数值与pr2是否匹配中断
    {
        PIE1bits.TMR2IE = 0;			//禁止时钟2匹配中断
        T2CONbits.TMR2ON = 0;			//停止计数
        PIR1bits.TMR2IF = 0;			//清除时钟2匹配中断标志
        count++;						//计数器加1， 
        
        if(count >= 15)					//200us*25=5ms(200HZ), 每十五次中断更换一次字符与数码管
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
    
    if(INTCONbits.TMR0IF == 1)			//检测时钟0是否溢出中断
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