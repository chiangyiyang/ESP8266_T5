#ifndef __EPROM_H__
#define __EPROM_H__

typedef unsigned char BYTE;
typedef unsigned int WORD;
void IapIdle();
BYTE IapReadBYTE(WORD addr);
#define MAIN_Fosc		22118400L	//定义主时钟
#define EEPROM_SSID_LC     0  		//定义SSID 存在FLASH上的相对位置
#define EEPROM_PASSWORD_LC 64		//定义密码 存在FLASH上的相对位置
#define EEPROM_LAB         128		//定义是否已将路由器账户和密码保存到FLASH中
#define CMD_IDLE      0
#define CMD_READ      1
#define CMD_PROGRAM   2
#define CMD_ERASE     3
#define ENABLE_IAP  0X81
#define IAP_ADDRESS 0X1200	//设置客户参数保存的位置，这里选择FLASH的最后一个扇区。
void Delay(BYTE n);
void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr,BYTE dat);
void IapEraseSector(WORD addr);

#endif
										  /////////////////////////////////////////////////////////////////////以下部分是STC的掉电存储程序，读写内部的EEPROM 保存上网账户和密码////////////////////////


