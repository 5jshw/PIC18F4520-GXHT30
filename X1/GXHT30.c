#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"

extern float Temperature; //温度数据，变量
extern float Humidity; //湿度数据，变量

//I2C起始信号
void IIC_Start(void)
{
	SDA_O;			//数据端口输出
	SCL_L;
		
	SDA_H;			//上拉数据（起始信号）
	Delayus(2);
	SCL_H;			//上拉时钟
	Delayus(2);     //						SCL __/---\__
	SDA_L;			//下拉数据				SDA ----\____ 
	Delayus(2);		//
	SCL_L; 		   	//下拉时钟（复位）
	XS1();
}

//I2C终止信号
void IIC_Stop(void)
{
	SDA_O;
	SCL_L;			//初始化
	
	SDA_L;			//下拉数据（结束信号）
	Delayus(2);		//
	SCL_H;			//上拉时钟
	Delayus(2);		//						SCL __/---\__
	SDA_H;			//上拉数据				SDA ____/----
	Delayus(2);		//
	SCL_L;			//下拉时钟
	XS1();
}

//I2C应答信号
void IIC_Ack(void)
{
	SDA_O;
	SCL_L;			//初始化
	
	SDA_L;			//下拉数据
	Delayus(2);		//						SCL __/---\__
	SCL_H;			//上拉时钟				SDA _________  
	Delayus(2);		//
	SCL_L;			//下拉时钟
	XS1();
}

//I2C不应答信号
void IIC_NAck(void)
{
	SDA_O;
	SCL_L;			//初始化
	
	SDA_H;			//上拉数据
	Delayus(2);		//						SCL __/---\__
	SCL_H;			//上拉时钟				SDA ---------
	Delayus(2);		//
	SCL_L;			//下拉时钟
	XS1();
}

//I2C等待应答
unsigned char IIC_WAck(void)
{
	unsigned long i; //长整型
	SDA_I; //数据接口输入
	SDA_H; //上拉数据
	SCL_H; //上拉时钟
	
	while(SDA) //判断传感器应答信号
	{	
		XS1();
		i++;
		if(i > 255) //简易时钟 判断是否接收到应答信号，即在SCL高电平时SDA下拉
		{
			IIC_Stop(); //如果超过规定时间没有动作，则主机发送结束信号
			return 1; //返回数值1，收到不应答信号
		}
	}
	
	SCL_L; //下拉时钟
	
	return 0; //收到应答信号
}

//发送字节
void IIC_SendByte(unsigned int txd)
{
	unsigned int i = 0;
 
	SDA_O; //数据接口输出
	SCL_L; //下拉时钟
 
	for(i = 0; i < 8; i++) //发送一字节8位数据
	{
		if((txd & 0x80) > 0) //判断该变量最高位的值
		{
			SDA_H; //为1时，上拉数据
		}	
		else
		{
			SDA_L; //为0时，下拉数据
 		}
 		XS1();
		txd <<= 1; //发送完成后将待发送数据左移一位，准备发送下一位数据
		SCL_H; //上拉时钟，发送数据
		Delayus(2);
		SCL_L; //下拉时钟，发送完成
		Delayus(2); //延迟为了数据稳定
		XS1();
	}
}

//接收字节
unsigned char IIC_ReadByte(unsigned int ack) //ack 判断字节数据是否接收完成
{
	
   unsigned int i = 0,receive = 0; //i是为了接收统计，receive是为了接收发来的数据
 
   SDA_I; //数据接口输入
   
   for(i = 0; i < 8; i++)
   {
   		SCL_L; //下拉时钟，复位
		Delayus(2);
		SCL_H; //上拉时钟
		
		while(!SCL);
		receive <<= 1;	
		if(SDA == 1)
		{
			receive++;
		}
		XS1();
   }
   
   	if(ack == 0) //这是判断在调用该函数时的ack的值，也即是否接受完数据，是否发送结束信号。
	{
	   	IIC_NAck(); //最后一字节数据接收完毕，不应答
	}
	else
	{
		IIC_Ack(); //一位字节数据接收完毕，回复应答
 	}	
	return receive; //返回接收到的一字节数据
}

//读取温度
void GXHT30_read_result(unsigned int addr)
{
	unsigned int tem,hum; //合并数据用，变量
	unsigned int buff[6]; //接收数据用，数组
	Temperature = 0; //温度数据，变量
	Humidity = 0; //湿度数据，变量

	IIC_Start(); //起始信号
	IIC_SendByte(addr | 0x01); // 1 ，地址最后一位表示读写操作，1为读操作
	XS1();
	Delay10us(1);
	XS1();
	if(IIC_WAck() == 0)
	{
		SDA_I; //数据接口输入
	
		buff[0] = IIC_ReadByte(1); //温度1
	
		buff[1] = IIC_ReadByte(1); //温度2
	
		buff[2] = IIC_ReadByte(1); //CRC校验码
	
		buff[3] = IIC_ReadByte(1); //湿度1
	
		buff[4] = IIC_ReadByte(1); //湿度2
	
		buff[5] = IIC_ReadByte(0); //CRC校验码，最后一个字节
	
		IIC_Stop(); //发送结束信号
		
	}
	
	tem = ((buff[0] << 8) | buff[1]);// 合并温度数据
	hum = ((buff[3] << 8) | buff[4]);// 合并湿度数据
	XS1();
	Temperature = (175.0 * (float)tem / 65535.0 - 45.0); // T = -45 + 175 * tem / (2^16-1)
	Humidity = (100.0 * (float)hum /65535.0); // RH = hum*100 / (2^16-1)
   
  	hum = 0; //变量重置
	tem = 0; //变量重置
	XS1();
	Delay10us(2);
}

void GXHT30_write_cmd(unsigned int addr, unsigned int MSB, unsigned int LSB)
{
	XS1();
	IIC_Start(); //起始信号
	IIC_SendByte(addr); // 0 ，地址最后一位表示读写操作，0为写操作
	IIC_WAck(); //等待应答
	IIC_SendByte(MSB); //开启clock stretching（）时钟延伸
	IIC_WAck();
	IIC_SendByte(LSB); //设置低重复率数据转换
	IIC_WAck();
	IIC_Stop(); //发送结束信号
	XS1();
	Delay10us(5); //延迟确保数据稳定
	XS1();
}