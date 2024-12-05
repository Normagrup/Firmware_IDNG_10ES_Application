#include "stm32f0xx.h"
#include "embeddedBootloader.h"
#include "m25p.h"

unsigned int _embeddedBootloaderCurrentPageCnt;

unsigned char _embeddedBootloaderWriteStart;
unsigned char _embeddedBootloaderWrite;
unsigned char _embeddedBootloaderReadStart;
unsigned char _embeddedBootloaderRead;


unsigned char _eBootloaderTransceiver_Pending;
unsigned int _eBootloaderTransceiver_CurrentPageCnt;
unsigned char _eBootloaderTransceiver_WriteStart;
unsigned char _eBootloaderTransceiver_Write;
unsigned char _eBootloaderTransceiver_ReadStart;
unsigned char _eBootloaderTransceiver_Read;







void embeddedBootloaderWrite (unsigned char *buffer)
{
		unsigned char status;			
		m25pWriteEnable();
		m25pWriteBytes(0x05,_embeddedBootloaderCurrentPageCnt,0,&buffer[0],256);	
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);	
		if (_embeddedBootloaderCurrentPageCnt<256)
		{
			_embeddedBootloaderCurrentPageCnt++;
		}
		_embeddedBootloaderWrite=0x01;
}

void embeddedBootloaderWriteStart (void)
{
		unsigned char status;		
		m25pWriteEnable();
		m25pSectorErase (5,0,0);
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);		
		_embeddedBootloaderCurrentPageCnt=0;
		
		_embeddedBootloaderWriteStart=0x01;
		_embeddedBootloaderWrite=0x00;
		_embeddedBootloaderReadStart=0x00;
		_embeddedBootloaderRead=0x00;
}

void embeddedBootloaderReadStart (void)
{
	_embeddedBootloaderCurrentPageCnt=0;
	_embeddedBootloaderReadStart=0x01;
}

void embeddedBootloaderRead (unsigned char *buffer)
{
	m25pReadBytes(0x05,_embeddedBootloaderCurrentPageCnt,0,&buffer[0],256);		
	if (_embeddedBootloaderCurrentPageCnt<256)
	{
		_embeddedBootloaderCurrentPageCnt++;
	}
	
	_embeddedBootloaderRead=0x01;
}

unsigned char embeddedBootloaderGo (unsigned char modelH, unsigned char modelL)
{
	unsigned char answer;
	
	if (((modelH==0x00) &&	(modelL==0x04)) && ((_embeddedBootloaderWriteStart==0x01) && (_embeddedBootloaderWrite==0x01) && (_embeddedBootloaderReadStart==0x01) && (_embeddedBootloaderRead==0x01)))
	{
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xAAAAAAAA);	
		answer=1;
	}
	else
	{
		answer=0;
	}
	return answer;
}


void embeddedBootloaderInit (void)
{
	if (RTC_ReadBackupRegister(RTC_BKP_DR0)==0xAAAAAAAA)
	{
		RTC_WriteBackupRegister(RTC_BKP_DR0,0);		
	}		
}





void eBootloaderTransceiver_Write (unsigned char *buffer)
{
}

void eBootloaderTransceiver_WriteStart (void)
{	
}

void eBootloaderTransceiver_Read (unsigned char *buffer)
{
}

void eBootloaderTransceiver_ReadStart (unsigned char *buffer)
{
}




void eBootloaderTransceiver_Check (void)
{
}







unsigned char eBootloaderTransceiver_Go (void)
{
}











