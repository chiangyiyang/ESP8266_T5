
#include "reg51.h"
#include "intrins.h"

#define FOSC    11059200L

typedef unsigned char BYTE;
typedef unsigned int WORD;

sfr P_SW1       = 0xA2;             //外设功能切换寄存器1

#define CCP_S0 0x00                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

sfr CCON        =   0xD8;           //PCA控制寄存器
sbit CCF0       =   CCON^0;         //PCA模块0中断标志
sbit CCF1       =   CCON^1;         //PCA模块1中断标志
sbit CR         =   CCON^6;         //PCA定时器运行控制位
sbit CF         =   CCON^7;         //PCA定时器溢出标志
sfr CMOD        =   0xD9;           //PCA模式寄存器
sfr CL          =   0xE9;           //PCA定时器低字节
sfr CH          =   0xF9;           //PCA定时器高字节
sfr CCAPM0      =   0xDA;           //PCA模块0模式寄存器
sfr CCAP0L      =   0xEA;           //PCA模块0捕获寄存器 LOW
sfr CCAP0H      =   0xFA;           //PCA模块0捕获寄存器 HIGH
sfr CCAPM1      =   0xDB;           //PCA模块1模式寄存器
sfr CCAP1L      =   0xEB;           //PCA模块1捕获寄存器 LOW
sfr CCAP1H      =   0xFB;           //PCA模块1捕获寄存器 HIGH
sfr CCAPM2      =   0xDC;           //PCA模块2模式寄存器
sfr CCAP2L      =   0xEC;           //PCA模块2捕获寄存器 LOW
sfr CCAP2H      =   0xFC;           //PCA模块2捕获寄存器 HIGH
sfr PCA_PWM0    =   0xf2;           //PCA模块0的PWM寄存器
sfr PCA_PWM1    =   0xf3;           //PCA模块1的PWM寄存器
sfr PCA_PWM2    =   0xf4;           //PCA模块2的PWM寄存器

void PWM(unsigned char r,unsigned char g,unsigned char b)
{ 
    P_SW1|=0x20;
    P_SW1&=0xef;
    CCON = 0;                       //初始化PCA控制寄存器
    CL = 0;                         //复位PCA寄存器
    CH = 0;
    CMOD = 0x02;                    //设置PCA时钟源
                                    //禁止PCA定时器溢出中断
    PCA_PWM0 = 0x00;                //PCA模块0工作于8位PWM
    CCAP0H = CCAP0L = r;         //PWM0的占空比为87.5% ((100H-20H)/100H)
    CCAPM0 = 0x42;                  //PCA模块0为8位PWM模式

    PCA_PWM1 = 0x00;                //PCA模块1工作于7位PWM
    CCAP1H = CCAP1L = g;         //PWM1的占空比为75% ((80H-20H)/80H)
    CCAPM1 = 0x42;                  //PCA模块1为7位PWM模式

    PCA_PWM2 = 0x00;                //PCA模块2工作于6位PWM
    CCAP2H = CCAP2L = b;         //PWM2的占空比为50% ((40H-20H)/40H)
    CCAPM2 = 0x42;                  //PCA模块2为6位PWM模式

    CR = 1;                         //PCA定时器开始工作
}
void set_pwm(unsigned char data rr,unsigned char data gg,unsigned char data bb)
{
    CCAP0H = CCAP0L = rr;         //PWM0的占空比为87.5% ((100H-20H)/100H)

              //PCA模块1工作于7位PWM
    CCAP1H = CCAP1L = gg;         //PWM1的占空比为75% ((80H-20H)/80H)
              //PCA模块1为7位PWM模式

    CCAP2H = CCAP2L = bb;         //PWM2的占空比为50% ((40H-20H)/40H)
                 //PCA模块2为6位PWM模式
}

