 //������

#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"
#include <math.h>

float Temperature;					//�ⲿ�������¶�
float Humidity;						//�ⲿ������ʪ��
const unsigned char TABLE[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F}; 	//0~9������ַ�
unsigned int cost1[3];				//ADת����������
int a = 1, j = 0, w = 0;			//a��PWM��������ݼ�ָʾ���� j��ADת������ָʾ���� w��ADת�������ȶ�ָʾ��
unsigned long ad10 = 0, ad11 = 0;		//ad1������ADת���Ĵ����� ad10��ADת����������Ĵ���
unsigned int ad1;
unsigned int p = 0, q = 0;			//p��ʮλ�������� q����λ������
unsigned int k = 0, l = 0;			//k��ʮλ�������� l����λ������
unsigned int count = 0;				//�����ˢ��ָʾ��
unsigned int chosebit = 0;			//�����ѡ��ָʾ��
int t, v, m;                		//t ��λ��ת���趨�¶�ֵ�� v NTCת��ʵ���¶�ֵ�� m �¶ȱ���ָʾ��
unsigned long VR;           			//VR ��ǰ��ѹֵ
unsigned long Rt;            		//Rt ��ǰ��̬����ֵ
unsigned int const TABLE1[] = {9712, 9166, 8654, 8172, 7722, 7298, 6900, 6526, 6176, 5534, 
							   5242, 4966, 4708, 4464, 4234, 4016, 3812, 3620, 3438, 3266, 
                               3104, 2950, 2806, 2668, 2540, 2418, 2302, 2192, 2088, 1990, 
                               1897, 1809, 1726, 1646, 1571, 1500, 1432, 1368, 1307, 1249, 
                               1194, 1142, 1092, 1045, 1000, 957, 916, 877, 840, 805, 772, 
                               740, 709, 680, 653, 626, 601, 577, 554, 532, 511, 491, 472, 
                               454, 436, 420, 404, 388, 374, 360, 346, 334, 321, 309, 298, 
                               287, 277, 267, 258, 248
                              };  	//-20 ~ 60 �¶ȶ�Ӧ����ֵ��Ϊ���Ż��ռ䣬����ֵ��Сһλ��

void main(void)
{
	int i, b = 0, d = 1;
	GXHT30_init(); 					//��ʼ��
	T0CONbits.TMR0ON = 1; 			//����ʱ��0
	T2CONbits.TMR2ON = 1; 			//����ʱ��2

	while(1)
	{
		//GXHT30_write_cmd(addr, 0x2c, 0x0d);		//��GXHT30���Ϳ����������ת�����
		//GXHT30_read_result(addr);					//��GXHT30���Ͷ�ȡ��������
		//Delay10us(1);
		
		if(b >= 20)
		{
			if(d == 1)
			{
				ADCON0 = 0x01;			//AN0ͨ�����ӵ�λ��
					d = -1;
			}
			else if(d == -1)
			{
				ADCON0 = 0x05;			//AN1ͨ������NTC
					d = 1;
			}
			DelayMs(5);
			ADCON0bits.GO = 1; 			//����ADת������Ҫ�ڶ�ȡ���¶Ȳ���ʾ���ڿ�ʼADת���� �����¶ȱ������쳣
			DelayMs(5);
			
			if(v <= t)                  //��vʵ���¶�С�ڵ���t�趨�¶�
			{
    			T1 = 1;                 //����������
			}
			else if(v > t + 2)        	//��vʵ���¶ȴ���t�趨�¶� - 2 �ȣ� �趨�ز�����¶��ٽ�����
			{
    			T1 = 0;                 //�رձ�����
			}
			b = 0;
		}
		else
		{
			Delay10Ms(1);
			b++;
		}
		
	}
}


void PIC18F_High_isr(void);				//�����ȼ��жϷ���������
void PIC18F_Low_isr(void);				//�����ȼ��жϷ���������

#pragma code high_vector_section=0x8	//�����ȼ��жϵ�ַ
void high_vector (void)
{
	_asm goto PIC18F_High_isr _endasm	//ͨ��һ����תָ��(���ָ�����ת���жϷ��������жϷ�����򣩴�
}

#pragma code low_vector_section=0x18	//�����ȼ��жϵ�ַ
void low_vector (void)
{
	_asm goto PIC18F_Low_isr _endasm	//ͨ��һ����תָ��(���ָ�����ת���жϷ��������жϷ�����򣩴�
}


#pragma code							//ָʾ�������ص�Ĭ�ϵĴ����,ȷ���������뱻��ȷ����


#pragma interrupt PIC18F_High_isr		//�����ȼ��ж�
void PIC18F_High_isr (void)	
{
	if(PIR1bits.ADIF == 1)				//����Ƿ�����ADת���ж�
	{
		PIE1bits.ADIE = 0;				//��ֹADת���ж�
		PIR1bits.ADIF = 0;				//���ADת���жϱ�־
		if(ADCON0 == 0x01)				//��λ��
		{
			ad1 = ADRESH;
			ad1 = (ad1 << 2) | (ADRESL >> 6);
			t = (unsigned int)(ad1 / 10 * 0.785);
			p = t / 10;
			q = t % 10;
			
		}
		else if (ADCON0 == 0x05)		//NTC
		{
			ad10 = ADRESH;
			ad10 = (ad10 << 2) | (ADRESL >> 6);
			ad11 = 1024 - ad10;
			VR = (unsigned long)(ad11 * 500 / 1024);		//ת���ɵ�ѹֵ
			Rt = (unsigned long)((500 - VR) * 1000 / VR);      //����NTC��ǰ�Ķ�̬����ֵ

			for(m = 0; m < 80; m++)     //��ת����Ķ�̬����ֵ��TABLE�����е�Ԫ�����αȽ�
			{
				if(Rt >= TABLE1[m])		//�¶�Խ�ͣ�����Խ�ߣ���ˣ������ݱ������ݵ���ֵ�Ӹߵ��ͽ��в���
				{
					v = m;              //����������ֹ����
					break;              //���ҵ��������ݺ�����ѭ��
				}
			}
			k = v / 10;
			l = v % 10;
/*
			k = (unsigned int)ad10 / 1000; //1000 
			l = (unsigned int)ad10 % 1000 / 100; //100
			p = (unsigned int)ad10 % 100 / 10; //10
			q = (unsigned int)ad10 % 10; //1
*/
		}
	}
	PIE1bits.ADIE = 1;				//����ADת���ж�
}


#pragma interruptlow PIC18F_Low_isr		//�����ȼ��ж�
void PIC18F_Low_isr (void)
{
	if(PIR1bits.TMR2IF == 1)			//�ж϶�ʱ��2����ֵ��pr2�Ƿ�ƥ���ж�		LED�����
    {
        PIE1bits.TMR2IE = 0;			//��ֹʱ��2ƥ���ж�
        T2CONbits.TMR2ON = 0;			//ֹͣ����
        PIR1bits.TMR2IF = 0;			//���ʱ��2ƥ���жϱ�־
        count++;						//��������1�� 
        
        if(count >= 10)					//200us*25=5ms(200HZ), ÿʮ����жϸ���һ���ַ��������
        {
            count = 0;					//���count����
            C1 = 1;						//��������ܣ����ݽӿ��빫���˶����ӵ���Ƭ����
            C2 = 1;						//���ԣ� һ�����룬 ��һ�˾�Ϊ���
            C3 = 1;						//����������У� ���ݶ����Ϊ1
            C4 = 1;						//��ˣ� �����˽�ͨ��Ϊ0�� Ϊ1��ر�
            switch(chosebit)			//��ѡ����
            {
            case 0:
                C1 = 0;					//ѡͨ��һλ����
                PORTB = TABLE[q];		//������
                chosebit = 1;
                break;
            case 1:
                C2 = 0;					//ѡͨ�ڶ�λ����
                PORTB = TABLE[p];		//������
                chosebit = 2;
                break;
            case 2:
                C3 = 0;					//ѡͨ����λ����
                PORTB = TABLE[l];		//������
                chosebit = 3;
                break;
            case 3:
                C4 = 0;					//ѡͨ����λ����
                PORTB = TABLE[k];		//������
                chosebit = 0;
                break;
            default:
                break;
            }
        }
        TMR2 = 0x00;					//ʱ��2����
        T2CONbits.TMR2ON = 1;			//����ʱ��2
        PIE1bits.TMR2IE = 1;			//����ʱ��2ƥ���ж�
    }

    if(INTCONbits.TMR0IF == 1)			//���ʱ��0�Ƿ�����ж�						PWM����
	{
		INTCONbits.TMR0IF = 0; 			//ʱ��0�����־����
		INTCONbits.TMR0IE = 0;			//����ʱ��0����ж�
		
		if(CCPR1L >= PR2)				//��������Ƿ񳬹��������ֵ
		{
			a = -1;						//�ǣ� ��ʼ�ݼ�
		}
		else if(CCPR1L <= 0x00)			//��������Ƿ����������Сֵ
		{
			a = 1;						//�ǣ� ��ʼ�����ݼ�
		}
		
		if(a == 1)
		{
			CCPR1L = CCPR1L + 5;		//ʹ�������
		}
		else if(a == -1)
		{
			CCPR1L = CCPR1L - 5;		//ʹ����ݼ�
		}
		
		INTCONbits.TMR0IF = 0; 			//Timer0�����־����
		INTCONbits.TMR0IE = 1;			//����Timer0����ж�
	}
}