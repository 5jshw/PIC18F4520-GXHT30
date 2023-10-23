//k18主板主文件

#include <p18cxxx.h>
#include "k18.h"

//初始化
void k18_init(void)
{
	ADCON1 = 0X0F; //设置所有双拥口位普通数字口
	
	CMCON = 0X07; //关闭所有比较器
	INTCON2bits.RBPU = 0; //开启B口弱上拉
	
	//K18引脚方向、输出初值定义
	
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
