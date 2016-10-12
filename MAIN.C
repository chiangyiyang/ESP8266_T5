/*
本测试程序在安信可科技30元 的测试板上正确运行，所有安信可出的的模块从ESP-01到ESP-14均可以正确运行，与远程控制。
*/
#include"SYS.H"
#include"AT_PRC.H"
#include"delay.h"
#include <string.h>
#include <stdio.h> /* for printf */

extern void PWM(unsigned char r,unsigned char g,unsigned char b);
extern void timer0_ini();
extern bit have_change_pwm;
extern void set_pwm(unsigned char data rr,unsigned char data gg,unsigned char data bb);
extern void read_dh(void);
extern unsigned char uart_time_out;
extern bit uart_rec_date;
extern bit uart_rec_new;
unsigned char bar_r=0;
unsigned char bar_g=0;
unsigned char bar_b=0;
unsigned char old_bar_r=0;
unsigned char old_bar_g=0;
unsigned char old_bar_b=0;

extern unsigned int delay_1min;
extern unsigned char TP_H;
extern unsigned char TP_L;
extern unsigned char RH_H;
extern unsigned char RH_L;
extern unsigned char CR_D;
extern bit min_rech;
code unsigned char RGB_CODE_R[]="LIGHTR:";
code unsigned char RGB_CODE_G[]="LIGHTG:";
code unsigned char RGB_CODE_B[]="LIGHTB:";
unsigned int key1_time=0;
unsigned char RGB_CODE[12];
char *ii;

/*-------------------------------------------------------------------------------------------------------
函数名称:
void ini_rec_buff(void)
函数功能:
把接收缓冲清零，设置串口接收机制为初始，串口搜索指针指向NULL
被本函数调用的函数清单:
无
调用本函数的函数清单:
void main(void)
输入参数说明:
无
输出参数的说明:
无
函数返回值的说明:
无
其它说明:
每次串口收到一串数据，要判断串口缓冲区是否包含关注的字符串，并想彻底清除上次接收到的缓冲，调用这个。
--------------------------------------------------------------------------------------------------------
*/
void ini_rec_buff(void)
{
  unsigned char len=0;  
  for(len=0;len<64;len++)
  recd_buf[len]=0;
  RX1_Cnt=0;
  uart_rec_date=1;
  uart_time_out=0;
  uart_rec_new=0;
  ii=NULL;
}
/*-------------------------------------------------------------------------------------------------------
函数名称:
void uptemp(void)
函数功能:
将温度和湿度信息 经过WIFI模块串口，发送到服务器，服务器转发给控制终端，比如手机或者电脑
被本函数调用的函数清单:
make_AT_SEND_LEN（）；//动态生成发送数据长度AT指令。
at_uart_send_str()	  //经过本函数发送温度信息
read_dh();			  //读取DHT11的温度和湿度信息
调用本函数的函数清单:
void main(void)
输入参数说明:
无
输出参数的说明:
无
函数返回值的说明:
无
其它说明:
--------------------------------------------------------------------------------------------------------
*/
void uptemp(void)
{
            if(min_rech)                                     //	定时器置位min_rech每分钟会执行到这里一次
			{
			min_rech=0;
		    need_seed_len=9;                                 
		    make_AT_SEND_LEN(need_seed_len,RMT_data); 		 //动态构造发送AT长度指令
			at_uart_send_str(at_send_len_ox);                //发送构造好的发送指令
		    Delay2(1000);			
		    at_uart_send_buf(temp_cd,need_seed_len);	     //经过WIFI开始发送数据
			LED2=0;
			BEEP=1;
			Delay2(200);
		    LED2=1;
			BEEP=0;                                          //闪亮心跳指示灯，和心跳音
	            							                 //每30秒会运行到这里一次。更新一次温度数值
			read_dh();
			temp_cd[5]=(((RH_H%100)/10)+'0');
			temp_cd[6]=(((RH_H%10))+'0');
		    temp_cd[7]=((RH_L)+'0');
			}		  
}
/*------------------------------------------------------------------------------------------------------
函数名称:
void ack_rgb_bar(unsigned char bar_mod)
函数功能:
模块经过服务器回应终端设备本WIFI模块控制的灯具红绿蓝的数值。手机或者电脑，收到远程模块回应过来的数值，才更新进度条
被本函数调用的函数清单:
make_AT_SEND_LEN（）；//动态生成发送数据长度AT指令。
at_uart_send_str()	  //经过本函数发送本地 R G B 的数值，从0-99
调用本函数的函数清单:
void main(void)
输入参数说明:
无
输出参数的说明:
无
函数返回值的说明:
无
其它说明:
本函数用来回馈本地状态到手机APP上，比如手机拖动红色灯进度条到85%，模块将收到LIGHR:85,模块调整红色灯亮度到85%后，回馈一个 LIGHR:85。
手机APP知道远程灯具已经执行了，于是跟新本地红色通道进度条到85%
--------------------------------------------------------------------------------------------------------
 */
void ack_rgb_bar(unsigned char bar_mod)
{
    unsigned char a=0;
	unsigned char temp_bar=0;
	for(a=0;a<12;a++)
	RGB_CODE[a]=0;
	for(a=0;a<7;a++)
	{
		if(bar_mod==1)
		{
		RGB_CODE[a]=RGB_CODE_R[a];
		}
		else if(bar_mod==2)
		{
		RGB_CODE[a]=RGB_CODE_G[a];
		}
		else if(bar_mod==3)
		{
		RGB_CODE[a]=RGB_CODE_B[a];
		}
	}
    if(bar_mod==1)
    temp_bar=bar_r;
    else if(bar_mod==2)
    temp_bar=bar_g;
    else if(bar_mod==3)
    temp_bar=bar_b;
	if(temp_bar>=10)
	{
	RGB_CODE[7]=(temp_bar/10)+'0';
	RGB_CODE[8]=(temp_bar%10)+'0';
	RGB_CODE[9]=0x0d;
	RGB_CODE[10]=0x0a;
	need_seed_len=11;
	}
	else
	{
	RGB_CODE[7]=temp_bar+'0';
	RGB_CODE[8]=0x0d;
	RGB_CODE[9]=0x0a;
	need_seed_len=10;
	}
	make_AT_SEND_LEN(need_seed_len,RMT_data); 		 //动态构造发送AT指令
	at_uart_send_str(at_send_len_ox);                //发送构造好的发送指令
	Delay2(50);
	at_uart_send_buf(RGB_CODE,need_seed_len);	     //经过WIFI发送数据  
} 
/*------------------------------------------------------------------------------------------------------
函数名称:
void main(void）
主函数主要完成功能如下：
1 打开WIFI供电，并让蜂鸣器不响，初始化各个变量到默认值。
2 初始化串口 中断  和PWM 以及定时器
3 判断上电时刻是否KEY1 被按下，若被按下后在下面环节，将进入SMARTLINK 状态，设置本模块连入家庭路由器的账户和密码信息
4 依次发送四条AT指令，让模块连到云服务器上，从而手机可以远程控制这个模块了。
  at_uart_send_str(AT_MODE);		 //设置模块进入STATION 模式
  at_uart_send_str(AT_SMARTLINK);	 //发送进入SMARTLINK AT指令，模块可用来获取家庭路由器账户密码连到互联网上。
  at_uart_send_str(AT_CLDSTART);     //访问安信可的云服务器加入安信可云。
  at_uart_send_str(AT_LANSTART);     //此命令，让模块经过本地局域网，监听本地手机发过来的查询信息，手机获得这个模块的信息后，这个手机即可向服务器申请对这个模块控制！
5 接着进行大循环了，经过上面的步骤，模块已经进入接受远程手机控制的状态，在这个循环中，只用关注串口过来的数据（远程手机经过服务器发过来的控制指令）
  比如 开关继电器，设置红绿蓝 LED亮度的命令，本机执行后，还需要告诉手机：我已经按照你的命令执行了！ 同时每分钟还要把本地温度 湿度 经过服务器 发送给远程手机APP上显示！
  另外在主循环中，还包含一个长按KEY2 5秒后，模组给云端服务器发送解绑指令，主动和这个手机解除绑定。
--------------------------------------------------------------------------------------------------------
*/

void delay_ms(unsigned int ms)
{
	unsigned long dly = ms * 236ul;
	while(dly--);
}



//void blink(u8 inx, u16 ms){
//		P2 &= ~(0x01<<(5+inx));
//		delay_ms(ms);
//		P2 |= (0x01<<(5+inx));
//		delay_ms(ms);
//}


//void dumpValue(u8 inx,int n){
//	while(n--)
//		blink(inx, 500ul);
//}

void main(void)
{
	
//	while(1){
//		P25=~P25;
//		Delay2(1000ul);
//	}

	
	
	char z=0;
	unsigned int h=0;
	unsigned int i;
	WIFI_VCC=0;		   //WIFI 电源受单片机控制，在异常的时候，单片机可以强制重启动ESP8266
	init_uart();	   // 初始化串口波特率和ESP8266 串口模组相同。
	BEEP=0;			   //关掉蜂鸣器
	Delay2(1000);	   //延迟启动，让系统更稳定
	old_bar_r=bar_r;   //初始化红绿蓝 灯的PWM 占空比0-99
	old_bar_g=bar_g;
	old_bar_b=bar_b;
	PWM(5,5,5);		   //让三个灯微亮
	timer0_ini();	   //初始化定时0，为一些需要时间基准服务的东西做准备。


		
	at_uart_send_str(AT_MODE);
	Delay2(500);
	ini_rec_buff();
	
	at_uart_send_str(AT_CIPMUX);		 
	Delay2(500);
	
	at_uart_send_str(AT_CWJAP);		 
	Delay2(1000);

	
	do								
	{
		PWM(i%100,(i+33)%100,(i+66)%100);
		Delay2(50);		
		i = i>99? 0: i+1;
	}while(strstr(recd_buf,smartlink_code)==NULL);
		
	
	while(1){
		PWM(5,5,5);
		while(!K1);
			at_uart_send_str(AT_MODE);
			Delay2(500);
		
			at_uart_send_str("AT+CIPMUX=0");		 
			Delay2(500);

			at_uart_send_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");		 
			Delay2(1000);

			at_uart_send_str("AT+CIPSEND=54");		 
			Delay2(1000);

			at_uart_send_str("GET /update?key=GJA4TBA1YS1V4VRW&field1=111&field2=123");		 
			Delay2(5000);
			
			at_uart_send_str("AT+CIPCLOSE");		 
			Delay2(500);

			while(1){
		//		BEEP=1; Delay2(500); BEEP=0;	
				PWM(i%100,(i+33)%100,(i+66)%100);
				Delay2(5);		
				i = i>99? 0: i+1;
				if(!K1) break;
			}
		
	}
	
	
//	if(K1==0)           //开机的时候发现按键1也就是MCU_P1.3 被按下，那么重新用AT指令配置WIFI模块连路由器的上网账户和密码
//	{
//			LED2=0;
//			BEEP=1;
//			Delay2(200);
//		    LED2=1;
//			BEEP=0;      //闪亮心跳指示灯，和心跳音
//			have_config=1;
//	}	 
//		at_uart_send_str(AT_MODE);		 //设置模块进入STATION 模式
//  	    Delay2(500);
//	    ini_rec_buff();	
//		if(have_config)					 //需要配置 模块连入本地的路由器账户和密码
//		{ 
//		at_uart_send_str(AT_SMARTLINK);	 //发送进入SMARTLINK AT指令
//		Delay2(500);
//		}
	
//		do								 //此循环完成SMARTLINK 的配置
//		{
//		BEEP=1;
//		Delay2(500);
//		BEEP=0;
//		Delay2(500);
//	    }while(strstr(recd_buf,smartlink_code)==NULL); //此循环完成SMARTLINK 的配置

//		uart_rec_new=0;
//		at_uart_send_str(AT_CLDSTART); //加入安信可云
//		Delay2(1000);
//		at_uart_send_str(AT_LANSTART);//开启内网模式（此模式下，模组会用内网回应手机APP 要查询的ID 和KEY）
//		Delay2(500);
//		while(1)
//		{  
//		if(have_change_pwm)		     //定时器置位后，10毫秒进入一次 假如在循环中检测到需要改变R G B 灯的变化。每10MS 加1或者减1 PWM数值，从最暗到最亮需要2.56秒
//		{							 //这里用来做渐亮或者渐灭效果
//		     have_change_pwm=0;		 //清标志
//			 if(K1==0)				 //假如K1被按下，记录按下的时间长短。
//		     key1_time++;
//		     else
//		     key1_time=0;
//		     if(key1_time>500)	     //K1 被连续按下超过5秒
//		     {
//		      key1_time=0;
//		      at_uart_send_str(AT_CUB);	//超过5秒后，模组经过AT指令向服务器发出退出家庭组命令
//		      BEEP=1;
//		      Delay2(100);				//蜂鸣器响三声，代表模块已经与这个账号解绑
//		      BEEP=0;
//		      Delay2(100);
//		      BEEP=1;
//			  Delay2(100);
//		      BEEP=0;
//		      Delay2(100);
//		      BEEP=1;
//		      Delay2(100);
//		      BEEP=0;
//		   } 
//		  	 if(bar_r>old_bar_r)//bar_r 这个变量中记录了最新从WIFI模块串口获得的PWM数值，若发现和旧的PWM数值有出入，说明需要改变！那就把老的记录升1或者降1
//			 {											   
//			   old_bar_r++;
//			   set_pwm(old_bar_r,old_bar_g,old_bar_b);//设置加1后的PWM到PWM寄存器中，灯的亮度会变化
//			 }
//			 else if(bar_r<old_bar_r)//bar_r 这个变量中记录了最新从WIFI模块串口获得的PWM数值，若发现和旧的PWM数值有出入，说明需要改变！那就把老的记录升1或者降1
//			 {
//			   old_bar_r--;
//			   set_pwm(old_bar_r,old_bar_g,old_bar_b);
//			 }
//			 if(bar_g>old_bar_g)
//			 {
//			   old_bar_g++;
//			   set_pwm(old_bar_r,old_bar_g,old_bar_b);
//			 }
//			 else if(bar_g<old_bar_g)
//			 {
//			   old_bar_g--;
//			   set_pwm(old_bar_r,old_bar_g,old_bar_b);
//			 }
//			 if(bar_b>old_bar_b)
//			 {
//			   old_bar_b++;
//			   set_pwm(old_bar_r,old_bar_g,old_bar_b);
//			 }
//			 else if(bar_b<old_bar_b)
//			 {
//			   old_bar_b--;
//			   set_pwm(old_bar_r,old_bar_g,old_bar_b);
//			 }
//		}	
//		if(uart_rec_new)//这个标志位为1，代表WIFI 收到数据从串口输出到MCU的串口缓冲区了，并且构成一小段，可以分析了。定时器计时，从收到从串口最后一个字节开始10MS，若没有新数据到来。就算一整段结束！
//		{
//		 if((ii=strstr(recd_buf,PCLD_code))!=NULL) // strstr这个函数是KEIL编译器的字符串库函数，功能是从一个字符串中查找是否存在指定的字符串，没找到返回NULL，找到后返回找到的位置，就是指针！
//		 {										   // 找到LIGHT: 这样的字符串存在于串口缓冲区中
//			if(*(ii+6)=='0')					   //  LIGHT:0 判断最后一个字符，是0代表要关继电器
//			{
//              DK1=0;						//关掉继电器
//			  need_seed_len=8;              //构造上传数据到云，转给手机的温度数据包，符合基本数据格式
//			  make_AT_SEND_LEN(need_seed_len,RMT_data); 					 //动态构造发送AT指令
//			  at_uart_send_str(at_send_len_ox);                //发送构造好的发送指令
//			  Delay2(50);
//			  at_uart_send_buf(switch_sta0,need_seed_len);	     //经过WIFI发送数据
//			  ///////////////////////////上面的几个AT发送函数，把已经关掉继电器的消息通过服务器告诉APP！APP收到后把界面上的灯设置为灰色关闭状态
//			  Delay2(20);
//			  bar_r=0;
//			  bar_g=0;
//			  bar_b=0;	     //点APP关灯状态，把本地的R G B三个灯的PWM的占空比 全设置为0
//			  ack_rgb_bar(1);
//			  ack_rgb_bar(1);
//			  ack_rgb_bar(2);
//			  Delay2(50);
//			  ack_rgb_bar(3);//上面几个ack_rgb_bar 函数，把本地RGB 的占空比经过云服务器反馈给手机APP，让手机APP更新拖动条
//			  Delay2(50);
//			}
//			if(*(ii+6)=='1')				      //  LIGHT:1 判断最后一个字符，是0代表要开继电器
//			{
//              DK1=1;
//			  need_seed_len=8;              //构造上传数据到云，转给手机的温度数据包，符合基本数据格式
//			  make_AT_SEND_LEN(need_seed_len,RMT_data); 					 //动态构造发送AT指令
//			  at_uart_send_str(at_send_len_ox);                  //发送构造好的发送指令
//			  Delay2(20);
//			  at_uart_send_buf(switch_sta1,need_seed_len);	     //经过WIFI发送数据 
//			  Delay2(50);
//			  bar_r=99;
//			  bar_g=99;
//			  bar_b=99;
//			  ack_rgb_bar(1);
//			  ack_rgb_bar(1);
//			  ack_rgb_bar(2);
//			  Delay2(50);
//			  ack_rgb_bar(3);
//			  Delay2(50);
//			}
//			ini_rec_buff();
//			uart_rec_new=0;
//		 }
//		 else if(((ii=strstr(recd_buf,LIGHTR_code))!=NULL)&&(old_bar_r==bar_r))	//在串口缓冲区中，找到LIGHTR:XX 这样设置红绿蓝PWM 的字符串命令
//		 {																		//当新旧PWM数值相同，也就是上次操作的亮度PWM渐变过程完成后（old_bar_r==bar_r)才允许进行
//		   old_bar_r=bar_r;														
//		   if((*(ii+8)==0)||(*(ii+8)==0x0d))
//		   bar_r=(*(ii+7)-'0');
//		   else if((*(ii+9)==0)||(*(ii+9)==0x0d))
//		   bar_r=(*(ii+7)-'0')*10+(*(ii+8)-'0');  //上面的语句过去出LIGHTR;XX 中把XX这个字符，变成数字！然后才能给PWM赋值
//		   ack_rgb_bar(1);						  //模块回应信息给远程手机上的APP 更新进度条
//		 }
//		 else if(((ii=strstr(recd_buf,LIGHTG_code))!=NULL)&&(old_bar_g==bar_g))
//		 {
//		   old_bar_g=bar_g;
//		   if((*(ii+8)==0)||(*(ii+8)==0x0d))
//		   bar_g=(*(ii+7)-'0');
//		   else if((*(ii+9)==0)||(*(ii+9)==0x0d))
//		   bar_g=(*(ii+7)-'0')*10+(*(ii+8)-'0');
//		   ack_rgb_bar(2);
//		 }
//		 else if(((ii=strstr(recd_buf,LIGHTB_code))!=NULL)&&(old_bar_b==bar_b))
//		 {
//		   
//		   old_bar_b=bar_b;
//		   if((*(ii+8)==0)||(*(ii+8)==0x0d))
//		   bar_b=(*(ii+7)-'0');
//		   else if((*(ii+9)==0)||(*(ii+9)==0x0d))
//		   bar_b=(*(ii+7)-'0')*10+(*(ii+8)-'0');
//		   ack_rgb_bar(3);
//		 }
//		 ini_rec_buff(); //每次判断完后，清扫串口缓冲区 和接收完成标志
//		 uart_rec_new=0;
//		}
//		uptemp();		 //看是否需要更新温度信息
//	   }
}

