//k18�������ļ�

#include <p18cxxx.h>
#include "k18.h"

//��ʼ��
void k18_init(void)
{
	ADCON1 = 0X0F; //��������˫�ÿ�λ��ͨ���ֿ�
	
	CMCON = 0X07; //�ر����бȽ���
	INTCON2bits.RBPU = 0; //����B��������
	
	//K18���ŷ��������ֵ����
	
	SDA = 0;
	T_SDA = OUT;

	SCL = 0;
	T_SCL = OUT;
	
	L1 = 0;
	T_L1 = OUT;
	
	L2 = 0;
	T_L2 = OUT;
	
	L3 = 0;
	T_L3 = OUT;
	
	L4 = 0;
	T_L4 = OUT;

	
}
