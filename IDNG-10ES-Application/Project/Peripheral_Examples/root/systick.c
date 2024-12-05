#include "stm32f0xx.h"
#include "systick.h"
#include "wdt.h"
#include "daliUtils.h"
#include "ethTransceiver.h"
#include "historico.h"

static __IO uint32_t _sysTickTimingDelay;
unsigned char _sysTickBlindCnt;


void sysTickTick (void)
{
	if (_sysTickTimingDelay!= 0x00)
  { 
    _sysTickTimingDelay--;
  }
	
	ethTransceiverTick();
	ethICMPTick();

	historicoTick();
	botoneraTick();
	interfazTick ();
	sensoresSysTick ();
	boardTick();
	
	
	
	if (_sysTickBlindCnt<16)	// Llamada cada 100mS.
	{
		_sysTickBlindCnt++;
		
	}
	else
	{				
		blindTick();

		_sysTickBlindCnt=0;
	}
	
	
}






void sysTickDelay (__IO uint32_t nTime)
{
	_sysTickTimingDelay = nTime;
//  while(_sysTickTimingDelay != 0)
//	{
		//wdtDoTask();
	//}		
}
