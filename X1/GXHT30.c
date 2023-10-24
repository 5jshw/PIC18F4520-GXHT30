#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"

extern float Temperature; //�¶����ݣ�����
extern float Humidity; //ʪ�����ݣ�����

//I2C��ʼ�ź�
void IIC_Start(void)
{
	SDA_O;			//���ݶ˿����
	SCL_L;
		
	SDA_H;			//�������ݣ���ʼ�źţ�
	Delayus(2);
	SCL_H;			//����ʱ��
	Delayus(2);     //						SCL __/---\__
	SDA_L;			//��������				SDA ----\____ 
	Delayus(2);		//
	SCL_L; 		   	//����ʱ�ӣ���λ��
	XS1();
}

//I2C��ֹ�ź�
void IIC_Stop(void)
{
	SDA_O;
	SCL_L;			//��ʼ��
	
	SDA_L;			//�������ݣ������źţ�
	Delayus(2);		//
	SCL_H;			//����ʱ��
	Delayus(2);		//						SCL __/---\__
	SDA_H;			//��������				SDA ____/----
	Delayus(2);		//
	SCL_L;			//����ʱ��
	XS1();
}

//I2CӦ���ź�
void IIC_Ack(void)
{
	SDA_O;
	SCL_L;			//��ʼ��
	
	SDA_L;			//��������
	Delayus(2);		//						SCL __/---\__
	SCL_H;			//����ʱ��				SDA _________  
	Delayus(2);		//
	SCL_L;			//����ʱ��
	XS1();
}

//I2C��Ӧ���ź�
void IIC_NAck(void)
{
	SDA_O;
	SCL_L;			//��ʼ��
	
	SDA_H;			//��������
	Delayus(2);		//						SCL __/---\__
	SCL_H;			//����ʱ��				SDA ---------
	Delayus(2);		//
	SCL_L;			//����ʱ��
	XS1();
}

//I2C�ȴ�Ӧ��
unsigned char IIC_WAck(void)
{
	unsigned long i; //������
	SDA_I; //���ݽӿ�����
	SDA_H; //��������
	SCL_H; //����ʱ��
	
	while(SDA) //�жϴ�����Ӧ���ź�
	{	
		XS1();
		i++;
		if(i > 255) //����ʱ�� �ж��Ƿ���յ�Ӧ���źţ�����SCL�ߵ�ƽʱSDA����
		{
			IIC_Stop(); //��������涨ʱ��û�ж��������������ͽ����ź�
			return 1; //������ֵ1���յ���Ӧ���ź�
		}
	}
	
	SCL_L; //����ʱ��
	
	return 0; //�յ�Ӧ���ź�
}

//�����ֽ�
void IIC_SendByte(unsigned int txd)
{
	unsigned int i = 0;
 
	SDA_O; //���ݽӿ����
	SCL_L; //����ʱ��
 
	for(i = 0; i < 8; i++) //����һ�ֽ�8λ����
	{
		if((txd & 0x80) > 0) //�жϸñ������λ��ֵ
		{
			SDA_H; //Ϊ1ʱ����������
		}	
		else
		{
			SDA_L; //Ϊ0ʱ����������
 		}
 		XS1();
		txd <<= 1; //������ɺ󽫴�������������һλ��׼��������һλ����
		SCL_H; //����ʱ�ӣ���������
		Delayus(2);
		SCL_L; //����ʱ�ӣ��������
		Delayus(2); //�ӳ�Ϊ�������ȶ�
		XS1();
	}
}

//�����ֽ�
unsigned char IIC_ReadByte(unsigned int ack) //ack �ж��ֽ������Ƿ�������
{
	
   unsigned int i = 0,receive = 0; //i��Ϊ�˽���ͳ�ƣ�receive��Ϊ�˽��շ���������
 
   SDA_I; //���ݽӿ�����
   
   for(i = 0; i < 8; i++)
   {
   		SCL_L; //����ʱ�ӣ���λ
		Delayus(2);
		SCL_H; //����ʱ��
		
		while(!SCL);
		receive <<= 1;	
		if(SDA == 1)
		{
			receive++;
		}
		XS1();
   }
   
   	if(ack == 0) //�����ж��ڵ��øú���ʱ��ack��ֵ��Ҳ���Ƿ���������ݣ��Ƿ��ͽ����źš�
	{
	   	IIC_NAck(); //���һ�ֽ����ݽ�����ϣ���Ӧ��
	}
	else
	{
		IIC_Ack(); //һλ�ֽ����ݽ�����ϣ��ظ�Ӧ��
 	}	
	return receive; //���ؽ��յ���һ�ֽ�����
}

//��ȡ�¶�
void GXHT30_read_result(unsigned int addr)
{
	unsigned int tem,hum; //�ϲ������ã�����
	unsigned int buff[6]; //���������ã�����
	Temperature = 0; //�¶����ݣ�����
	Humidity = 0; //ʪ�����ݣ�����

	IIC_Start(); //��ʼ�ź�
	IIC_SendByte(addr | 0x01); // 1 ����ַ���һλ��ʾ��д������1Ϊ������
	XS1();
	Delay10us(1);
	XS1();
	if(IIC_WAck() == 0)
	{
		SDA_I; //���ݽӿ�����
	
		buff[0] = IIC_ReadByte(1); //�¶�1
	
		buff[1] = IIC_ReadByte(1); //�¶�2
	
		buff[2] = IIC_ReadByte(1); //CRCУ����
	
		buff[3] = IIC_ReadByte(1); //ʪ��1
	
		buff[4] = IIC_ReadByte(1); //ʪ��2
	
		buff[5] = IIC_ReadByte(0); //CRCУ���룬���һ���ֽ�
	
		IIC_Stop(); //���ͽ����ź�
		
	}
	
	tem = ((buff[0] << 8) | buff[1]);// �ϲ��¶�����
	hum = ((buff[3] << 8) | buff[4]);// �ϲ�ʪ������
	XS1();
	Temperature = (175.0 * (float)tem / 65535.0 - 45.0); // T = -45 + 175 * tem / (2^16-1)
	Humidity = (100.0 * (float)hum /65535.0); // RH = hum*100 / (2^16-1)
   
  	hum = 0; //��������
	tem = 0; //��������
	XS1();
	Delay10us(2);
}

void GXHT30_write_cmd(unsigned int addr, unsigned int MSB, unsigned int LSB)
{
	XS1();
	IIC_Start(); //��ʼ�ź�
	IIC_SendByte(addr); // 0 ����ַ���һλ��ʾ��д������0Ϊд����
	IIC_WAck(); //�ȴ�Ӧ��
	IIC_SendByte(MSB); //����clock stretching����ʱ������
	IIC_WAck();
	IIC_SendByte(LSB); //���õ��ظ�������ת��
	IIC_WAck();
	IIC_Stop(); //���ͽ����ź�
	XS1();
	Delay10us(5); //�ӳ�ȷ�������ȶ�
	XS1();
}