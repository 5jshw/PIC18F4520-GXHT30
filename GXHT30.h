#ifndef __gxht30_h
#define __gxht30_h

#define SCL_O	TRIS_SCL = 0; //时钟端口输出
#define SCL_I	TRIS_SCL = 1; //时钟端口输入

#define SCL_H	SCL = 1; //上拉时钟
#define SCL_L	SCL = 0; //下拉时钟

#define SDA_O	TRIS_SDA = 0; //数据端口输出
#define SDA_I	TRIS_SDA = 1; //数据端口输入

#define SDA_H	SDA = 1; //上拉数据
#define SDA_L	SDA = 0; //下拉数据

void IIC_init(void);  //初始化
void IIC_Start(void); //起始信号
void IIC_Stop(void); //结束信号
void IIC_Ack(void); //应答信号
void IIC_NAck(void); //不应答信号
unsigned char IIC_WAck(void); //等待应答
void IIC_SendByte(unsigned int txd); //发送字节
unsigned char IIC_ReadByte(unsigned int ack); //接收字节
void GXHT30_read_result(unsigned int addr); //读取温度
void GXHT30_write_cmd(unsigned int addr, unsigned int MSB, unsigned int LSB);


#endif