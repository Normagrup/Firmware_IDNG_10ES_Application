/**
  ******************************************************************************
  * @file    IO_Toggle/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "dali.h"
#include "board.h"
#include "rtcc.h"
#include "config.h"
#include "ethTransceiver.h"
#include "ethFramming.h"
#include "wdt.h"
#include "daliLoop.h"
#include "fallos.h"
#include "systick.h"
#include "storage.h"
#include "historico.h"
#include "test.h"
#include "temp.h"
#include "daliAddressing.h"
#include "polling.h"
#include "daliUtils.h"
//#include "webserver.h"
//#include "botonera.h"

//#define NO_BOOTLOADER
//#define DESACTIVAR_IED2

// no boot

// 0x8000000 0x10000
// 0x20000000 0x2000

// boot 

// 0x8001000 0xF000
// 0x200000C0 0x1F40

#ifndef NO_BOOTLOADER

	#define APPLICATION_ADDRESS     (uint32_t)0x08001000


	#if   (defined ( __CC_ARM ))
		__IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
	#elif (defined (__ICCARM__))
	#pragma location = 0x20000000
		__no_init __IO uint32_t VectorTable[48];
	#elif defined   (  __GNUC__  )
		__IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
	#elif defined ( __TASKING__ )
		__IO uint32_t VectorTable[48] __at(0x20000000);
	#endif
	
#endif
	


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
	
int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
  */	
	
	#ifndef NO_BOOTLOADER
		uint32_t i = 0;
		for(i = 0; i < 48; i++)
		{
			VectorTable[i] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
		}

		/* Enable the SYSCFG peripheral clock*/
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
		/* Remap SRAM at 0x00000000 */
		SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);	
	#endif
	
	
	wdtInit();
	boardInit();		
	sysTickDelay(100);		
	boardInit();		
	configInit();
	rtccInit();
	embeddedBootloaderInit();
	macInit();
	ethTransceiverInit();		
	tempInit();
	sensoresInit();	
	idgies_esquemaGpioInit();
	interfazInit ();
	idngPowerOn();
	//configResetToFactorySettings();
	
  while (1)
  {
		wdtDoTask();
		ethDoTask();	
		ethTransceiverDoTask();
		boardDoTask();
		botoneraDoTask();
		ethICMPDoTask();	
		tempDoTask();
		sensoresDoTask ();
		interfazDoTask ();
		idngIesDoTask ();
		blindDoTask();
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
