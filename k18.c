//k18�������ļ�

#include <p18cxxx.h>
#include "k18.h"
#include "GXHT30.h"
//��ʼ��
void GXHT30_init(void)
{
	//ADCON1 = 0X0F; //��������˫�ÿ�λ��ͨ���ֿ�
	
	//CMCON = 0X07; //�ر����бȽ���
	//INTCON2bits.RBPU = 1; //����B��������

	SDA = 0;
	T_SDA = OUT;
	SCL = 0;
	T_SCL = OUT;
	
	
	T_AD = 1;
	T_T1 = 0;
	T_L1 = 0;
	T_A = 0;
	T_B = 0;
	T_C = 0;
	T_D = 0;
	T_E = 0;
	T_F = 0;
	T_G = 0;
	T_C1 = 0;
	T_C2 = 0;
	T_C3 = 0;
	T_C4 = 0;
	
	
	AD = 0;
	T1 = 0;
	L1 = 0;
	A = 0;
	B = 0;
	C = 0;
	D = 0;
	E = 0;
	F = 0;
	G = 0;
	C1 = 0;
	C2 = 0;
	C3 = 0;
	C4 = 0;
}
