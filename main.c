 //������

#include <p18cxxx.h>
#include <delays.h>
#include "k18.h"
#include "delay.h"
#include "GXHT30.h"

float Temperature;					//�ⲿ�������¶�
float Humidity;						//�ⲿ������ʪ��
const unsigned char TABLE[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F}; 	//0~9������ַ�
unsigned int cost1[3];				//ADת����������
int a = 1, j = 0, w = 0;			//a��PWM��������ݼ�ָʾ���� j��ADת������ָʾ���� w��ADת�������ȶ�ָʾ��
int ad1 = 0, ad10 = 0;				//ad1������ADת���Ĵ����� ad10��ADת����������Ĵ���
unsigned char p = 0, q = 0;			//p��ʮλ�������� q����λ������
unsigned char k = 0, l = 0;			//k��ʮλ�������� l����λ������
unsigned int addr;					//GXHT30��I2C�����ϵĵ�ַ
unsigned int count = 0;				//�����ˢ��ָʾ��
unsigned char chosebit = 0;			//�����ѡ��ָʾ��

void main(void)
{
	GXHT30_init(); 					//��ʼ��
	addr = 0x88; 					//������Ĭ�ϵ�ַ

	T0CONbits.TMR0ON = 1; 			//����ʱ��0
	T2CONbits.TMR2ON = 1; 			//����ʱ��2

	while(1)
	{
		GXHT30_write_cmd(addr, 0x2c, 0x0d);		//��GXHT30���Ϳ����������ת�����	
		GXHT30_read_result(addr);				//��GXHT30���Ͷ�ȡ��������
		Delay10us(1);
		p = ((int)Temperature / 10); //10		//ȡ���¶�ֵ��ʮλ��
		q = ((int)Temperature % 10); //1		//ȡ���¶�ֵ�ĸ�λ��
		
		if((ad10 <= (int)Temperature) && (ad10 != 0))	//�жϵ�ǰ�¶�ֵ�Ƿ񳬹��趨�¶�ֵ�� ��δ�趨�¶�ֵʱ��Ч
		{
			T1 = 1;								//����������
		}
		else if(ad10 - 2 >= (int)Temperature)			//�жϵ�ǰ�¶�ֵ�Ƿ�����趨�¶�ֵ
		{
			T1 = 0;								//�رձ�����
		}

		ADCON0bits.GO=1; 					//����ADת������Ҫ�ڶ�ȡ���¶Ȳ���ʾ���ڿ�ʼADת���� �����¶ȱ������쳣
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
		
		if(w >= 3) 						//��β������ȶ�����
		{
			ad10 = cost1[0] + cost1[1] + cost1[2];		//�ܺ�
			ad10 = ad10 / 3;			//ƽ��
			k = ad10 /10;				//ȡADֵ��ʮλ��
			l = ad10 % 10;				//ȡADֵ�ĸ�λ��
			w = 0;						//���ü�����
			
			PIR1bits.ADIF = 0;			//��ʼ��ADת���жϱ�־
			PIE1bits.ADIE = 1;			//����ADת���ж�
		}
		else 							//���β���
		{
			ad1 = 0;					//��ʼ��AD1����
			ad1 = ADRESH;				//��ADֵ�ĸ�8λ�������
			ad1 = ad1 << 2;				//������ı��������ƶ���λ�������ɵ���λADֵ
			ad1 = ad1 | (ADRESL >> 6);	//�ϲ�����λ��ADֵ
			ad1 = (ad1 / 10) * 0.781;	//��ADת�����ֵ�������ŵ���
			cost1[w] = (int)ad1;		//���ÿһ�ζ�ȡ�����趨�¶�ֵ
			w++;						//���������־��һ
			
			PIR1bits.ADIF = 0;			//��ʼ��ADת���жϱ�־
			PIE1bits.ADIE = 1;			//����ADת���ж�
		}
	}
}


#pragma interruptlow PIC18F_Low_isr		//�����ȼ��ж�
void PIC18F_Low_isr (void)
{
	if(PIR1bits.TMR2IF == 1)			//�ж϶�ʱ��2����ֵ��pr2�Ƿ�ƥ���ж�
    {
        PIE1bits.TMR2IE = 0;			//��ֹʱ��2ƥ���ж�
        T2CONbits.TMR2ON = 0;			//ֹͣ����
        PIR1bits.TMR2IF = 0;			//���ʱ��2ƥ���жϱ�־
        count++;						//��������1�� 
        
        if(count >= 15)					//200us*25=5ms(200HZ), ÿʮ����жϸ���һ���ַ��������
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
    
    if(INTCONbits.TMR0IF == 1)			//���ʱ��0�Ƿ�����ж�
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