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
		recd_buf[RX1_Cnt] = SBUF;		//����һ���ֽ�
		if(RX1_Cnt<62)	/////////////////////��ֹ64�ֽڵĻ��������
		RX1_Cnt++;
		else			///////ÿ���յ���ָ���64�ֽڣ��Ͱ�������գ�����ָ��ָ��ͷ
		RX1_Cnt=0; 
	}
	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//�������æ��־
	}
}