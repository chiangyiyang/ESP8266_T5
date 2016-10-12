
void change_pack();					  //把接收到的数据包转义过来，0X55 转义成0X54 0X01 0X54 替换成0X54 02
extern data unsigned char stac_a;		      //全局专用变量
extern unsigned char stac_b;			  //全局专用变量
extern xdata unsigned char send_buf[64];	                     //发送缓冲区
extern idata unsigned char recd_buf[64];						 //接收缓冲区
extern data unsigned char temp_buf[72];
extern unsigned char two_lab;
extern void make_AT_CIP3(void);
extern code unsigned char AT_CIP3[];
extern code unsigned char cip3_lcport[];
extern unsigned char esp_dev_id[8]; 
extern unsigned char esp_user_data[14];
extern unsigned char at_send_len_ox[20];
extern code unsigned char temp_bander[];
extern code unsigned char CIPSEND_LEN[];
extern code unsigned char AT_CLDSEND[];
extern bit t_o;	
												 //  在构造一个如00123 的数据时候，去掉前面的00变成123 这里若碰到0就置1
//将16进制数，变成16进制字符比如10变成A
extern char back_char(unsigned char user_d);

////制作 向手机发送如下格式UDP数据
////RPT:"0x00FE6738","0xB8B3C281","192.168.0.123","light","123456a"
extern void make_bander_data();


extern void make_AT_SEND_LEN(unsigned char a_len,unsigned char DATA_S);   //生成右边这样的指令，将参数a_len 改成10进制，右边这条指令  "AT+CIPSEND=XX" XX是发送的数量

extern void at_uart_send_str(unsigned char *str);//发送AT字符串到串口

extern void at_uart_send_buf(unsigned char *str,unsigned char len);//发送数据缓冲区的非字符串信息，数据流信息到串口


extern void init_uart(void);

extern unsigned char	TX1_Cnt;	//发送计数
extern unsigned char	RX1_Cnt;	//接收计数
extern bit	B_TX1_Busy;	//发送忙标志

#define	RX1_Lenth		32			//串口接收缓冲长度
#define	BaudRate1		115200UL	//选择波特率
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 重装值， 对应300KHZ
#define	Timer2_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 2 重装值， 对应300KHZ
#define LOC_data 0
#define RMT_data 1
#define MAIN_Fosc		22118400L	//定义主时钟



