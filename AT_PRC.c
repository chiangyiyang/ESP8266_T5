#include"AT_PRC.H"	
#include"STC15FXXXX.H"
#include"DELAY.h"
#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7


#define MAIN_Fosc       22118400L  //定义主时钟
#define Timer0_Rate     25000       //中断频率


typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;



#define Timer0_Reload   (65536UL -(MAIN_Fosc / Timer0_Rate)) 

void at_uart_send_str(unsigned char *str)//发送AT字符串到串口
{
  unsigned char *st_p=str;
  do{
     SBUF=*st_p;
	 st_p++;
	 Delay2(1);
	}while(*st_p);
	SBUF='\r';
	Delay2(1);
	SBUF='\n';
	Delay2(1);
}
void at_uart_send_buf(unsigned char *str,unsigned char len)//发送数据缓冲区的非字符串信息，数据流信息到串口
{
  unsigned char *st_p=str;
  
  while(len){
     SBUF=*st_p;
	 st_p++;
	 Delay2(1);
	 len--;
	}
	Delay2(1);
}

void init_uart(void)
{
  	B_TX1_Busy = 0;
	RX1_Cnt = 0;
	TX1_Cnt = 0;
	S1_8bit();				//8位数据
	S1_USE_P30P31();		//UART1 使用P30 P31口	默认
	AUXR &= ~(1<<4);	//Timer stop		波特率使用Timer2产生
	AUXR |= 0x01;		//S1 BRT Use Timer2;
	AUXR |=  (1<<2);	//Timer2 set as 1T mode
	TH2 = (unsigned char)(Timer2_Reload >> 8);
	TL2 = (unsigned char)Timer2_Reload;
	AUXR |=  (1<<4);	//Timer run enable
	REN = 1;	//允许接收
	ES  = 1;	//允许中断
	EA = 1;		//允许全局中断
	PS=1;
	P3M1 = 0x00;
    P3M0 = 0xFF;
	RX1_Cnt=0;
	ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
    ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
    P_SW1 = ACC; 				 //(P3.0/RX,P3.1/TX)
}



void make_AT_SEND_LEN(unsigned char a_len,unsigned char DATA_S)   //生成右边这样的指令，将参数a_len 改成10进制，右边这条指令  "AT+CIPSEND=XX" XX是发送的数量
{
  unsigned char aa=0;
  for(aa=0;aa<20;aa++)
  at_send_len_ox[aa]=0;

  for(aa=0;aa<11;aa++)
  {
    if(DATA_S==LOC_data)
    at_send_len_ox[aa]=CIPSEND_LEN[aa];	 //剪贴"AT+CIPSEND= 到RAM  后面的十进制参数由下面的部分生成
	else if(DATA_S==RMT_data)
	at_send_len_ox[aa]=AT_CLDSEND[aa];	 //剪贴"AT+CIPSEND= 到RAM  后面的十进制参数由下面的部分生成
  }
  t_o=0;                                 //去掉前面的0，比如发送38个字节，038，前面的0就可以去掉了。
  if((a_len/100))
  {
  at_send_len_ox[aa]=a_len/100+'0';
  aa++;
  t_o=1;
  }
  if((a_len%100)/10)
  {
  at_send_len_ox[aa]=(a_len%100)/10+'0';
  aa++;
  t_o=1;
  }
  else if(t_o)
  {
    at_send_len_ox[aa]=0+'0';
	aa++;
  }
  at_send_len_ox[aa]=(a_len%10)+'0';
  aa++;
  at_send_len_ox[aa]=0;
}

