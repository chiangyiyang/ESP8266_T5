/*
�����Գ����ڰ��ſɿƼ�30Ԫ �Ĳ��԰�����ȷ���У����а��ſɳ��ĵ�ģ���ESP-01��ESP-14��������ȷ���У���Զ�̿��ơ�
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
��������:
void ini_rec_buff(void)
��������:
�ѽ��ջ������㣬���ô��ڽ��ջ���Ϊ��ʼ����������ָ��ָ��NULL
�����������õĺ����嵥:
��
���ñ������ĺ����嵥:
void main(void)
�������˵��:
��
���������˵��:
��
��������ֵ��˵��:
��
����˵��:
ÿ�δ����յ�һ�����ݣ�Ҫ�жϴ��ڻ������Ƿ������ע���ַ��������볹������ϴν��յ��Ļ��壬���������
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
��������:
void uptemp(void)
��������:
���¶Ⱥ�ʪ����Ϣ ����WIFIģ�鴮�ڣ����͵���������������ת���������նˣ������ֻ����ߵ���
�����������õĺ����嵥:
make_AT_SEND_LEN������//��̬���ɷ������ݳ���ATָ�
at_uart_send_str()	  //���������������¶���Ϣ
read_dh();			  //��ȡDHT11���¶Ⱥ�ʪ����Ϣ
���ñ������ĺ����嵥:
void main(void)
�������˵��:
��
���������˵��:
��
��������ֵ��˵��:
��
����˵��:
--------------------------------------------------------------------------------------------------------
*/
void uptemp(void)
{
            if(min_rech)                                     //	��ʱ����λmin_rechÿ���ӻ�ִ�е�����һ��
			{
			min_rech=0;
		    need_seed_len=9;                                 
		    make_AT_SEND_LEN(need_seed_len,RMT_data); 		 //��̬���췢��AT����ָ��
			at_uart_send_str(at_send_len_ox);                //���͹���õķ���ָ��
		    Delay2(1000);			
		    at_uart_send_buf(temp_cd,need_seed_len);	     //����WIFI��ʼ��������
			LED2=0;
			BEEP=1;
			Delay2(200);
		    LED2=1;
			BEEP=0;                                          //��������ָʾ�ƣ���������
	            							                 //ÿ30������е�����һ�Ρ�����һ���¶���ֵ
			read_dh();
			temp_cd[5]=(((RH_H%100)/10)+'0');
			temp_cd[6]=(((RH_H%10))+'0');
		    temp_cd[7]=((RH_L)+'0');
			}		  
}
/*------------------------------------------------------------------------------------------------------
��������:
void ack_rgb_bar(unsigned char bar_mod)
��������:
ģ�龭����������Ӧ�ն��豸��WIFIģ����Ƶĵƾߺ���������ֵ���ֻ����ߵ��ԣ��յ�Զ��ģ���Ӧ��������ֵ���Ÿ��½�����
�����������õĺ����嵥:
make_AT_SEND_LEN������//��̬���ɷ������ݳ���ATָ�
at_uart_send_str()	  //�������������ͱ��� R G B ����ֵ����0-99
���ñ������ĺ����嵥:
void main(void)
�������˵��:
��
���������˵��:
��
��������ֵ��˵��:
��
����˵��:
������������������״̬���ֻ�APP�ϣ������ֻ��϶���ɫ�ƽ�������85%��ģ�齫�յ�LIGHR:85,ģ�������ɫ�����ȵ�85%�󣬻���һ�� LIGHR:85��
�ֻ�APP֪��Զ�̵ƾ��Ѿ�ִ���ˣ����Ǹ��±��غ�ɫͨ����������85%
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
	make_AT_SEND_LEN(need_seed_len,RMT_data); 		 //��̬���췢��ATָ��
	at_uart_send_str(at_send_len_ox);                //���͹���õķ���ָ��
	Delay2(50);
	at_uart_send_buf(RGB_CODE,need_seed_len);	     //����WIFI��������  
} 
/*------------------------------------------------------------------------------------------------------
��������:
void main(void��
��������Ҫ��ɹ������£�
1 ��WIFI���磬���÷��������죬��ʼ������������Ĭ��ֵ��
2 ��ʼ������ �ж�  ��PWM �Լ���ʱ��
3 �ж��ϵ�ʱ���Ƿ�KEY1 �����£��������º������滷�ڣ�������SMARTLINK ״̬�����ñ�ģ�������ͥ·�������˻���������Ϣ
4 ���η�������ATָ���ģ�������Ʒ������ϣ��Ӷ��ֻ�����Զ�̿������ģ���ˡ�
  at_uart_send_str(AT_MODE);		 //����ģ�����STATION ģʽ
  at_uart_send_str(AT_SMARTLINK);	 //���ͽ���SMARTLINK ATָ�ģ���������ȡ��ͥ·�����˻����������������ϡ�
  at_uart_send_str(AT_CLDSTART);     //���ʰ��ſɵ��Ʒ��������밲�ſ��ơ�
  at_uart_send_str(AT_LANSTART);     //�������ģ�龭�����ؾ����������������ֻ��������Ĳ�ѯ��Ϣ���ֻ�������ģ�����Ϣ������ֻ��������������������ģ����ƣ�
5 ���Ž��д�ѭ���ˣ���������Ĳ��裬ģ���Ѿ��������Զ���ֻ����Ƶ�״̬�������ѭ���У�ֻ�ù�ע���ڹ��������ݣ�Զ���ֻ������������������Ŀ���ָ�
  ���� ���ؼ̵��������ú����� LED���ȵ��������ִ�к󣬻���Ҫ�����ֻ������Ѿ������������ִ���ˣ� ͬʱÿ���ӻ�Ҫ�ѱ����¶� ʪ�� ���������� ���͸�Զ���ֻ�APP����ʾ��
  ��������ѭ���У�������һ������KEY2 5���ģ����ƶ˷��������ͽ��ָ�����������ֻ�����󶨡�
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
	WIFI_VCC=0;		   //WIFI ��Դ�ܵ�Ƭ�����ƣ����쳣��ʱ�򣬵�Ƭ������ǿ��������ESP8266
	init_uart();	   // ��ʼ�����ڲ����ʺ�ESP8266 ����ģ����ͬ��
	BEEP=0;			   //�ص�������
	Delay2(1000);	   //�ӳ���������ϵͳ���ȶ�
	old_bar_r=bar_r;   //��ʼ�������� �Ƶ�PWM ռ�ձ�0-99
	old_bar_g=bar_g;
	old_bar_b=bar_b;
	PWM(5,5,5);		   //��������΢��
	timer0_ini();	   //��ʼ����ʱ0��ΪһЩ��Ҫʱ���׼����Ķ�����׼����


		
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
	
	
//	if(K1==0)           //������ʱ���ְ���1Ҳ����MCU_P1.3 �����£���ô������ATָ������WIFIģ����·�����������˻�������
//	{
//			LED2=0;
//			BEEP=1;
//			Delay2(200);
//		    LED2=1;
//			BEEP=0;      //��������ָʾ�ƣ���������
//			have_config=1;
//	}	 
//		at_uart_send_str(AT_MODE);		 //����ģ�����STATION ģʽ
//  	    Delay2(500);
//	    ini_rec_buff();	
//		if(have_config)					 //��Ҫ���� ģ�����뱾�ص�·�����˻�������
//		{ 
//		at_uart_send_str(AT_SMARTLINK);	 //���ͽ���SMARTLINK ATָ��
//		Delay2(500);
//		}
	
//		do								 //��ѭ�����SMARTLINK ������
//		{
//		BEEP=1;
//		Delay2(500);
//		BEEP=0;
//		Delay2(500);
//	    }while(strstr(recd_buf,smartlink_code)==NULL); //��ѭ�����SMARTLINK ������

//		uart_rec_new=0;
//		at_uart_send_str(AT_CLDSTART); //���밲�ſ���
//		Delay2(1000);
//		at_uart_send_str(AT_LANSTART);//��������ģʽ����ģʽ�£�ģ�����������Ӧ�ֻ�APP Ҫ��ѯ��ID ��KEY��
//		Delay2(500);
//		while(1)
//		{  
//		if(have_change_pwm)		     //��ʱ����λ��10�������һ�� ������ѭ���м�⵽��Ҫ�ı�R G B �Ƶı仯��ÿ10MS ��1���߼�1 PWM��ֵ�������������Ҫ2.56��
//		{							 //�����������������߽���Ч��
//		     have_change_pwm=0;		 //���־
//			 if(K1==0)				 //����K1�����£���¼���µ�ʱ�䳤�̡�
//		     key1_time++;
//		     else
//		     key1_time=0;
//		     if(key1_time>500)	     //K1 ���������³���5��
//		     {
//		      key1_time=0;
//		      at_uart_send_str(AT_CUB);	//����5���ģ�龭��ATָ��������������˳���ͥ������
//		      BEEP=1;
//		      Delay2(100);				//������������������ģ���Ѿ�������˺Ž��
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
//		  	 if(bar_r>old_bar_r)//bar_r ��������м�¼�����´�WIFIģ�鴮�ڻ�õ�PWM��ֵ�������ֺ;ɵ�PWM��ֵ�г��룬˵����Ҫ�ı䣡�ǾͰ��ϵļ�¼��1���߽�1
//			 {											   
//			   old_bar_r++;
//			   set_pwm(old_bar_r,old_bar_g,old_bar_b);//���ü�1���PWM��PWM�Ĵ����У��Ƶ����Ȼ�仯
//			 }
//			 else if(bar_r<old_bar_r)//bar_r ��������м�¼�����´�WIFIģ�鴮�ڻ�õ�PWM��ֵ�������ֺ;ɵ�PWM��ֵ�г��룬˵����Ҫ�ı䣡�ǾͰ��ϵļ�¼��1���߽�1
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
//		if(uart_rec_new)//�����־λΪ1������WIFI �յ����ݴӴ��������MCU�Ĵ��ڻ������ˣ����ҹ���һС�Σ����Է����ˡ���ʱ����ʱ�����յ��Ӵ������һ���ֽڿ�ʼ10MS����û�������ݵ���������һ���ν�����
//		{
//		 if((ii=strstr(recd_buf,PCLD_code))!=NULL) // strstr���������KEIL���������ַ����⺯���������Ǵ�һ���ַ����в����Ƿ����ָ�����ַ�����û�ҵ�����NULL���ҵ��󷵻��ҵ���λ�ã�����ָ�룡
//		 {										   // �ҵ�LIGHT: �������ַ��������ڴ��ڻ�������
//			if(*(ii+6)=='0')					   //  LIGHT:0 �ж����һ���ַ�����0����Ҫ�ؼ̵���
//			{
//              DK1=0;						//�ص��̵���
//			  need_seed_len=8;              //�����ϴ����ݵ��ƣ�ת���ֻ����¶����ݰ������ϻ������ݸ�ʽ
//			  make_AT_SEND_LEN(need_seed_len,RMT_data); 					 //��̬���췢��ATָ��
//			  at_uart_send_str(at_send_len_ox);                //���͹���õķ���ָ��
//			  Delay2(50);
//			  at_uart_send_buf(switch_sta0,need_seed_len);	     //����WIFI��������
//			  ///////////////////////////����ļ���AT���ͺ��������Ѿ��ص��̵�������Ϣͨ������������APP��APP�յ���ѽ����ϵĵ�����Ϊ��ɫ�ر�״̬
//			  Delay2(20);
//			  bar_r=0;
//			  bar_g=0;
//			  bar_b=0;	     //��APP�ص�״̬���ѱ��ص�R G B�����Ƶ�PWM��ռ�ձ� ȫ����Ϊ0
//			  ack_rgb_bar(1);
//			  ack_rgb_bar(1);
//			  ack_rgb_bar(2);
//			  Delay2(50);
//			  ack_rgb_bar(3);//���漸��ack_rgb_bar �������ѱ���RGB ��ռ�ձȾ����Ʒ������������ֻ�APP�����ֻ�APP�����϶���
//			  Delay2(50);
//			}
//			if(*(ii+6)=='1')				      //  LIGHT:1 �ж����һ���ַ�����0����Ҫ���̵���
//			{
//              DK1=1;
//			  need_seed_len=8;              //�����ϴ����ݵ��ƣ�ת���ֻ����¶����ݰ������ϻ������ݸ�ʽ
//			  make_AT_SEND_LEN(need_seed_len,RMT_data); 					 //��̬���췢��ATָ��
//			  at_uart_send_str(at_send_len_ox);                  //���͹���õķ���ָ��
//			  Delay2(20);
//			  at_uart_send_buf(switch_sta1,need_seed_len);	     //����WIFI�������� 
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
//		 else if(((ii=strstr(recd_buf,LIGHTR_code))!=NULL)&&(old_bar_r==bar_r))	//�ڴ��ڻ������У��ҵ�LIGHTR:XX �������ú�����PWM ���ַ�������
//		 {																		//���¾�PWM��ֵ��ͬ��Ҳ�����ϴβ���������PWM���������ɺ�old_bar_r==bar_r)����������
//		   old_bar_r=bar_r;														
//		   if((*(ii+8)==0)||(*(ii+8)==0x0d))
//		   bar_r=(*(ii+7)-'0');
//		   else if((*(ii+9)==0)||(*(ii+9)==0x0d))
//		   bar_r=(*(ii+7)-'0')*10+(*(ii+8)-'0');  //���������ȥ��LIGHTR;XX �а�XX����ַ���������֣�Ȼ����ܸ�PWM��ֵ
//		   ack_rgb_bar(1);						  //ģ���Ӧ��Ϣ��Զ���ֻ��ϵ�APP ���½�����
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
//		 ini_rec_buff(); //ÿ���ж������ɨ���ڻ����� �ͽ�����ɱ�־
//		 uart_rec_new=0;
//		}
//		uptemp();		 //���Ƿ���Ҫ�����¶���Ϣ
//	   }
}
