#include"STC15FXXXX.H"
#include"delay.h"
sbit DHT = P3^5;
unsigned char TP_H=0;
unsigned char TP_L=0;
unsigned char RH_H=0;
unsigned char RH_L=0;
unsigned char CR_D=0;
bit ch_1_0(void)
{
  unsigned char t=0;
  delay_5us(6);
  while(DHT==0);
  for(t=0;DHT;t++)
  delay_5us(1); 
  if(t>10)
  return 1;
  else
  return 0;
}
sbit BEEP=P3^4;//·äÃùÆ÷
void read_dh(void)	  //DHT11 ¶ÁÈ¡³ÌĞò
{
  unsigned char i;
  DHT=0;
  Delay2(20);
  DHT=1;
  delay_5us(10);
  while(DHT==0);
  delay_5us(6);
  while(DHT);
  for(i=0;i<8;i++)
  {
  if(ch_1_0())
  TP_H=((TP_H<<1)+1);
  else
  TP_H<<=1;
  }
  for(i=0;i<8;i++)
  {
  if(ch_1_0())
  TP_L=((TP_L<<1)+1);
  else
  TP_L<<=1;
  }
    for(i=0;i<8;i++)
  {
  if(ch_1_0())
  RH_H=((RH_H<<1)+1);
  else
  RH_H<<=1;
  }
    for(i=0;i<8;i++)
  {
  if(ch_1_0())
  RH_L=((RH_L<<1)+1);
  else
  RH_L<<=1;
  }
  for(i=0;i<8;i++)
  {
  if(ch_1_0())
  CR_D=((CR_D<<1)+1);
  else
  CR_D<<=1;
  }
}
