#include"AT_PRC.H"	
#include"STC15FXXXX.H"
#include"DELAY.h"
#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7


#define MAIN_Fosc       22118400L  //������ʱ��
#define Timer0_Rate     25000       //�ж�Ƶ��


typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;



#define Timer0_Reload   (65536UL -(MAIN_Fosc / Timer0_Rate)) 

void at_uart_send_str(unsigned char *str)//����AT�ַ���������
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
void at_uart_send_buf(unsigned char *str,unsigned char len)//�������ݻ������ķ��ַ�����Ϣ����������Ϣ������
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
	S1_8bit();				//8λ����
	S1_USE_P30P31();		//UART1 ʹ��P30 P31��	Ĭ��
	AUXR &= ~(1<<4);	//Timer stop		������ʹ��Timer2����
	AUXR |= 0x01;		//S1 BRT Use Timer2;
	AUXR |=  (1<<2);	//Timer2 set as 1T mode
	TH2 = (unsigned char)(Timer2_Reload >> 8);
	TL2 = (unsigned char)Timer2_Reload;
	AUXR |=  (1<<4);	//Timer run enable
	REN = 1;	//�������
	ES  = 1;	//�����ж�
	EA = 1;		//����ȫ���ж�
	PS=1;
	P3M1 = 0x00;
    P3M0 = 0xFF;
	RX1_Cnt=0;
	ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
    ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
    P_SW1 = ACC; 				 //(P3.0/RX,P3.1/TX)
}



void make_AT_SEND_LEN(unsigned char a_len,unsigned char DATA_S)   //�����ұ�������ָ�������a_len �ĳ�10���ƣ��ұ�����ָ��  "AT+CIPSEND=XX" XX�Ƿ��͵�����
{
  unsigned char aa=0;
  for(aa=0;aa<20;aa++)
  at_send_len_ox[aa]=0;

  for(aa=0;aa<11;aa++)
  {
    if(DATA_S==LOC_data)
    at_send_len_ox[aa]=CIPSEND_LEN[aa];	 //����"AT+CIPSEND= ��RAM  �����ʮ���Ʋ���������Ĳ�������
	else if(DATA_S==RMT_data)
	at_send_len_ox[aa]=AT_CLDSEND[aa];	 //����"AT+CIPSEND= ��RAM  �����ʮ���Ʋ���������Ĳ�������
  }
  t_o=0;                                 //ȥ��ǰ���0�����緢��38���ֽڣ�038��ǰ���0�Ϳ���ȥ���ˡ�
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

