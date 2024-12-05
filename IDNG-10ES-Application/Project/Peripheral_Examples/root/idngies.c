#include "stm32f0xx.h"

#define INC_INTERFACES_CONFIGURACION_S
    #include "interfaces.h"
#undef INC_INTERFACES_CONFIGURACION_S

#define INC_INTERFACES_PRT
    #include "interfaces.h"
#undef INC_INTERFACES_PRT

#include "macros.h"

interfacesConfiguracion 							_interfazConfiguracionBackup[10];
//unsigned char 												gpioEsquemaDir[11];
extern unsigned char _comA[2];		
extern unsigned char _ipA[4];

unsigned short int	_idngiesCfg_uso = 0x07FF;								// 0b11111111111
//unsigned short int	_idngiesCfg_salida = 0x07C0;						// 0b11111000000
// De momento son todas ILU.
//unsigned short int	_idngiesCfg_ilu = 0x07C0;							// 0b11111000000
//unsigned short int	_idngiesCfg_res;
//unsigned short int	_idngiesCfg_entrada = 0x003F;						// 0b00000111111
unsigned short int	_idngiesCfg_pulShort = 0x000F;					// 0b00000001111
unsigned short int	_idngiesCfg_pulShortLong = 0x0000;			// 0b00000000000
unsigned short int	_idngiesCfg_int = 0x0030;								// 0b00000110000
unsigned short int	_idngiesCfg_es;
unsigned short int	_idngiesCfg_ilu;
unsigned short int	_idngiesCfg_per;

unsigned char _algunaEntradaEnUso=0x00;



unsigned char _serial[4];


extern unsigned char _blindPin[11];
extern unsigned char _blindUsed[5];

unsigned char arcLevel[11];
unsigned char powerOnLevel[11];
unsigned char sceneLevel[11][16];

unsigned char _dtr[11];

extern unsigned char daliLoopActualLevel[64];
extern unsigned char daliLoopActualLevelValid[64];

unsigned char storeSceneTimeout=0xFF;


extern unsigned char _configDaliLoopGroupLow[64];
extern unsigned char _configDaliLoopGroupHigh[64];

extern unsigned char _configIpAddress[4];



unsigned char _idngIesCfgPendingCnt=0xFF;



void idgies_esquemaGpioInit (void);
void idngPowerOn (void);


void setupIES_Set (unsigned short int* setupArray);
unsigned char idngies_buscarInterfaces(void);
void idngiesSalidasInit (void);

void configStoreSerialToFlash		(unsigned char *serial);
void configReadSerialFromFlash	(void);


/*
unsigned char idngies_esquemaGpioDirGet (unsigned char gpio)
{	
	return gpioEsquemaDir[gpio];
}
*/

/*
void idngIesStoreDTR (unsigned char dest, unsigned char scene)
{
	
}
*/

void idngIesRtcTick (void)
{
	if ((storeSceneTimeout!=0xFF) && (storeSceneTimeout>0))
	{
		storeSceneTimeout--;
	}	
	
	if ((_idngIesCfgPendingCnt!=0xFF) && (_idngIesCfgPendingCnt>0))
	{
		_idngIesCfgPendingCnt--;
	}	
}
void idngIesStoreScene (unsigned char output, unsigned char scene)
{
	sceneLevel[output][scene]=_dtr[output];
	
	storeSceneTimeout=20;
	
	
}
void idngIesAddGroup (unsigned char n,	unsigned char addGroup)
{
	if (addGroup<8)
	{
		SETBIT(_configDaliLoopGroupLow[n],addGroup);
	}
	else if ((addGroup>8) && (addGroup<=10))
	{
		SETBIT(_configDaliLoopGroupHigh[n],(addGroup-8));
	}
	storeSceneTimeout=20;	
}
void idngIesRemoveGroup (unsigned char n,unsigned char group)
{
	if (group<8)
	{
		CLEARBIT(_configDaliLoopGroupLow[n],group);
	}
	else if ((group>8) && (group<=10))
	{
		CLEARBIT(_configDaliLoopGroupHigh[n],(group-8));
	}
	storeSceneTimeout=20;
}


void idngIesRemoveScene (unsigned char output, unsigned char scene)
{
	sceneLevel[output][scene]=0xFF;	
	storeSceneTimeout=20;	
}
void setupIES_Get (unsigned short int* setupArray)
{
	unsigned char n;	
	setupArray[0]=_idngiesCfg_uso;
	setupArray[1]=_idngiesCfg_es;	
	setupArray[2]=_idngiesCfg_ilu;	
	setupArray[3]=_idngiesCfg_per;	
	setupArray[4]=_idngiesCfg_pulShort;
	setupArray[5]=_idngiesCfg_pulShortLong;
	setupArray[6]=_idngiesCfg_int;
	setupArray[7]=0;
	setupArray[8]=0;
	setupArray[9]=0;	
	for (n=0;n<11;n++)
	{
		if (powerOnLevel[n]==254)		SETBIT(setupArray[7],n);
		if (powerOnLevel[n]==0)			SETBIT(setupArray[8],n);
		if (powerOnLevel[n]==255)		SETBIT(setupArray[9],n);		
	}	
}

void iesSerialSet (unsigned char *serial)
{
	configStoreSerialToFlash (&serial[0]);
}

void iesSerialGet (unsigned char *serial)
{
	serial[0]=_serial[0];
	serial[1]=_serial[1];
	serial[2]=_serial[2];
	serial[3]=_serial[3];	
}

void setupIES_Set (unsigned short int* setupArray)
{
	unsigned char n;	
	for (n=0; n<11; n++)
	{
		if ((BITVAL(setupArray[0],n)==0x01) && (BITVAL(setupArray[1],n)==0x01))
		{
			if (BITVAL(_idngiesCfg_es,n)==0x00)
			{							
				sceneLevel[n][0]=255;
				sceneLevel[n][1]=255;
				sceneLevel[n][2]=255;
				sceneLevel[n][3]=255;
				sceneLevel[n][4]=255;
				sceneLevel[n][5]=255;
				sceneLevel[n][6]=255;
				sceneLevel[n][7]=255;
				sceneLevel[n][8]=255;
				sceneLevel[n][9]=255;
				sceneLevel[n][10]=255;
				sceneLevel[n][11]=255;
				sceneLevel[n][12]=255;
				sceneLevel[n][13]=255;
				sceneLevel[n][14]=255;
				sceneLevel[n][15]=255;
			}			
			if ((BITVAL(setupArray[7],n)==0x01) && (BITVAL(setupArray[8],n)==0x00) && (BITVAL(setupArray[9],n)==0x00))		powerOnLevel[n]=254;
			if ((BITVAL(setupArray[7],n)==0x00) && (BITVAL(setupArray[8],n)==0x01) && (BITVAL(setupArray[9],n)==0x00))		powerOnLevel[n]=0;
			if ((BITVAL(setupArray[7],n)==0x00) && (BITVAL(setupArray[8],n)==0x00) && (BITVAL(setupArray[9],n)==0x01))		powerOnLevel[n]=255;	
		}
	}	
	_idngiesCfg_uso=setupArray[0];
	_idngiesCfg_es=setupArray[1];	
	_idngiesCfg_ilu=setupArray[2];
	_idngiesCfg_per=setupArray[3];	
	_idngiesCfg_pulShort=setupArray[4];
	_idngiesCfg_pulShortLong=setupArray[5];
	_idngiesCfg_int=setupArray[6];	
	
	
	_idngIesCfgPendingCnt=3;
	
	//configStoreInFlash();		
	
	/*
	//configInit();
	
//	idgies_esquemaGpioInit();	
//	interfazInit ();
	//idngPowerOn();	
	
	//idngiesSalidasInit();
	
	//idngies_buscarInterfaces();	
	
//	sensoresInit ();
		//interfazInit ();
		
		
			//wdtInit();
	
	idngies_buscarInterfaces();	
	idgies_esquemaGpioInit();
	
	interfazInit ();
	sensoresInit();
	idngPowerOn();
	idngiesSalidasInit();
	
		interfazInit ();
	sensoresInit();
			idngies_buscarInterfaces();	
	idgies_esquemaGpioInit();
	idngPowerOn();
	*/


}


void idngPowerOn (void)
{
	// No implementado el 255.	
	unsigned char n;
	
	unsigned int backupR;
	
	backupR=RTC_ReadBackupRegister(RTC_BKP_DR1);
	
	for (n=0;n<11;n++)
	{
		if ((BITVAL(_idngiesCfg_uso,n)==0x01) && (BITVAL(_idngiesCfg_es,n)==0x01))	
		{
			if (powerOnLevel[n]!=0xFF)
			{
				arcLevel[n]=powerOnLevel[n];		
			}
			else
			{
				if (BITVAL(backupR,n)==0x01)
				{
					arcLevel[n]=254;		
				}
				else
				{
					arcLevel[n]=0;
				}				
			}			
		}
	}	
}



void graba (unsigned char input)
{
	unsigned int backupR;	
	if (powerOnLevel[input]==255)
	{
		backupR=RTC_ReadBackupRegister(RTC_BKP_DR1);		
		if (arcLevel[input]==0x00)	
		{
				CLEARBIT(backupR,input);
		}
		else
		{
				SETBIT(backupR,input);
		}		
		RTC_WriteBackupRegister(RTC_BKP_DR1,backupR);
	}	
}



unsigned char idngIesCom(unsigned char dest, unsigned char com)
{			
			unsigned char answer=0xFF;
			unsigned char level=0xFF;
			unsigned char scene=0xFF;
			unsigned char storeScene=0xFF;
			unsigned char removeScene=0xFF;
			unsigned char addGroup=0xFF;
			unsigned char removeGroup=0xFF;	
			unsigned char querySceneLevel=0xFF;
			unsigned char reset=0xFF;
			unsigned char storeActualLevelDTR=0xFF;
			unsigned char storePowerOnLevel=0xFF;
			
			unsigned char n;
			
			
			if (com==0x01)	level=0;
			if ((com==0x06) || (com==0x07) || (com==0x09))  	level=254;	
			if ((com>=0x11) && (com<=0x20))										scene=(com-0x11);
			if ((com>=0x41) && (com<=0x50))										storeScene=(com-0x41);
			if ((com>=0x51) && (com<=0x60))										removeScene=(com-0x51);
			if ((com>=0x61) && (com<=0x70))										addGroup=(com-0x61);
			if ((com>=0x71) && (com<=0x80))										removeGroup=(com-0x71);
			if ((com>=0xB1) && (com<=0xC0))										querySceneLevel=(com-0xB1);			
			if (com==0x2E)	storePowerOnLevel=0x01;
				
			
			if (dest==0xFF)
			{				
				for (n=0;n<11;n++)
				{
					if ((BITVAL(_idngiesCfg_uso,n)==0x01) && (BITVAL(_idngiesCfg_es,n)==0x01))
					{
						if (scene!=0xFF)							arcLevel[n]=sceneLevel[n][scene];				
						else if (storeScene!=0xFF)		idngIesStoreScene (n, storeScene);									
						else if (removeScene!=0xFF)		idngIesRemoveScene (n, storeScene);	
						else if (addGroup!=0xFF)			idngIesAddGroup (n,addGroup);
						else if (removeGroup!=0xFF)		idngIesRemoveGroup (n,removeGroup);
						else if (querySceneLevel!=0xFF)		answer=sceneLevel[n][scene];				
						else if (level!=0xFF) 	arcLevel[n]=level;	
						else if (storePowerOnLevel==0x01)
						{
							powerOnLevel[n]=_dtr[n];
						}
						graba(n);					
					}
				}
				
				
			}
			else if (((dest&0xE1)==0x81) && (((dest&0x1E)>>1)<16))
			{		
				for (n=0;n<11;n++)
				{
					if (((BITVAL(_idngiesCfg_uso,n)==0x01) && (BITVAL(_idngiesCfg_es,n)==0x01)) && (actualLevelAddressPerteneceGrupoGet(n, ((dest&0x1E)>>1))==0x01))
					{
						if (scene!=0xFF)	arcLevel[n]=sceneLevel[n][scene];					
						else if (storeScene!=0xFF)		idngIesStoreScene (n, storeScene);									
						else if (removeScene!=0xFF)		idngIesRemoveScene (n, storeScene);						
						else if (addGroup!=0xFF)			idngIesAddGroup (n,addGroup);
						else if (removeGroup!=0xFF)		idngIesRemoveGroup (n,removeGroup);
						else if (level!=0xFF)	arcLevel[n]=level;		
						else if (storePowerOnLevel==0x01)
						{
							powerOnLevel[n]=_dtr[n];
						}						
						graba(n);
					}
				}				
				
			}
			else if (((dest&0x81)==0x01) && (((dest&0x7E)>>1)<11))
			{					
				if ((BITVAL(_idngiesCfg_uso,((dest&0x7E)>>1))==0x01) && (BITVAL(_idngiesCfg_es,((dest&0x7E)>>1))==0x01))						
				{				
						if (scene!=0xFF)	arcLevel[((dest&0x7E)>>1)]=sceneLevel[((dest&0x7E)>>1)][scene];					
						else if (storeScene!=0xFF)		idngIesStoreScene (((dest&0x7E)>>1), storeScene);									
						else if (removeScene!=0xFF)		idngIesRemoveScene (((dest&0x7E)>>1), storeScene);						
						else if (addGroup!=0xFF)			idngIesAddGroup (((dest&0x7E)>>1),addGroup);
						else if (removeGroup!=0xFF)		idngIesRemoveGroup (((dest&0x7E)>>1),removeGroup);
						else if (level!=0xFF) 	arcLevel[((dest&0x7E)>>1)]=level;	
						else if (storePowerOnLevel==0x01)
						{
							powerOnLevel[((dest&0x7E)>>1)]=_dtr[((dest&0x7E)>>1)];
						}					
						graba(((dest&0x7E)>>1));
			}
			}			
			
			return answer;
		}




void idngIesDoTask (void)
{
	unsigned char n;		
	
	if (_idngIesCfgPendingCnt==0)
	{
		configStoreInFlash();	
		_idngIesCfgPendingCnt=0xFF;
		idgies_esquemaGpioInit();	
		interfazInit ();
		sensoresInit();
		idngiesSalidasInit();
		idngies_buscarInterfaces();	
		idngPowerOn();
	}
	if (storeSceneTimeout==0)
	{
		configStoreInFlash();	
		storeSceneTimeout=0xFF;
	}
	for (n=0;n<11;n++)
	{			
		if ((BITVAL(_idngiesCfg_uso,n)==0x01) && (BITVAL(_idngiesCfg_es,n)==0x01))	
		{
			if ((BITVAL(_idngiesCfg_ilu,n)==0x01) && (BITVAL(_idngiesCfg_per,n)==0x00))
			{
				daliLoopActualLevel[n]=arcLevel[n];
				daliLoopActualLevelValid[n]=0x01;
				if (arcLevel[n]==0)
				{
					boardGPIOPinValueSet (n, 0);			
				}
				else
				{
					boardGPIOPinValueSet (n, 1);
				}
			}
			else
			{
				daliLoopActualLevelValid[n]=0x00;
			}
			
			if ((BITVAL(_idngiesCfg_per,n)==0x01) && (BITVAL(_idngiesCfg_ilu,n)==0x00))
			{
				if (_blindPin[n]==1)
				{
					boardGPIOPinValueSet (n, 1);			
				}
				else
				{
					boardGPIOPinValueSet (n, 0);
				}				
			}			
		}
	}	
}


void idgies_esquemaGpioInit (void)
{
	unsigned char n;
	for (n=0;n<10;n++)
	{
		boardGPIODirSet (n, 0x00);
	}
	boardGPIODirSet (10, 0x01);	
	_algunaEntradaEnUso=0;
	// INICIALIZACION DE ENTRADAS.	
	for (n=0;n<10;n++)
	{
		if ((BITVAL(_idngiesCfg_uso,n)==0x01) && (BITVAL(_idngiesCfg_es,n)==0x00))		
		{
			boardGPIODirSet (n, 0x00);
			_algunaEntradaEnUso=1;
		}
	}	
	// INICIALIZACION DE SALIDAS.
	for (n=0;n<10;n++)
	{
		if ((BITVAL(_idngiesCfg_uso,n)==0x01) && (BITVAL(_idngiesCfg_es,n)==0x01))		
		{
			boardGPIODirSet (n, 0x01);
		}
	}	
	
	for (n=0;n<5;n++)
	{					
		if ((BITVAL(_idngiesCfg_per,((2*n)+0))==0x01) && (BITVAL(_idngiesCfg_per,((2*n)+1))==0x01))
		{
			_blindUsed[n]=1;	
		}
		else		
		{
			_blindUsed[n]=0;	
		}
	}	
}


unsigned char idngies_buscarInterfaces(void)
{	
	unsigned char interfazAddress;
	unsigned char 												configArray[INTERFACES_CONFIGURACION_S_SIZE];		
	unsigned char n;		
	unsigned char interfacesCnt=0;			
	configInterfacesIniciarConfiguracion();		
	for (interfazAddress=0;interfazAddress<16;interfazAddress++)
	{
		if ((interfazAddress==0) && (_algunaEntradaEnUso==0x01))
		{
			_interfazConfiguracionBackup[interfazAddress].instalado=0x01;			
			_interfazConfiguracionBackup[interfazAddress].desactivado=0x00;			
			_interfazConfiguracionBackup[interfazAddress].numeroSerie.byte[0]=_serial[0];	// SERIAL 1
			_interfazConfiguracionBackup[interfazAddress].numeroSerie.byte[1]=_serial[1];	// SERIAL 2
			_interfazConfiguracionBackup[interfazAddress].numeroSerie.byte[2]=_serial[2];	// SERIAL 3		
			_interfazConfiguracionBackup[interfazAddress].numeroSerie.byte[3]=_serial[3];	// SERIAL 4
			_interfazConfiguracionBackup[interfazAddress].tipoInterfaz=INTERFACES_TIPO_INTERFAZ_IDNGIES;				
			_interfazConfiguracionBackup[interfazAddress].modoInterfaz=INTERFACES_MODO_INTERFAZ_IDNGIES;				
			interfacesConfiguracionArrayFromStructGet(_interfazConfiguracionBackup[interfazAddress], &configArray[0]);	
			interfacesConfiguracionSet(interfazAddress, &configArray[0]);		
			interfacesCnt++;
		}
		else
		{
			_interfazConfiguracionBackup[interfazAddress].instalado=0x00;
			_interfazConfiguracionBackup[interfazAddress].desactivado=0x00;
			_interfazConfiguracionBackup[interfazAddress].numeroSerie.byte[0]=0xFF;	// SERIAL 1 
			_interfazConfiguracionBackup[interfazAddress].numeroSerie.byte[1]=0xFF;	// SERIAL 2
			_interfazConfiguracionBackup[interfazAddress].numeroSerie.byte[2]=0xFF;	// SERIAL 3
			_interfazConfiguracionBackup[interfazAddress].numeroSerie.byte[3]=0xFF;	// SERIAL 4
			_interfazConfiguracionBackup[interfazAddress].tipoInterfaz=0;
			// ¿? fill parameters with 0xFF.		
			interfacesConfiguracionArrayFromStructGet(_interfazConfiguracionBackup[interfazAddress], &configArray[0]);	
			interfacesConfiguracionSet(interfazAddress, &configArray[0]);	
		}
	}
		// Finalizar.	
	configInterfacesFinalizarConfiguracion();
	return interfacesCnt;
}

void idngiesSalidasInit (void)
{
	unsigned char n;
	unsigned char matrizOcupadas[64];
	unsigned char matrizTipo[64];
	unsigned char ocupadas=0;
	
	for (n=0;n<64;n++)
	{		
		if (((BITVAL(_idngiesCfg_uso,n)==0x01) && (BITVAL(_idngiesCfg_es,n)==0x01) && (BITVAL(_idngiesCfg_ilu,n)==0x01)) && (n<11))		
		{
			matrizOcupadas[n]=1;
			matrizTipo[n]=0x06;
			ocupadas++;
		}
		else
		{
			matrizOcupadas[n]=0;	
			matrizTipo[n]=0xff;
		}
	}
	configDaliLoopInstalledSet (&matrizOcupadas[0], 0x00);
	configDaliLoopDeviceTypeSet (&matrizOcupadas[0], &matrizTipo[0], 0x01);				
	_comA[0]=0x40;
	_comA[1]=0x00;
	_ipA[0]=_configIpAddress[0];
	_ipA[1]=_configIpAddress[1];
	_ipA[2]=_configIpAddress[2];
	_ipA[3]=255;	
	daliAddressingNotify(0x01,0x02,0x01,0xFF,0xFF,ocupadas);	
}












