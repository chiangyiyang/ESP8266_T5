#define __EPROM.C_								 
#include"EPROM.H"
#include "STC15Fxxxx.H"
void IapIdle()
{
  IAP_CONTR=0;
  IAP_CMD=0;
  IAP_TRIG=0;
  IAP_ADDRH=0X80;
  IAP_ADDRL=0;
}

BYTE IapReadByte(WORD addr)
{
   BYTE dat;
   IAP_CONTR=ENABLE_IAP;
   IAP_CMD=CMD_READ;
   IAP_ADDRL=addr;
   IAP_ADDRH=addr>>8;
   IAP_TRIG=0x5a;
   IAP_TRIG=0xa5;
   _nop_();
   dat=IAP_DATA;
   IapIdle();
   return dat;
}



void IapEraseSector(WORD addr)
{
  IAP_CONTR=ENABLE_IAP;
  IAP_CMD=CMD_ERASE;
  IAP_ADDRL=addr;
  IAP_ADDRH=addr>>8;
  IAP_TRIG=0x5a;
  IAP_TRIG=0xa5;
  _nop_();
  IapIdle();
}