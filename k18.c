//k18�������ļ�

#include <p18cxxx.h>
#include "k18.h"

//��ʼ��
void k18_init(void)
{
	ADCON1 = 0X0F; //��������˫ӵ��λ��ͨ���ֿ�
	
	CMCON = 0X07; //�ر����бȽ���
	INTCON2bits.RBPU = 0; //����B��������
	
	//K18���ŷ��������ֵ����
	
	COL1 = 0;
	TRIS_COL1 = OUT;
	
	SDA = 0;
	TRIS_SDA = OUT;

	SCL = 0;
	TRIS_SCL = OUT;
	
	L1 = 0;
	TRIS_L1 = OUT;
	
	L2 = 0;
	TRIS_L2 = OUT;
	
	L3 = 0;
	TRIS_L3 = OUT;
	
	L4 = 0;
	TRIS_L4 = OUT;
	
	L5 = 0;
	TRIS_L5 = OUT;
	
	L6 = 0;
	TRIS_L6 = OUT;
	
	L7 = 0;
	TRIS_L7 = OUT;
	
}
