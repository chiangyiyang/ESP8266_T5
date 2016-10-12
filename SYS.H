 
#ifndef __SYS_H__
#define __SYS_H__

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;
#define MAIN_Fosc		22118400L	//定义主时钟

#include "STC15Fxxxx.H"
 /*************	本地常量声明	**************/
#define MAIN_Fosc		22118400L	//定义主时钟
#define	RX1_Lenth		32			//串口接收缓冲长度
#define	BaudRate1		115200UL	//选择波特率
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 重装值， 对应300KHZ
#define	Timer2_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 2 重装值， 对应300KHZ

/*************	本地变量声明	**************/
u8	idata RX1_Buffer[RX1_Lenth];	//接收缓冲
u8	TX1_Cnt;	//发送计数
u8	RX1_Cnt;	//接收计数
bit	B_TX1_Busy;	//发送忙标志
/*************	端口引脚定义	**************/

bit have_tok=0;
bit have_data=0;
unsigned char rec_len=0;
bit                   a_vec=0;		   //设置一个标志位，串口程序 出现指定的字串，并动态过滤到指定的字符后 置1
unsigned char          ceng=0;         //最多 多少字节内 会出现指定字串,静态变量，
unsigned char str_len_limt=16;         //设置一个限定参考数值，在这个参考个数内必定出现指定字符串
unsigned char   str_len_num=11;        //字符个数
char str_ref=':';
unsigned char two_lab=0;

sbit LED1=P1^0;//LED1
sbit LED2=P1^1;//LED2
sbit LED3=P1^2;//LED3

sbit DK1=P3^3;//继电器
sbit BEEP=P3^4;//蜂鸣器
sbit WIFI_VCC=P5^4;//蜂鸣器
sbit K1=P1^3;//按键1
sbit K2=P1^4;//按键2
sbit K3=P1^5;//按键3
bit have_config=0;
bit unlink=0;
#define LOC_data 0
#define RMT_data 1
/*

1 加入云功能：        AT+CLDSTART 
2 停止云功能:         AT+CLDSTOP
3 向云传输数据:       AT+CLDSEND 
4 启用本地局域网功能: AT+LANSTART 
5 停止本地局域网功能: AT+LANSTOP  
6 退出家庭组：        AT+CLDUNBIND 
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
/////////////////////////////////////////////////////////////////////以上字串是单片机发给串口模块的AT语句有的是直接给WIFI模块的指令，有些是用来拷贝字符串用/////////

unsigned char at_send_len_ox[20];                
//xdata unsigned char send_buf[64]="jisifsfhello:99999;oop";	                     //发送缓冲区
idata unsigned char recd_buf[64]= {0x55,0x1e,0x00,0xa0 ,0x00 ,0xf3 ,0xc0 ,0x00 ,0x00, 0x00, 0x54, 0x01 ,0x9f, 0xa6 ,0x00,0x00, 0x00, 0x00, 0x00 ,0x08, 0x00, 0xcc ,0x54 ,0x39, 0x3a ,0x66, 0xaa ,0x1c ,0xb6,0xc5,0X55};						 //接收缓冲区
//data unsigned char temp_buf[72];				 //加工缓冲区
//idata unsigned char toke[8]={0x24,0x41,0xD6,0x39,0x48,0x83,0xAC,0x00};//此设备在服务器上获得的令牌包
//unsigned char esp_dev_id[8]={0,0,0,0,0,0,0X1E,0XDE};         //    8266的8个字节的器件ID号，可在服务器数据库中查到，唯一标示一个器件，登录过程需要一个器件ID,和数据区放一个数据密码，这么简单登录
//unsigned char esp_TOK_id[8]={0,0,0,0,0,0,0,0};				 //    服务器分给器件器件的令牌包，另外个地方也定义了，完全可以用一个数组完成的
//unsigned char esp_user_data[14]={0,0,0,0,0,0,0X1E,0XDE};	 //    客户的净数据负荷区，可以很大，因为本款单片机有限，并且一般控制信号，定义几个字节够了！注意在登录的时候，这里是器件密码！
unsigned char temp_cd[]="TEMP:123;";						 //    一个数据包，前面是包格式定义，后面是客户数据区，这里定义一个即将要发送的温度数据
code unsigned char switch_sta1[]="LIGHT:1;";
code unsigned char switch_sta0[]="LIGHT:0;";
unsigned char need_seed_len=0;								 //    全局变量，本次总共需要发到串口的数据
bit t_o=0;													 //  在构造一个如00123 的数据时候，去掉前面的00变成123 这里若碰到0就置1
code unsigned char cip3_lcport[]="2469,0";
//data unsigned char chip_id[8]={'0','0','0','0','0','0','0','0'};
//data unsigned char flash_id[8]={'0','0','0','0','0','0','0','0'};
//pdata unsigned char pass_id[8]={'0','0','0','0','0','0','0','0'};

unsigned int time=0; //每隔30秒把一个变量加1，然后把这个变量作为温度数据上报给云平台，转给手机端

//xdata unsigned char ssid[32];	  //暂存SSID账户信息
//xdata unsigned char password[20]; //暂存客户密码
//idata char ssid_len=0;	          //记录SSID 长度
//idata char pasd_len=0;			  //记录密码长度

bit have_id=0;					  //记录是否用AT指令获取到了这个模块的ID 和KEY信息
bit have_smartlink=0;			  //记录是否过滤到 从串口来的TCP UDP 数据  smartlink 
bit have_bander=0;				  //记录是从串口中输出的网络数据中过滤到 手机用UDP发来的请求信息

data unsigned char stac_a=0;		      //全局专用变量
unsigned char stac_b=0;			  //全局专用变量
extern bit led_sta;
extern bit led_data;
#define uart_rec_tcp_udp_data 0	//系统进入正常网络数据收发状态
#define uart_rec_csysid       1	//系统获取ESP8266WIFI模块的许可号状态，获取四个字节的芯片ID 和 4个字节的授权码
#define uart_rec_smartlink    2	//系统进入获取上网账号密码状态
#define uart_rec_bander       3	//系统进入将本WIFI设备 和APP 手机绑定控制状态
#define uart_rmt_tcp_udp_data 4	//系统进入云端网络数据收发状态
code char CYSYS_code[]="+CSYSID:CHIP";
code char PIPD_code[]="+IPD,";
code char PCLD_code[]="LIGHT:";
code char LIGHTR_code[]="LIGHTR:";
code char LIGHTG_code[]="LIGHTG:";
code char LIGHTB_code[]="LIGHTB:";
code char bander_code[]="+IPD,4,26:RPL:";//这里过滤有漏洞
code char smartlink_code[]="WIFI GOT";
/////////////////////////////////////////////////////////////////////以上是从WIFI模块串口输出到本单片机串口输入后，本单片机要过滤的各种头部////////////////////
unsigned char uart_rec_sta=uart_rec_csysid;// 串口目前所处于的状态，比如进入正常数据接收，或者SMARTLINK 或者 获取模块的ID。
//////////////////////////////////////////////////////////////////////以上部分定义了系统的几种状态///////////////////////////////////////
#endif 
