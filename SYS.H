 
#ifndef __SYS_H__
#define __SYS_H__

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;
#define MAIN_Fosc		22118400L	//������ʱ��

#include "STC15Fxxxx.H"
 /*************	���س�������	**************/
#define MAIN_Fosc		22118400L	//������ʱ��
#define	RX1_Lenth		32			//���ڽ��ջ��峤��
#define	BaudRate1		115200UL	//ѡ������
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 ��װֵ�� ��Ӧ300KHZ
#define	Timer2_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 2 ��װֵ�� ��Ӧ300KHZ

/*************	���ر�������	**************/
u8	idata RX1_Buffer[RX1_Lenth];	//���ջ���
u8	TX1_Cnt;	//���ͼ���
u8	RX1_Cnt;	//���ռ���
bit	B_TX1_Busy;	//����æ��־
/*************	�˿����Ŷ���	**************/

bit have_tok=0;
bit have_data=0;
unsigned char rec_len=0;
bit                   a_vec=0;		   //����һ����־λ�����ڳ��� ����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
unsigned char          ceng=0;         //��� �����ֽ��� �����ָ���ִ�,��̬������
unsigned char str_len_limt=16;         //����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
unsigned char   str_len_num=11;        //�ַ�����
char str_ref=':';
unsigned char two_lab=0;

sbit LED1=P1^0;//LED1
sbit LED2=P1^1;//LED2
sbit LED3=P1^2;//LED3

sbit DK1=P3^3;//�̵���
sbit BEEP=P3^4;//������
sbit WIFI_VCC=P5^4;//������
sbit K1=P1^3;//����1
sbit K2=P1^4;//����2
sbit K3=P1^5;//����3
bit have_config=0;
bit unlink=0;
#define LOC_data 0
#define RMT_data 1
/*

1 �����ƹ��ܣ�        AT+CLDSTART 
2 ֹͣ�ƹ���:         AT+CLDSTOP
3 ���ƴ�������:       AT+CLDSEND 
4 ���ñ��ؾ���������: AT+LANSTART 
5 ֹͣ���ؾ���������: AT+LANSTOP  
6 �˳���ͥ�飺        AT+CLDUNBIND 
*/
unsigned char count_relink=0;
code unsigned char AT_CLDSEND[]="AT+CLDSEND=10"; 
code unsigned char AT_CLDSTART[]="AT+CLDSTART";
code unsigned char AT_CUB[]="AT+CLDUNBIND";
code unsigned char AT_RST[]="AT+RST"; 	                                             
code unsigned char AT_MODE[]="AT+CWMODE=1";
code unsigned char AT_CWJAP[]="AT+CWJAP=\"YY WIFI\",\"mary1234\"";
code unsigned char AT_CIP3[]="AT+CIPSTART=3,\"UDP\",\"cloud.ai-thinker.com\",5001,2468,0" ;
code unsigned char temp_bander[]="RPT:\"0xa1b23467\",\"0xaf321234\",\"192.168.0.123\",\"light\",\"123456a\"";
code unsigned char AT_CIPMUX[]="AT+CIPMUX=1";
code unsigned char CIPSTART[]="AT+CIPSTART=4,\"UDP\",\"cloud.ai-thinker.com\",5001,2468,0";
code unsigned char AT_CSYSID[]="AT+CSYSID";
code unsigned char AT_SMARTLINK[]="AT+CWSMARTSTART=1";
code unsigned char AT_LANSTART[]="AT+LANSTART";
code unsigned char ZERO[]="00000000000000000000000000000000001";
code unsigned char CIPSEND_LEN[]="AT+CIPSEND=4,";
code unsigned char CIP3SEND_LEN[]="AT+CIPSEND=3,63";
/////////////////////////////////////////////////////////////////////�����ִ��ǵ�Ƭ����������ģ���AT����е���ֱ�Ӹ�WIFIģ���ָ���Щ�����������ַ�����/////////

unsigned char at_send_len_ox[20];                
//xdata unsigned char send_buf[64]="jisifsfhello:99999;oop";	                     //���ͻ�����
idata unsigned char recd_buf[64]= {0x55,0x1e,0x00,0xa0 ,0x00 ,0xf3 ,0xc0 ,0x00 ,0x00, 0x00, 0x54, 0x01 ,0x9f, 0xa6 ,0x00,0x00, 0x00, 0x00, 0x00 ,0x08, 0x00, 0xcc ,0x54 ,0x39, 0x3a ,0x66, 0xaa ,0x1c ,0xb6,0xc5,0X55};						 //���ջ�����
//data unsigned char temp_buf[72];				 //�ӹ�������
//idata unsigned char toke[8]={0x24,0x41,0xD6,0x39,0x48,0x83,0xAC,0x00};//���豸�ڷ������ϻ�õ����ư�
//unsigned char esp_dev_id[8]={0,0,0,0,0,0,0X1E,0XDE};         //    8266��8���ֽڵ�����ID�ţ����ڷ��������ݿ��в鵽��Ψһ��ʾһ����������¼������Ҫһ������ID,����������һ���������룬��ô�򵥵�¼
//unsigned char esp_TOK_id[8]={0,0,0,0,0,0,0,0};				 //    �������ָ��������������ư���������ط�Ҳ�����ˣ���ȫ������һ��������ɵ�
//unsigned char esp_user_data[14]={0,0,0,0,0,0,0X1E,0XDE};	 //    �ͻ��ľ����ݸ����������Ժܴ���Ϊ���Ƭ�����ޣ�����һ������źţ����弸���ֽڹ��ˣ�ע���ڵ�¼��ʱ���������������룡
unsigned char temp_cd[]="TEMP:123;";						 //    һ�����ݰ���ǰ���ǰ���ʽ���壬�����ǿͻ������������ﶨ��һ������Ҫ���͵��¶�����
code unsigned char switch_sta1[]="LIGHT:1;";
code unsigned char switch_sta0[]="LIGHT:0;";
unsigned char need_seed_len=0;								 //    ȫ�ֱ����������ܹ���Ҫ�������ڵ�����
bit t_o=0;													 //  �ڹ���һ����00123 ������ʱ��ȥ��ǰ���00���123 ����������0����1
code unsigned char cip3_lcport[]="2469,0";
//data unsigned char chip_id[8]={'0','0','0','0','0','0','0','0'};
//data unsigned char flash_id[8]={'0','0','0','0','0','0','0','0'};
//pdata unsigned char pass_id[8]={'0','0','0','0','0','0','0','0'};

unsigned int time=0; //ÿ��30���һ��������1��Ȼ������������Ϊ�¶������ϱ�����ƽ̨��ת���ֻ���

//xdata unsigned char ssid[32];	  //�ݴ�SSID�˻���Ϣ
//xdata unsigned char password[20]; //�ݴ�ͻ�����
//idata char ssid_len=0;	          //��¼SSID ����
//idata char pasd_len=0;			  //��¼���볤��

bit have_id=0;					  //��¼�Ƿ���ATָ���ȡ�������ģ���ID ��KEY��Ϣ
bit have_smartlink=0;			  //��¼�Ƿ���˵� �Ӵ�������TCP UDP ����  smartlink 
bit have_bander=0;				  //��¼�ǴӴ�������������������й��˵� �ֻ���UDP������������Ϣ

data unsigned char stac_a=0;		      //ȫ��ר�ñ���
unsigned char stac_b=0;			  //ȫ��ר�ñ���
extern bit led_sta;
extern bit led_data;
#define uart_rec_tcp_udp_data 0	//ϵͳ�����������������շ�״̬
#define uart_rec_csysid       1	//ϵͳ��ȡESP8266WIFIģ������ɺ�״̬����ȡ�ĸ��ֽڵ�оƬID �� 4���ֽڵ���Ȩ��
#define uart_rec_smartlink    2	//ϵͳ�����ȡ�����˺�����״̬
#define uart_rec_bander       3	//ϵͳ���뽫��WIFI�豸 ��APP �ֻ��󶨿���״̬
#define uart_rmt_tcp_udp_data 4	//ϵͳ�����ƶ����������շ�״̬
code char CYSYS_code[]="+CSYSID:CHIP";
code char PIPD_code[]="+IPD,";
code char PCLD_code[]="LIGHT:";
code char LIGHTR_code[]="LIGHTR:";
code char LIGHTG_code[]="LIGHTG:";
code char LIGHTB_code[]="LIGHTB:";
code char bander_code[]="+IPD,4,26:RPL:";//���������©��
code char smartlink_code[]="WIFI GOT";
/////////////////////////////////////////////////////////////////////�����Ǵ�WIFIģ�鴮�����������Ƭ����������󣬱���Ƭ��Ҫ���˵ĸ���ͷ��////////////////////
unsigned char uart_rec_sta=uart_rec_csysid;// ����Ŀǰ�����ڵ�״̬����������������ݽ��գ�����SMARTLINK ���� ��ȡģ���ID��
//////////////////////////////////////////////////////////////////////���ϲ��ֶ�����ϵͳ�ļ���״̬///////////////////////////////////////
#endif 