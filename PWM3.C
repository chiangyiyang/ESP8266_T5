
#include "reg51.h"
#include "intrins.h"

#define FOSC    11059200L

typedef unsigned char BYTE;
typedef unsigned int WORD;

sfr P_SW1       = 0xA2;             //���蹦���л��Ĵ���1

#define CCP_S0 0x00                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

sfr CCON        =   0xD8;           //PCA���ƼĴ���
sbit CCF0       =   CCON^0;         //PCAģ��0�жϱ�־
sbit CCF1       =   CCON^1;         //PCAģ��1�жϱ�־
sbit CR         =   CCON^6;         //PCA��ʱ�����п���λ
sbit CF         =   CCON^7;         //PCA��ʱ�������־
sfr CMOD        =   0xD9;           //PCAģʽ�Ĵ���
sfr CL          =   0xE9;           //PCA��ʱ�����ֽ�
sfr CH          =   0xF9;           //PCA��ʱ�����ֽ�
sfr CCAPM0      =   0xDA;           //PCAģ��0ģʽ�Ĵ���
sfr CCAP0L      =   0xEA;           //PCAģ��0����Ĵ��� LOW
sfr CCAP0H      =   0xFA;           //PCAģ��0����Ĵ��� HIGH
sfr CCAPM1      =   0xDB;           //PCAģ��1ģʽ�Ĵ���
sfr CCAP1L      =   0xEB;           //PCAģ��1����Ĵ��� LOW
sfr CCAP1H      =   0xFB;           //PCAģ��1����Ĵ��� HIGH
sfr CCAPM2      =   0xDC;           //PCAģ��2ģʽ�Ĵ���
sfr CCAP2L      =   0xEC;           //PCAģ��2����Ĵ��� LOW
sfr CCAP2H      =   0xFC;           //PCAģ��2����Ĵ��� HIGH
sfr PCA_PWM0    =   0xf2;           //PCAģ��0��PWM�Ĵ���
sfr PCA_PWM1    =   0xf3;           //PCAģ��1��PWM�Ĵ���
sfr PCA_PWM2    =   0xf4;           //PCAģ��2��PWM�Ĵ���

void PWM(unsigned char r,unsigned char g,unsigned char b)
{ 
    P_SW1|=0x20;
    P_SW1&=0xef;
    CCON = 0;                       //��ʼ��PCA���ƼĴ���
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CMOD = 0x02;                    //����PCAʱ��Դ
                                    //��ֹPCA��ʱ������ж�
    PCA_PWM0 = 0x00;                //PCAģ��0������8λPWM
    CCAP0H = CCAP0L = r;         //PWM0��ռ�ձ�Ϊ87.5% ((100H-20H)/100H)
    CCAPM0 = 0x42;                  //PCAģ��0Ϊ8λPWMģʽ

    PCA_PWM1 = 0x00;                //PCAģ��1������7λPWM
    CCAP1H = CCAP1L = g;         //PWM1��ռ�ձ�Ϊ75% ((80H-20H)/80H)
    CCAPM1 = 0x42;                  //PCAģ��1Ϊ7λPWMģʽ

    PCA_PWM2 = 0x00;                //PCAģ��2������6λPWM
    CCAP2H = CCAP2L = b;         //PWM2��ռ�ձ�Ϊ50% ((40H-20H)/40H)
    CCAPM2 = 0x42;                  //PCAģ��2Ϊ6λPWMģʽ

    CR = 1;                         //PCA��ʱ����ʼ����
}
void set_pwm(unsigned char data rr,unsigned char data gg,unsigned char data bb)
{
    CCAP0H = CCAP0L = rr;         //PWM0��ռ�ձ�Ϊ87.5% ((100H-20H)/100H)

              //PCAģ��1������7λPWM
    CCAP1H = CCAP1L = gg;         //PWM1��ռ�ձ�Ϊ75% ((80H-20H)/80H)
              //PCAģ��1Ϊ7λPWMģʽ

    CCAP2H = CCAP2L = bb;         //PWM2��ռ�ձ�Ϊ50% ((40H-20H)/40H)
                 //PCAģ��2Ϊ6λPWMģʽ
}
