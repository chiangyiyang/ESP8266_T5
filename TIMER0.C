
#include"STC15FXXXX.H"

typedef unsigned char BYTE;
typedef unsigned int WORD;
extern unsigned char uart_time_out;
extern unsigned char uart_time_out;
extern bit uart_rec_date;
extern bit uart_rec_new;
//-----------------------------------------------

#define FOSC 22118400L

#define T1MS (65536-FOSC/1000)      //1Tģʽ
//#define T1MS (65536-FOSC/12/1000) //12Tģʽ

//sfr AUXR = 0x8e;                    //Auxiliary register
sbit pwm = P1^5;
bit p_s=0;
//-----------------------------------------------

/* Timer0 interrupt routine */
extern unsigned char uart_time_out;
extern bit uart_rec_date;
extern bit uart_rec_new;
unsigned int delay_1min=0;
bit min_rech=0;
extern unsigned char old_bar_r;
extern unsigned char old_bar_g;
extern unsigned char old_bar_b;
extern unsigned char bar_r;
extern unsigned char bar_g;
extern unsigned char bar_b;
unsigned char ms_10=0;
extern void PWM(unsigned char r,unsigned char g,unsigned char b);
extern void set_pwm(unsigned char r,unsigned char g,unsigned char b);
bit have_change_pwm=0;
void tm0_isr() interrupt 1 using 1
{
	if(ms_10<10)
	ms_10++;
	else 
	{
	 ms_10=0;
	 have_change_pwm=1;
	}

	if(delay_1min<60000)
	delay_1min++;
	else
	{
	  min_rech=1;
	  delay_1min=0;
	}

	if(uart_time_out<10)
	uart_time_out++;
	else
	{
	 if((uart_rec_date)&&(uart_rec_new==0))
	 {
	 uart_rec_date=0;
	 uart_rec_new=1;
	 }
	}
}

//-----------------------------------------------

/* main program */
void timer0_ini()
{
    AUXR |= 0x80;                   //��ʱ��0Ϊ1Tģʽ
//  AUXR &= 0x7f;                   //��ʱ��0Ϊ12Tģʽ
	PT0=0;
    TMOD = 0x00;                    //���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
    TL0 = T1MS;                     //��ʼ����ʱֵ
    TH0 = T1MS >> 8;
    TR0 = 1;                        //��ʱ��0��ʼ��ʱ
    ET0 = 1;                        //ʹ�ܶ�ʱ��0�ж�
    EA = 1;

}