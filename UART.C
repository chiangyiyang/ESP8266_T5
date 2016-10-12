#include"UART.H"
#include"STC15FXXXX.H"
void UART1_int (void) interrupt UART1_VECTOR
{
	if(RI)
	{
		RI = 0;
		uart_rec_date=1;
		uart_time_out=0;
		uart_rec_new=0;
		recd_buf[RX1_Cnt] = SBUF;		//保存一个字节
		if(RX1_Cnt<62)	/////////////////////防止64字节的缓冲区溢出
		RX1_Cnt++;
		else			///////每次收到的指令超过64字节，就把数据清空，接收指针指向开头
		RX1_Cnt=0; 
	}
	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//清除发送忙标志
	}
}