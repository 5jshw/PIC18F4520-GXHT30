#ifndef __gxht30_h
#define __gxht30_h

#define SCL_O	TRIS_SCL = 0; //ʱ�Ӷ˿����
#define SCL_I	TRIS_SCL = 1; //ʱ�Ӷ˿�����

#define SCL_H	SCL = 1; //����ʱ��
#define SCL_L	SCL = 0; //����ʱ��

#define SDA_O	TRIS_SDA = 0; //���ݶ˿����
#define SDA_I	TRIS_SDA = 1; //���ݶ˿�����

#define SDA_H	SDA = 1; //��������
#define SDA_L	SDA = 0; //��������

void IIC_init(void);  //��ʼ��
void IIC_Start(void); //��ʼ�ź�
void IIC_Stop(void); //�����ź�
void IIC_Ack(void); //Ӧ���ź�
void IIC_NAck(void); //��Ӧ���ź�
unsigned char IIC_WAck(void); //�ȴ�Ӧ��
void IIC_SendByte(unsigned int txd); //�����ֽ�
unsigned char IIC_ReadByte(unsigned int ack); //�����ֽ�
void GXHT30_read_result(unsigned int addr); //��ȡ�¶�
void GXHT30_write_cmd(unsigned int addr, unsigned int MSB, unsigned int LSB);


#endif