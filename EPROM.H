#ifndef __EPROM_H__
#define __EPROM_H__

typedef unsigned char BYTE;
typedef unsigned int WORD;
void IapIdle();
BYTE IapReadBYTE(WORD addr);
#define MAIN_Fosc		22118400L	//������ʱ��
#define EEPROM_SSID_LC     0  		//����SSID ����FLASH�ϵ����λ��
#define EEPROM_PASSWORD_LC 64		//�������� ����FLASH�ϵ����λ��
#define EEPROM_LAB         128		//�����Ƿ��ѽ�·�����˻������뱣�浽FLASH��
#define CMD_IDLE      0
#define CMD_READ      1
#define CMD_PROGRAM   2
#define CMD_ERASE     3
#define ENABLE_IAP  0X81
#define IAP_ADDRESS 0X1200	//���ÿͻ����������λ�ã�����ѡ��FLASH�����һ��������
void Delay(BYTE n);
void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr,BYTE dat);
void IapEraseSector(WORD addr);

#endif
										  /////////////////////////////////////////////////////////////////////���²�����STC�ĵ���洢���򣬶�д�ڲ���EEPROM ���������˻�������////////////////////////

