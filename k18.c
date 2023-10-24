//k18主板主文件

#include <p18cxxx.h>
#include "k18.h"

//初始化
void k18_init(void)
{
	ADCON1 = 0X0F; //设置所有双用口位普通数字口
	
	CMCON = 0X07; //关闭所有比较器
	INTCON2bits.RBPU = 0; //开启B口弱上拉
	
	//K18引脚方向、输出初值定义
	
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
