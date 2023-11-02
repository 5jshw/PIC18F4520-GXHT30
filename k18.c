//k18主板主文件

#include <p18cxxx.h>
#include "k18.h"
#include "GXHT30.h"
//初始化
void GXHT30_init(void)
{
	//ADCON1 = 0X0F; //设置所有双用口位普通数字口
	
	//CMCON = 0X07; //关闭所有比较器
	//INTCON2bits.RBPU = 1; //开启B口弱上拉

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
