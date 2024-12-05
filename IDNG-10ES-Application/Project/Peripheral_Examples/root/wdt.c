#include "stm32f0xx.h"
#include "fallos.h"

unsigned char _wdtReboot;

void wdtReboot (void)
{
	_wdtReboot=0x01;
}

void wdtInit (void)
{	
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    // IWDGRST flag set //
		fallosFalloResetSet();		
  }
  else
  {
    // IWDGRST flag is not set //
  }	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_128);
	IWDG_SetReload(0xFFF);
	IWDG_ReloadCounter();
	IWDG_Enable();	
}

void wdtDoTask (void)
{
	if (_wdtReboot==0x00)
	{
		IWDG_ReloadCounter();	
	}
}