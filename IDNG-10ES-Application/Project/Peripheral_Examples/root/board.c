// En este archivo se realiza la inicialización de PCB incluyendo E/S.
// En principio vamos a HARDCODEAR la configuración de pines. 
// Habrá una función para configurar dirección del GPIO.
// Habrá una función para setear pin.
// Habrá una función para resetear pin.
// Llama a una función cuando detecta un flanco ascendente en pin configurado como entrada.
// Llama a una función cuando detecta un flanco descendente en pin configurado como entrada.


#include "board.h"

#define BOARD_PERIODO_SHORT													0xFF
#define BOARD_PERIODO_SHORT_REINTENTO								0xFFFF

unsigned char gpio_dir[11];
unsigned char gpio_value[11];
unsigned char gpio_short[11];
GPIO_TypeDef* gpio_gpio_out[11];
uint32_t 			gpio_gpio_out_pin[11];
GPIO_TypeDef* gpio_gpio_in[11];
uint32_t 			gpio_gpio_in_pin[11];
uint32_t			gpio_shortCnt[11]={BOARD_PERIODO_SHORT,BOARD_PERIODO_SHORT,BOARD_PERIODO_SHORT,BOARD_PERIODO_SHORT,BOARD_PERIODO_SHORT,BOARD_PERIODO_SHORT,BOARD_PERIODO_SHORT,BOARD_PERIODO_SHORT,BOARD_PERIODO_SHORT,BOARD_PERIODO_SHORT,BOARD_PERIODO_SHORT};


unsigned short int	gpio_pressed[11];




void boardGPIODirSet (unsigned char gpio, unsigned char dir)
{
	gpio_dir[gpio]=dir;	
	if (gpio_dir[gpio]==0x01)
	{	
		gpio_value[gpio]=0x00;
	}
	else
	{
		gpio_value[gpio]=0xFF;	
	}
}

void boardGPIOPinValueSet (unsigned char gpio, unsigned char value)
{
	if ((gpio_dir[gpio]==0x01) && (gpio_value[gpio]!=value)) 
	{
		if (gpio_short[gpio]==0x00)		gpio_shortCnt[gpio]=BOARD_PERIODO_SHORT;		
		gpio_value[gpio]=value;
	}
}

unsigned char boardGPIOPinValueGet (unsigned char gpio)
{
	return gpio_value[gpio];
	
}



void boardTick (void)
{
	unsigned char n;
	for (n=0;n<11;n++)
	{
		if ((gpio_pressed[n]<0xFFFE) && (gpio_pressed[n]>0))
		{		
			gpio_pressed[n]--;
		}
	}
}


void boardDoTask (void)
{
	unsigned char n;	
	for (n=0;n<11;n++)
	{
		if (gpio_dir[n]==0x01)				// Salida.
		{
			if (gpio_value[n]==0x01)		// Salida activada.
			{			
				if (gpio_short[n]==0x01)
				{		
					if (gpio_shortCnt[n]<BOARD_PERIODO_SHORT_REINTENTO)
					{
						gpio_shortCnt[n]++;				
					}
					else
					{
						gpio_shortCnt[n]=BOARD_PERIODO_SHORT;
						gpio_short[n]=0;
					}			
					GPIO_ResetBits(gpio_gpio_out[n], gpio_gpio_out_pin[n]);					
				}
				else
				{					
					if (GPIO_ReadInputDataBit(gpio_gpio_in[n], gpio_gpio_in_pin[n])==0x01)
					{
						if ((gpio_shortCnt[n])>0)
						{
							gpio_shortCnt[n]--;
						}
						else
						{
							gpio_short[n]=1;
						}			
					}		
					else							
					{
						if ((gpio_shortCnt[n])<BOARD_PERIODO_SHORT)
						{
							gpio_shortCnt[n]++;
						}
					}				
					GPIO_SetBits(gpio_gpio_out[n], gpio_gpio_out_pin[n]);
				}
			}
			else
			{
				GPIO_ResetBits(gpio_gpio_out[n], gpio_gpio_out_pin[n]);
			}
		}
		else																				// Entrada.
		{					
			
			if ((gpio_value[n]==0x01) && (GPIO_ReadInputDataBit(gpio_gpio_in[n], gpio_gpio_in_pin[n])==0x00))
			{			
				// Flanco descendente.				
				gpio_value[n]=0x00;								
				interfacesNotificar (n, 0x08, 0x00);			// PRESENCY				
			}			
			
			
			
			if (GPIO_ReadInputDataBit(gpio_gpio_in[n], gpio_gpio_in_pin[n])==0x00)
			{
				if (gpio_pressed[n]==0xFFFF)	gpio_pressed[n]=200;				
				if (gpio_pressed[n]==0)
				{				
					interfacesNotificar (n, 0x02, 0x00);		// LONG_PRESS?					
					gpio_pressed[n]=0xFFFE;
				}				
			}
			
			
			
			
		
			if ((gpio_value[n]==0x00) && (GPIO_ReadInputDataBit(gpio_gpio_in[n], gpio_gpio_in_pin[n])==0x01))
			{			
				// Flanco ascendente.			
				if (gpio_pressed[n]==0xFFFE)	
				{
					interfacesNotificar (n, 0x04, 0x00);		// LONG_RELEASE					
				}				
				else
				{
					interfacesNotificar (n, 0x01, 0x00);		// SHORT_PRESS
				}
				
				
				
				gpio_pressed[n]=0xFFFF;				
				gpio_value[n]=0x01;
				interfacesNotificar (n, 0x10, 0x00);		// VACANCY				
			}						
			
			
			
			
			
			if (gpio_value[n]==0xFF)  gpio_value[n]=GPIO_ReadInputDataBit(gpio_gpio_in[n], gpio_gpio_in_pin[n]);				
			GPIO_ResetBits(gpio_gpio_out[n], gpio_gpio_out_pin[n]);
		
		
		}				
		gpio_short[10]=0x00;		
	}			
}

void boardInit (void)
{	
	/* W5100 Y M25P*/	
	SPI_InitTypeDef 		spi2con;
	GPIO_InitTypeDef 		spi2;
	GPIO_InitTypeDef 		en_w5100;
	GPIO_InitTypeDef 		rst_w5100;
	GPIO_InitTypeDef 		nss_w5100;
	GPIO_InitTypeDef 		nss_m25p;		
	/* W5100 Y M25P*/	
		
	/* MAC */
	I2C_InitTypeDef 		I2C_InitStructure;	
	GPIO_InitTypeDef 		i2c;
	/* MAC */
	
	
		
	/* GPIO */	
	GPIO_InitTypeDef out[4];
	GPIO_InitTypeDef in[4];	
	// OUTPUTS
		// O01:PA12, O02:PA10, O03:PA08, O04:PC08, O05:PB00, O06:PC04, O07:PA06, O08:PA04, O09:PF04, O10:PA02, O11:PC03
		// GPIOA:	12,10,8,6,4,2
		// GPIOB: 0
		// GPIOC: 3,4,8
		// GPIOD:
		// GPIOE:
		// GPIOF: 04
	// INPUTS
		// I01:PA11, IO2:PA09, I03:PC09, I04:PB01, I05:PC05, I06:PA07, I07:PA05, I08:PF05, I09:PA03, I10:PA01
		// GPIOA:	11,9,7,5,3,1
		// GPIOB:	1
		// GPIOC:	9,5
		// GPIOD:	
		// GPIOE:	
		// GPIOF:	5	
	RCC_DeInit();		
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
	RCC_PLLCmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);	
	out[0].GPIO_Mode = GPIO_Mode_OUT;
	out[0].GPIO_Speed = GPIO_Speed_Level_3;
	out[0].GPIO_OType = GPIO_OType_PP;
	out[0].GPIO_PuPd = GPIO_PuPd_NOPULL;
	out[0].GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12; 
	GPIO_Init(GPIOA, &out[0]);	
	out[1].GPIO_Mode = GPIO_Mode_OUT;
	out[1].GPIO_Speed = GPIO_Speed_Level_3;
	out[1].GPIO_OType = GPIO_OType_PP;
	out[1].GPIO_PuPd = GPIO_PuPd_NOPULL;
	out[1].GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOB, &out[1]);
	out[2].GPIO_Mode = GPIO_Mode_OUT;
	out[2].GPIO_Speed = GPIO_Speed_Level_3;
	out[2].GPIO_OType = GPIO_OType_PP;
	out[2].GPIO_PuPd = GPIO_PuPd_NOPULL;
	out[2].GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_8; 
	GPIO_Init(GPIOC, &out[2]);
	out[3].GPIO_Mode = GPIO_Mode_OUT;
	out[3].GPIO_Speed = GPIO_Speed_Level_3;
	out[3].GPIO_OType = GPIO_OType_PP;
	out[3].GPIO_PuPd = GPIO_PuPd_NOPULL;
	out[3].GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOF, &out[3]);
	// >> STARTUP VALUES.
		GPIO_ResetBits(GPIOA, out[0].GPIO_Pin);
		GPIO_ResetBits(GPIOB, out[1].GPIO_Pin);
		GPIO_ResetBits(GPIOC, out[2].GPIO_Pin);
		GPIO_ResetBits(GPIOF, out[3].GPIO_Pin);
	// << STARTUP VALUES.	
		// GPIOA:	11,9,7,5,3,1
		// GPIOB:	1
		// GPIOC:	9,5
		// GPIOD:	
		// GPIOE:	
		// GPIOF:	5
		in[0].GPIO_Mode = GPIO_Mode_IN;
		in[0].GPIO_PuPd = GPIO_PuPd_NOPULL;
		in[0].GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_11;
		GPIO_Init(GPIOA, &in[0]);	
		in[1].GPIO_Mode = GPIO_Mode_IN;
		in[1].GPIO_PuPd = GPIO_PuPd_NOPULL;
		in[1].GPIO_Pin = GPIO_Pin_1;
		GPIO_Init(GPIOB, &in[1]);		
		in[2].GPIO_Mode = GPIO_Mode_IN;
		in[2].GPIO_PuPd = GPIO_PuPd_NOPULL;
		in[2].GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_9;
		GPIO_Init(GPIOC, &in[2]);		
		in[3].GPIO_Mode = GPIO_Mode_IN;
		in[3].GPIO_PuPd = GPIO_PuPd_NOPULL;
		in[3].GPIO_Pin = GPIO_Pin_5;
		GPIO_Init(GPIOF, &in[3]);		
		gpio_dir[9]=0;
		gpio_value[9]=0xFF;
		gpio_gpio_in[9]=GPIOA;
		gpio_gpio_in_pin[9]=GPIO_Pin_11;
		gpio_gpio_out[9]=GPIOA;
		gpio_gpio_out_pin[9]=GPIO_Pin_12;		
		gpio_dir[8]=0;
		gpio_value[8]=0xFF;
		gpio_gpio_in[8]=GPIOA;
		gpio_gpio_in_pin[8]=GPIO_Pin_9;
		gpio_gpio_out[8]=GPIOA;
		gpio_gpio_out_pin[8]=GPIO_Pin_10;		
		gpio_dir[7]=0;
		gpio_value[7]=0xFF;
		gpio_gpio_in[7]=GPIOC;
		gpio_gpio_in_pin[7]=GPIO_Pin_9;
		gpio_gpio_out[7]=GPIOA;
		gpio_gpio_out_pin[7]=GPIO_Pin_8;		
		gpio_dir[6]=0;
		gpio_value[6]=0xFF;
		gpio_gpio_in[6]=GPIOB;
		gpio_gpio_in_pin[6]=GPIO_Pin_1;
		gpio_gpio_out[6]=GPIOC;
		gpio_gpio_out_pin[6]=GPIO_Pin_8;		
		gpio_dir[5]=0;
		gpio_value[5]=0xFF;
		gpio_gpio_in[5]=GPIOC;
		gpio_gpio_in_pin[5]=GPIO_Pin_5;
		gpio_gpio_out[5]=GPIOB;
		gpio_gpio_out_pin[5]=GPIO_Pin_0;		
		gpio_dir[4]=0;
		gpio_value[4]=0xFF;
		gpio_gpio_in[4]=GPIOA;
		gpio_gpio_in_pin[4]=GPIO_Pin_7;
		gpio_gpio_out[4]=GPIOC;
		gpio_gpio_out_pin[4]=GPIO_Pin_4;		
		gpio_dir[3]=0;
		gpio_value[3]=0xFF;
		gpio_gpio_in[3]=GPIOA;
		gpio_gpio_in_pin[3]=GPIO_Pin_5;
		gpio_gpio_out[3]=GPIOA;
		gpio_gpio_out_pin[3]=GPIO_Pin_6;		
		gpio_dir[2]=0;
		gpio_value[2]=0xFF;
		gpio_gpio_in[2]=GPIOF;
		gpio_gpio_in_pin[2]=GPIO_Pin_5;
		gpio_gpio_out[2]=GPIOA;
		gpio_gpio_out_pin[2]=GPIO_Pin_4;		
		gpio_dir[1]=0;
		gpio_value[1]=0xFF;
		gpio_gpio_in[1]=GPIOA;
		gpio_gpio_in_pin[1]=GPIO_Pin_3;
		gpio_gpio_out[1]=GPIOF;
		gpio_gpio_out_pin[1]=GPIO_Pin_4;		
		gpio_dir[0]=0;
		gpio_value[0]=0xFF;
		gpio_gpio_in[0]=GPIOA;
		gpio_gpio_in_pin[0]=GPIO_Pin_1;
		gpio_gpio_out[0]=GPIOA;
		gpio_gpio_out_pin[0]=GPIO_Pin_2;		
		gpio_dir[10]=1;
		gpio_value[10]=0x00;
		gpio_gpio_in[10]=GPIOC;
		gpio_gpio_in_pin[10]=GPIO_Pin_3;
		gpio_gpio_out[10]=GPIOC;
		gpio_gpio_out_pin[10]=GPIO_Pin_3;	
	/* GPIO */

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);
		//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);				// Previamente habilitado.
		spi2.GPIO_Mode = GPIO_Mode_AF;
		spi2.GPIO_Speed = GPIO_Speed_Level_3;
		spi2.GPIO_OType = GPIO_OType_PP;
		spi2.GPIO_PuPd = GPIO_PuPd_NOPULL;
		spi2.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB,&spi2);			
	
		nss_w5100.GPIO_Mode = GPIO_Mode_OUT;
		nss_w5100.GPIO_Speed = GPIO_Speed_Level_3;
		nss_w5100.GPIO_OType = GPIO_OType_PP;
		nss_w5100.GPIO_PuPd = GPIO_PuPd_NOPULL;
		nss_w5100.GPIO_Pin = GPIO_Pin_12;
		GPIO_Init(GPIOB,&nss_w5100);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);	
	
		nss_m25p.GPIO_Mode = GPIO_Mode_OUT;
		nss_m25p.GPIO_Speed = GPIO_Speed_Level_3;
		nss_m25p.GPIO_OType = GPIO_OType_PP;
		nss_m25p.GPIO_PuPd = GPIO_PuPd_NOPULL;
		nss_m25p.GPIO_Pin = GPIO_Pin_11;
		GPIO_Init(GPIOB,&nss_m25p);	
		GPIO_SetBits(GPIOB, GPIO_Pin_11);	
		
		SPI_I2S_DeInit(SPI2);
		spi2con.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // Initially Tx
		spi2con.SPI_Mode	= SPI_Mode_Master;
		spi2con.SPI_DataSize	= SPI_DataSize_8b;
		spi2con.SPI_CPOL	= SPI_CPOL_Low ;
		spi2con.SPI_CPHA	= SPI_CPHA_1Edge;
		spi2con.SPI_NSS	= SPI_NSS_Soft ;		
		spi2con.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_2;		
		spi2con.SPI_FirstBit	= SPI_FirstBit_MSB;
		spi2con.SPI_CRCPolynomial	= SPI_CRCLength_8b;
		SPI_Init(SPI2, &spi2con);
		SPI_RxFIFOThresholdConfig (SPI2, SPI_RxFIFOThreshold_QF);
		SPI_Cmd(SPI2, ENABLE); 			

		rst_w5100.GPIO_Pin = GPIO_Pin_7;
		rst_w5100.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_Init(GPIOC, &rst_w5100);	
		GPIO_ResetBits(GPIOC, GPIO_Pin_7); 	
	
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	
		//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);		
		RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);	
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource6, GPIO_AF_1);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource7, GPIO_AF_1);		
	
		i2c.GPIO_Pin = GPIO_Pin_6;
		i2c.GPIO_Mode = GPIO_Mode_AF;
		i2c.GPIO_OType = GPIO_OType_OD;
		i2c.GPIO_PuPd = GPIO_PuPd_NOPULL;	
		GPIO_Init(GPIOB, &i2c);		
	
		i2c.GPIO_Pin = GPIO_Pin_7;
		i2c.GPIO_Mode = GPIO_Mode_AF;
		i2c.GPIO_OType = GPIO_OType_OD;
		i2c.GPIO_PuPd = GPIO_PuPd_NOPULL;	
		GPIO_Init(GPIOB, &i2c);			
		I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
		I2C_InitStructure.I2C_DigitalFilter = 0x00;
		I2C_InitStructure.I2C_OwnAddress1 = 0x00; 
		I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStructure.I2C_Timing = 0xA0120227;
		I2C_Init(I2C1, &I2C_InitStructure);
		I2C_Cmd(I2C1, ENABLE);			
			
		if (SysTick_Config(SystemCoreClock / 1000))
		{ 
			while (1);
		}	
}
