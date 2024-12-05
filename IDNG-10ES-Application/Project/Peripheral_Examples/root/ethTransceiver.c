#include "stm32f0xx.h"
#include "config.h"
#include "ethFramming.h"
#include "ethTransceiver.h"
#include "systick.h"
#include "daliUtils.h"
#include "w5100/w5100.h"
#include "iED.h"

unsigned char macAdrress[6];

volatile unsigned int ethTransceiverInicializadoCnt;
volatile unsigned char ethTransceiverEstado;
volatile unsigned char ethTransceiverTiempoSinRecibirCnt;
volatile unsigned char ethTransceiverIntentosFallidosCnt;
volatile unsigned char sendKeepAliveYa;
volatile unsigned char ethTransceiverKeepAliveWhenConnectedCnt;

extern unsigned int portDest;

extern unsigned char _configIpAddress[4];

unsigned char _sendI1Abierto;
unsigned char _sendI1Cerrado;
unsigned char _sendI2Abierto;
unsigned char _sendI2Cerrado;

unsigned char _broadcastAddress[4];

unsigned int InputNotificationTimer=5;
extern unsigned char gpio_dir[11];
extern unsigned char gpio_value[11];
extern GPIO_TypeDef* gpio_gpio_in[11];
extern uint32_t 			gpio_gpio_in_pin[11];


void ethInputsNotifications_Tick (void)
{
	unsigned char ip[4];
	unsigned char  InputNotification[32];
	unsigned char frame[19]={0};
	unsigned int n=0;

	configInputNotificationsGet(&InputNotification[0]);
	
	if (InputNotification[0]==0x01)
	{
	
	//Generación del Mensaje de estados de I/O
	

		frame[0]=0x05;
		frame[1]=0xFF;
		frame[2]=0xF0;
		frame[3]=0xF0;
		frame[4]=0xF0;
		frame[5]=0xF0;
		frame[6]=0x00;
		frame[7]=0x0B;
		
		if ((ethTransceiverEstado==0x05) && (InputNotificationTimer<=0)){
			for(n=0;n<10;n++){
				if(gpio_value[n]==0xFF){ //Sin configurar
					frame[n+8]=0xFF;
				}	
				else if (gpio_dir[n]==0x00){ //Entradas
					if(GPIO_ReadInputDataBit(gpio_gpio_in[n], gpio_gpio_in_pin[n])){ //Lectura
						frame[n+8]=0x00;
					}
					else{
						frame[n+8]=0x01;
					}
				}
				else{
					if(gpio_value[n-8]==0x00){ //SALIDA
						frame[n+8]=0x02; //DESACTIVADA
					}
					else{
						frame[n+8]=0x03;	//ACTIVADA
					}
				}
			}

		ethSendFrame(0,&frame[0],19,&_broadcastAddress[0], portDest);
		InputNotificationTimer=5;	
		}
	
	else{
		InputNotificationTimer--;	
	}
	
	}
}


void sendI1Abierto (void)
{
	_sendI1Abierto=0x01;
}

void sendI1Cerrado (void)
{
	_sendI1Cerrado=0x01;
}

void sendI2Abierto (void)
{
	_sendI2Abierto=0x01;
}

void sendI2Cerrado (void)
{
	_sendI2Cerrado=0x01;
}


void ethTransceiverReset (void)
{
	ethTransceiverIntentosFallidosCnt=0;
	ethTransceiverEstado=0x00;
}

void ethTransceiverLAN_test (void)
{
	ethTransceiverIntentosFallidosCnt=0;
	ethTransceiverEstado=0x03;
}


void sendKeepAlive (void)
{
	sendKeepAliveYa=0x01;
}


void ethTransceiverTiempoSinRecibirCntRst (void)
{
	ethTransceiverTiempoSinRecibirCnt=0x00;	
}



void ethTransceiverRTCC_Tick (void)
{
		if (ethTransceiverEstado==0x05)
		{
			sendKeepAlive();			
			if (ethTransceiverTiempoSinRecibirCnt<2)
			{
				ethTransceiverTiempoSinRecibirCnt++;
			}			
			else
			{
				ethTransceiverTiempoSinRecibirCnt=0x00;
				ethTransceiverLAN_test();	
			}
		}			
}

unsigned char ethTransceiverConTestActiveGet (void)
{
	unsigned char gatewayAddress[4];
	configGatewayAddressGet(&gatewayAddress[0]);	
	if ((gatewayAddress[0]==0x00) && (gatewayAddress[1]==0x00) && (gatewayAddress[2]==0x00) && (gatewayAddress[3]==0x00))
	{
		return 0x00;		
	}
	else
	{
		return 0x01;
	}	
}

	
void ethTransceiverTick (void)
{
	if (ethTransceiverInicializadoCnt>0)
	{
		ethTransceiverInicializadoCnt--;
	}
	
	if (ethTransceiverKeepAliveWhenConnectedCnt>0)
	{
		ethTransceiverKeepAliveWhenConnectedCnt--;
	}
	
	
}


void macInit (void)
{
	
	unsigned char macRead[6];
	// 0xA0 means WRITE - 0xFA is the ADDRESS
	I2C_TransferHandling(I2C1,0xA0, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);		
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);
	I2C_SendData(I2C1,0xFA);				
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TC) == RESET);
	// 0xA1 means READ - 6 times.
	I2C_TransferHandling(I2C1,0xA1, 6, I2C_SoftEnd_Mode,I2C_Generate_Start_Read);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);
	macRead[0] = I2C_ReceiveData(I2C1);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);
	macRead[1] = I2C_ReceiveData(I2C1);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);
	macRead[2] = I2C_ReceiveData(I2C1);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);		
	macRead[3] = I2C_ReceiveData(I2C1);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);
	macRead[4] = I2C_ReceiveData(I2C1);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);
	macRead[5] = I2C_ReceiveData(I2C1);			
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TC) == RESET);
	I2C_GenerateSTOP(I2C1,ENABLE);
	macAdrress[0]=macRead[0];
	macAdrress[1]=macRead[1];
	macAdrress[2]=macRead[2];
	macAdrress[3]=macRead[3];
	macAdrress[4]=macRead[4];
	macAdrress[5]=macRead[5];		

	

	
}













void ethTransceiverDoTask (void)
{
	unsigned char gatewayAddress[4];
	configGatewayAddressGet(&gatewayAddress[0]);
	switch (ethTransceiverEstado)
	{
		case 0x00:
		{		
			GPIO_ResetBits(GPIOC, GPIO_Pin_7); 				//Reset ON
			ethTransceiverInicializadoCnt=100;
			ethTransceiverEstado=0x01;
			break;
		}
		
		case 0x01:
		{
			if (ethTransceiverInicializadoCnt==0)
			{
				ethTransceiverInicializadoCnt=100;
				ethTransceiverEstado=0x02;
			}
			break;
		}

		case 0x02:
		{
			if (ethTransceiverInicializadoCnt==0)
			{
				ethTransceiverW5100Init();							//Init transceiver
				ethTransceiverEstado=0x03;			
				ethTransceiverInicializadoCnt=200;				
			}
			break;
		}	

		case 0x03:
		{
			if (ethTransceiverConTestActiveGet()==0x01)
			{		
				if (ethTransceiverInicializadoCnt==0)
				{
					ethICMPPingSend(&gatewayAddress[0]);
					ethTransceiverEstado=0x04;	
				}				
			}
			else
			{
				ethTransceiverEstado=0x05;
				sendKeepAliveYa=1;
				ethTransceiverKeepAliveWhenConnectedCnt=10;		
			}
			break;
		}		
		
		case 0x04:
		{
			if (ethTransceiverConTestActiveGet()==0x01)
			{
				if ((ethICMPPongReceivedGet()==0x01) || (ethICMPPongTimeoutGet()==0x01))
				{
					if ((ethICMPPongReceivedGet()==0x01) || (ethTransceiverIntentosFallidosCnt>10))
					{
						ethTransceiverEstado=0x05;	
						sendKeepAliveYa=1;
						ethTransceiverKeepAliveWhenConnectedCnt=10;

					}				
					else
					{
						ethTransceiverIntentosFallidosCnt++;
						ethTransceiverEstado=0x00;	
						botoneraFailLedBlink();
						
					}					
				}			
			}
			else
			{
				ethTransceiverEstado=0x05;	
				sendKeepAliveYa=1;
				ethTransceiverKeepAliveWhenConnectedCnt=10;				
			}
			break;
		}		
		
		case 0x05:
		{
			webServerDoTask();			
			if ((sendKeepAliveYa==0x01)  && (ethTransceiverKeepAliveWhenConnectedCnt==0x00))
			{
					unsigned char keepAlive1[7];	
					unsigned char ip1[4]={255,255,255,255};	
					keepAlive1[0]=0xFF;
					keepAlive1[1]=macAdrress[0];
					keepAlive1[2]=macAdrress[1];
					keepAlive1[3]=macAdrress[2];
					keepAlive1[4]=macAdrress[3];
					keepAlive1[5]=macAdrress[4];
					keepAlive1[6]=macAdrress[5];
					ethSendFrame(0,&keepAlive1[0],7,&ip1[0], portDest);						
					sendKeepAliveYa=0x00;
			}			
				
			if (_sendI1Abierto==1)
			{
				unsigned char frame[7];	
				unsigned char ip[4];				
				ip[0]=_configIpAddress[0];
				ip[1]=_configIpAddress[1];
				ip[2]=_configIpAddress[2];
				ip[3]=0xFF;				
				frame[0]=0x01;
				frame[1]=0x00;
				frame[2]=0xFE;
				frame[3]=0xFE;
				frame[4]=IED_DEST_NORMALINK_I1;
				frame[5]=IED_CMDH_NORMALINK_I1_ABIERTO;
				frame[6]=IED_CMDL_NORMALINK_I1_ABIERTO;
				ethSendFrame(0,&frame[0],7,&ip[0], portDest);
				_sendI1Abierto=0;				
			}
			
			if (_sendI1Cerrado==1)
			{
				unsigned char frame[7];	
				unsigned char ip[4];				
				ip[0]=_configIpAddress[0];
				ip[1]=_configIpAddress[1];
				ip[2]=_configIpAddress[2];
				ip[3]=0xFF;
				frame[0]=0x01;
				frame[1]=0x00;
				frame[2]=0xFE;
				frame[3]=0xFE;
				frame[4]=IED_DEST_NORMALINK_I1;
				frame[5]=IED_CMDH_NORMALINK_I1_CERRADO;
				frame[6]=IED_CMDL_NORMALINK_I1_CERRADO;
				ethSendFrame(0,&frame[0],7,&ip[0], portDest);
				_sendI1Cerrado=0;				
			}
			
			if (_sendI2Abierto==1)
			{
				unsigned char frame[7];	
				unsigned char ip[4];				
				ip[0]=_configIpAddress[0];
				ip[1]=_configIpAddress[1];
				ip[2]=_configIpAddress[2];
				ip[3]=0xFF;
				frame[0]=0x01;
				frame[1]=0x00;
				frame[2]=0xFE;
				frame[3]=0xFE;
				frame[4]=IED_DEST_NORMALINK_I2;
				frame[5]=IED_CMDH_NORMALINK_I2_ABIERTO;
				frame[6]=IED_CMDL_NORMALINK_I2_ABIERTO;
				ethSendFrame(0,&frame[0],7,&ip[0], portDest);
				_sendI2Abierto=0;				
			}
			
			if (_sendI2Cerrado==1)
			{
				unsigned char frame[7];	
				unsigned char ip[4];				
				ip[0]=_configIpAddress[0];
				ip[1]=_configIpAddress[1];
				ip[2]=_configIpAddress[2];
				ip[3]=0xFF;
				frame[0]=0x01;
				frame[1]=0x00;
				frame[2]=0xFE;
				frame[3]=0xFE;
				frame[4]=IED_DEST_NORMALINK_I2;
				frame[5]=IED_CMDH_NORMALINK_I2_CERRADO;
				frame[6]=IED_CMDL_NORMALINK_I2_CERRADO;
				ethSendFrame(0,&frame[0],7,&ip[0], portDest);
				_sendI2Cerrado=0;				
			}
			break;
		}				
	}
}


























void ethTransceiverInit (void)
{
	ethTransceiverIntentosFallidosCnt=0;	
	ethTransceiverEstado=0x00;
}

void ethTransceiverW5100Init (void)
{
	unsigned char ipAddress[4];
	unsigned char submaskAddress[4];
	unsigned char gatewayAddress[4];
	configIpAddressGet(&ipAddress[0]);
	configSubmaskAddressGet(&submaskAddress[0]);
	configGatewayAddressGet(&gatewayAddress[0]);
	GPIO_SetBits(GPIOC, GPIO_Pin_7); 			//Reset Off	
	sysTickDelay(10);
	iinchip_init();	
	setGAR(&gatewayAddress[0]);
	setSHAR(&macAdrress[0]);
	setSUBR(&submaskAddress[0]);
	applySUBR();
	setSIPR(&ipAddress[0]);		
	sysinit(10,01);
	sysTickDelay(10);	
	socket(0,Sn_MR_UDP,2234,0);	
	sysTickDelay(10);		
	_broadcastAddress[0]=ipAddress[0] | (~submaskAddress[0]);
	_broadcastAddress[1]=ipAddress[1] |	(~submaskAddress[1]);
	_broadcastAddress[2]=ipAddress[2] |	(~submaskAddress[2]);
	_broadcastAddress[3]=ipAddress[3] |	(~submaskAddress[3]);
}












