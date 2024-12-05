#include "stm32f0xx.h"
#include "config.h"
#include "wdt.h"
#include "daliUtils.h"

#define INC_SENSORES_PRT
	#include "sensores.h"
#undef INC_SENSORES_PRT

unsigned char _enviando;

void botoneraLinkLedBlink (void)
{
}

void botoneraFailLedBlink(void)
{
}

void botoneraFailLedEncender(void)
{
}

void botoneraFailLedApagar(void)
{	
}

void botoneraTick (void)
{
}



void botoneraDoTask (void)
{	
	unsigned short int data[8]={0x07FF,0x07C0,0x000F,0x0000,0x0030,0x0000,0x0000,0x07FF};
	
	
	if (GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)==0x00)
	{
		while(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)==0x00);
		configResetNetworkSettings();
		
		

		setupIES_Set(&data[0]);
		
		wdtReboot();
	}	
}

