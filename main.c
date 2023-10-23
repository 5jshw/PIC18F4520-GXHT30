//主程序

#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"

void K(void);
void G(void);

float Temperature;
float Humidity;
	
unsigned int addr;
	
void main(void)
{
	OSCCON = 0b01110011; //8MHz振荡器频率，内部振荡器电路
	addr = 0x88; //传感器默认地址
	

	k18_init(); //初始化
	IIC_init();
	//GXHT30_write_cmd(addr, 0x30, 0xa2); //软复位
	//GXHT30_write_cmd(addr, 0x30, 0x93); //中断周期测量
	//GXHT30_write_cmd(addr, 0x30, 0x6d); //加热器
	//GXHT30_write_cmd(addr, 0x2c, 0x0d); //单次转换
	COL1 = 1;
	K();
	Delay10Ms(20);

	
	while(1)
	{
		COL1 = 0;
		G();
		Delay10Ms(1);
		
		GXHT30_write_cmd(addr, 0x2c, 0x0d);

		GXHT30_read_result(addr);
		
		if(Temperature <= 27){
			
			L1 = 1;
			Delay10Ms(1);
			
		}else if(Temperature > 27 && Temperature <= 28){
		
			L2 = 1;
			Delay10Ms(1);
			
		}else if(Temperature >28 && Temperature <= 29){
		
			L3 = 1;
			Delay10Ms(1);
			
		}else if(Temperature > 29 && Temperature <= 32){
		
			L4 = 1;
			Delay10Ms(1);
			
		}else{
			L1 = 1;
			L2 = 1;
			L3 = 1;
			L4 = 1;
			Delay10Ms(1);
		}
	}

}

void K(void)
{
	L1 = 1;
	L2 = 1;
	L3 = 1;
	L4 = 1;
	L5 = 1;
	L6 = 1;
	L7 = 1;
}

void G(void)
{
	L1 = 0;
	L2 = 0;
	L3 = 0;
	L4 = 0;
	L5 = 0;
	L6 = 0;
	L7 = 0;
}