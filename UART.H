
#define MAIN_Fosc		22118400L	//定义主时钟
#include"STC15FXXXX.H"
#define uart_rec_tcp_udp_data 0	//系统进入正常网络数据收发状态
#define uart_rec_csysid       1	//系统获取ESP8266WIFI模块的许可号状态，获取四个字节的芯片ID 和 4个字节的授权码
#define uart_rec_smartlink    2	//系统进入获取上网账号密码状态
#define uart_rec_bander       3	//系统进入将本WIFI设备 和APP 手机绑定控制状态
typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

extern bit have_tok;
extern bit have_data;
extern unsigned char rec_len;
extern bit                   a_vec;		   //设置一个标志位，串口程序 出现指定的字串，并动态过滤到指定的字符后 置1
extern unsigned char          ceng;         //最多 多少字节内 会出现指定字串,静态变量，
extern unsigned char str_len_limt;         //设置一个限定参考数值，在这个参考个数内必定出现指定字符串
extern unsigned char   str_len_num;        //字符个数
extern char str_ref;
extern unsigned char uart_rec_sta;

extern code char CYSYS_code[];
extern code char PIPD_code[];
extern code char PCLD_code[];

extern code char bander_code[];//这里过滤有漏洞
extern code char smartlink_code[];

extern u8	TX1_Cnt;	//发送计数
extern u8	RX1_Cnt;	//接收计数
extern bit	B_TX1_Busy;	//发送忙标志

extern xdata unsigned char send_buf[64];	                     //发送缓冲区
extern idata unsigned char recd_buf[64];						 //接收缓冲区
extern data unsigned char temp_buf[72];
extern unsigned char two_lab;
extern bit unlink;
extern unsigned char count_relink;
sbit LED1=P1^0;//LED1
sbit LED2=P1^1;//LED2
sbit LED3=P1^2;//LED3

sbit DK1=P3^3;//继电器
sbit BEEP=P3^4;//蜂鸣器
sbit K1=P1^3;//按键1
sbit K2=P1^2;//按键2
sbit K3=P1^4;//按键3
bit led_sta=0;
bit led_data=0;
unsigned char uart_time_out=0;
bit uart_rec_date=0;
bit uart_rec_new=0;

extern void change_pack();					  //把接收到的数据包转义过来，0X55 转义成0X54 0X01 0X54 替换成0X54 02

extern idata unsigned char toke[8];//此设备在服务器上获得的令牌包
extern bit have_id;					  //记录是否用AT指令获取到了这个模块的ID 和KEY信息
extern bit have_smartlink;			  //记录是否过滤到 从串口来的TCP UDP 数据  smartlink 
extern bit have_bander;				  //记录是从串口中输出的网络数据中过滤到 手机用UDP发来的请求信息

/////这是安信可云平台数据包的全部组成结构，细节可参照安信可的云平台V1.0版本规格书，包结构章节
#define O_PF    0X00  //包头1字节固定0X55 
#define O_LEN_L 0X01  //整个包长低字节
#define O_LEN_H 0X02  //整个包长高字节 注意转义码 两个这里只计算一个数据处理！
#define O_CMD_T 0X03  //命令类型
#define O_CMD_C 0X04  //具体命令
#define O_CIX_L 0X05  //本命令序列编号低字节
#define O_CIX_H 0X06  //本命令序列编号高字节
#define O_EXMSH 0X07  //扩展信息高字节
#define O_EXMSL 0X08  //扩展信息低字节
#define O_RESTA 0X09  //数据包状态信息，成功 失败 请求 未知
#define O_DEVID_START 0x0A  //8字节设备识别  低字节在前
#define O_DEVID 0x0A  //8字节设备识别
#define O_TK_LEN      0X12 //1 BYTS TL_LEN	  //获得的设备临时通讯令牌长度
#define O_TOKE_START  0X13
#define O_DATAS_START 0X1B

#define uart_rec_tcp_udp_data 0	//系统进入正常网络数据收发状态
#define uart_rec_csysid       1	//系统获取ESP8266WIFI模块的许可号状态，获取四个字节的芯片ID 和 4个字节的授权码
#define uart_rec_smartlink    2	//系统进入获取上网账号密码状态
#define uart_rec_bander       3	//系统进入将本WIFI设备 和APP 手机绑定控制状态
#define uart_rmt_tcp_udp_data 4
//N BYTS DATAS	  //客户的数据包串
//CRC_L    1BYTE	  //CRC16 低字节
//CRC_H    1BYTE	  //CRC16 高字节
//PACK_END 1BYTE	  //包尾1字节固定0X55

////////////////////////////////////////////////////////////////////////以上定义了包中的各参数的绝对位置//////////////////////////////////
/////////////////////////////////////////////////////////////////////以上是从WIFI模块串口输出到本单片机串口输入后，本单片机要过滤的各种头部////////////////////
