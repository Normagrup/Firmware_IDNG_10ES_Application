#include "macros.h"
#include "dali.h"
#include "m25p.h"
#include "daliLoop.h"
#include "temp.h"
#include "test.h"
#include "fallos.h"

unsigned char 	daliLoopEmergencyModeValid[64];
unsigned char 	daliLoopEmergencyMode[64];

unsigned char 	daliLoopFailureStatus[64];
unsigned char 	daliLoopFailureStatusValid[64];

unsigned char 	daliLoopStatusValid[64];
unsigned char 	daliLoopStatus[64];

unsigned char 	daliLoopActualLevelValid[64];
unsigned char 	daliLoopActualLevel[64];

unsigned char 	daliLoopErrorCom[64];
unsigned char 	daliLoopErrorComCnt[64];
unsigned char 	_actualLevelNotif;


extern unsigned char _configIpAddress[4];







unsigned char daliLoopStatusGet (unsigned char address)
{
	unsigned char answerTmp=0;	
	unsigned char daliLoopInstalled[64];	
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	
	if (daliLoopInstalled[address]==0x01)
	{
		 if ((daliLoopErrorCom[address]==0x00) && (daliLoopStatusValid[address]==0x01))
		 {
			 answerTmp=daliLoopStatus[address];
		 }		
		 else
		 {		 	
			if (daliLoopStatusValid[address]==0x00)
			{
				answerTmp=answerTmp+0x40;
			}
		
			if (daliLoopErrorCom[address]==0x01)
			{
				answerTmp=answerTmp+0x80;
			}	
		}
	}
	return answerTmp;	
}

unsigned char daliLoopErrorComGet (unsigned char address)
{
	return daliLoopErrorCom[address];
}

unsigned char daliLoopInstalledArrayByteGet (unsigned char *arrayByte)
{
	unsigned char answerTmp=0;
	unsigned char cnt=0;
	unsigned char addressTmp;
	unsigned char i;
	unsigned char j;
	unsigned char daliLoopInstalled[64];
	
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	
	for (i=0;i<8;i++)
	{
		answerTmp=0x00;
		for (j=0;j<8;j++)
		{
			addressTmp=(i*8)+j;
			if (daliLoopInstalled[addressTmp]==0x01)
			{
				answerTmp=(answerTmp+(0x01<<j));
				cnt++;
			}
		}
		arrayByte[i]=answerTmp;
	}	
	return cnt;
}


unsigned char daliLoopDeviceTypeMatchArrayByteGet (unsigned char *arrayByte, unsigned char deviceType)
{
	unsigned char answerTmp=0;
	unsigned char cnt=0;
	unsigned char addressTmp;
	unsigned char i;
	unsigned char j;
	unsigned char daliLoopInstalled[64];
	unsigned char daliLoopDeviceTypeValid[64];
	unsigned char daliLoopDeviceType[64];
	
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	configDaliLoopDeviceTypeGet(&daliLoopDeviceTypeValid[0],&daliLoopDeviceType[0]);

	
	
	
	for (i=0;i<8;i++)
	{
		answerTmp=0x00;
		for (j=0;j<8;j++)
		{
			addressTmp=(i*8)+j;
			if ((daliLoopInstalled[addressTmp]==0x01) && (daliLoopDeviceTypeValid[addressTmp]==0x01) && (daliLoopDeviceType[addressTmp]==deviceType))
			{
				answerTmp=(answerTmp+(0x01<<j));
				cnt++;
			}
		}
		arrayByte[i]=answerTmp;
	}	
	return cnt;
}


unsigned char daliLoopDeviceTypeMatchAndEvenArrayByteGet (unsigned char *arrayByte, unsigned char deviceType)
{
	unsigned char answerTmp=0;
	unsigned char cnt=0;
	unsigned char addressTmp;
	unsigned char i;
	unsigned char j;
	unsigned char daliLoopInstalled[64];
	unsigned char daliLoopDeviceTypeValid[64];
	unsigned char daliLoopDeviceType[64];
	
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	configDaliLoopDeviceTypeGet(&daliLoopDeviceTypeValid[0],&daliLoopDeviceType[0]);

	
	
	
	for (i=0;i<8;i++)
	{
		answerTmp=0x00;
		for (j=0;j<8;j++)
		{
			addressTmp=(i*8)+j;
			if ((daliLoopInstalled[addressTmp]==0x01) && (daliLoopDeviceTypeValid[addressTmp]==0x01) && (daliLoopDeviceType[addressTmp]==deviceType) && ((addressTmp&0x01)==0x00))
			{
				answerTmp=(answerTmp+(0x01<<j));
				cnt++;
			}
		}
		arrayByte[i]=answerTmp;
	}	
	return cnt;
}

unsigned char daliLoopDeviceTypeMatchAndOddArrayByteGet (unsigned char *arrayByte, unsigned char deviceType)
{
	unsigned char answerTmp=0;
	unsigned char cnt=0;
	unsigned char addressTmp;
	unsigned char i;
	unsigned char j;
	unsigned char daliLoopInstalled[64];
	unsigned char daliLoopDeviceTypeValid[64];
	unsigned char daliLoopDeviceType[64];
	
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	configDaliLoopDeviceTypeGet(&daliLoopDeviceTypeValid[0],&daliLoopDeviceType[0]);

	
	
	
	for (i=0;i<8;i++)
	{
		answerTmp=0x00;
		for (j=0;j<8;j++)
		{
			addressTmp=(i*8)+j;
			if ((daliLoopInstalled[addressTmp]==0x01) && (daliLoopDeviceTypeValid[addressTmp]==0x01) && (daliLoopDeviceType[addressTmp]==deviceType) && ((addressTmp&0x01)==0x01))
			{
				answerTmp=(answerTmp+(0x01<<j));
				cnt++;
			}
		}
		arrayByte[i]=answerTmp;
	}	
	return cnt;
}










// Devuelve 0x01 si el valor el bit est� marcado en el array de 64 bits y 0x00 en caso contrario.
unsigned char daliLoopDeviceBitFromByteArrayGet (unsigned char *byteArray, unsigned char device)
{
	unsigned char byte;
	unsigned char bit;
	unsigned char answer;
	byte=device/8;
	bit=device-(8*byte);
	answer=BITVAL(byteArray[byte],bit);
	return answer;
}














// Incrementa el contador de errores de comunicaci�n de un equipo en concreto, devuelve 0x01 cuando no puede incrementarlo m�s, y 0x00 en caso contrario.
unsigned char daliLoopErrorComCntIncrease (unsigned char address)
{
	unsigned char daliLoopInstalled[64];
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	if (daliLoopInstalled[address]==0x01)
	{
		if (daliLoopErrorComCnt[address]<DALI_LOOP_ERROR_COM_RETRIES)
		{
			daliLoopErrorComCnt[address]++;
		}
		else
		{
			return 0x01;
		}
	}	
	return 0x00;
}


// Decrementa el contador de errores de comunicaci�n de un equipo en concreto, devuelve 0x01 cuando no puede decrementarlo m�s y 0x00 en caso contrario.
unsigned char daliLoopErrorComCntDecrease (unsigned char address)
{
	unsigned char daliLoopInstalled[64];
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	if (daliLoopInstalled[address]==0x01)
	{	
		if (daliLoopErrorComCnt[address]>0)
		{
			daliLoopErrorComCnt[address]--;
		}	
		else
		{
			return 0x01;
		}
	}
	return 0x00;
}














// Borra el bit valid...
void daliLoopActualLevelValidClearAll (void)
{
	unsigned char n;
	for (n=0;n<64;n++)
	{	
		daliLoopActualLevelValid[n]=0x00;
	}	
}


// Borra el bit valid...
void daliLoopStatusValidClearAll (void)
{
	unsigned char n;
	for (n=0;n<64;n++)
	{	
		daliLoopStatusValid[n]=0x00;
	}	
}





// Devuelve 0x01 si el equipo est� instalado y 0x00 en caso contrario.
unsigned char daliLoopInstalledGet (unsigned char address)
{
	unsigned char daliLoopInstalled[64];
	if (address<64)
	{
		configDaliLoopInstalledGet(&daliLoopInstalled[0]);
		return daliLoopInstalled[address];
	}
	else
	{
		return 0x00;
	}
}

// Devuelve el Byte Device Type.
unsigned char daliLoopDeviceTypeGet (unsigned char address)
{
	unsigned char answerTmp=0;		
	unsigned char daliLoopInstalled[64];
	unsigned char daliLoopDeviceTypeValid[64];
	unsigned char daliLoopDeviceType[64];
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);	
	configDaliLoopDeviceTypeGet(&daliLoopDeviceTypeValid[0],&daliLoopDeviceType[0]);
	if (address<64)
	{
		if (daliLoopInstalled[address]==0x01)
		{
			if (daliLoopDeviceTypeValid[address]==0x01)
			{
				answerTmp=daliLoopDeviceType[address];
			}		
			else
			{		 	
				if (daliLoopDeviceTypeValid[address]==0x00)
				{
					answerTmp=answerTmp+0x40;
				}
			}
		}
		return answerTmp;
	}
	else
	{
		return 0x00;
	}
}































// Devuelve el Byte Emergency Mode de un equipo en concreto.
unsigned char daliLoopEmergencyModeGet (unsigned char address)
{
	unsigned char answerTmp=0;		
	unsigned char daliLoopInstalled[64];
	unsigned char daliLoopDeviceTypeValid[64];
	unsigned char daliLoopDeviceType[64];
	
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	configDaliLoopDeviceTypeGet(&daliLoopDeviceTypeValid[0],&daliLoopDeviceType[0]);
	
	
	if ((daliLoopInstalled[address]==0x01) && (daliLoopDeviceType[address]==0x01))
	{
		 if ((daliLoopErrorCom[address]==0x00) && (daliLoopEmergencyModeValid[address]==0x01))
		 {
			 answerTmp=daliLoopEmergencyMode[address];
		 }		
		 else
		 {
			if (daliLoopEmergencyModeValid[address]==0x00)
			{
				answerTmp=answerTmp+0x40;
			}
			if (daliLoopErrorCom[address]==0x01)
			{
				answerTmp=answerTmp+0x80;
			}	
		}
	}
	return answerTmp;
}

// Devuelve el Byte Failure Status de un equipo en concreto.
unsigned char daliLoopFailureStatusGet (unsigned char address)
{
	unsigned char answerTmp=0;	
	unsigned char daliLoopInstalled[64];
	unsigned char daliLoopDeviceTypeValid[64];
	unsigned char daliLoopDeviceType[64];
	
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	configDaliLoopDeviceTypeGet(&daliLoopDeviceTypeValid[0],&daliLoopDeviceType[0]);
	
	
	if ((daliLoopInstalled[address]==0x01) && (daliLoopDeviceType[address]==0x01))
	{
		 if ((daliLoopErrorCom[address]==0x00) && (daliLoopFailureStatusValid[address]==0x01))
		 {
			 answerTmp=daliLoopFailureStatus[address];
		 }		
		 else
		 {
			if (daliLoopFailureStatusValid[address]==0x00)
			{
				answerTmp=answerTmp+0x40;
			}
			if (daliLoopErrorCom[address]==0x01)
			{
				answerTmp=answerTmp+0x80;
			}	
		}
	}
	return answerTmp;
}

// Devuelve 0x01 si el Actual Level de un equipo en concreto es valido y 0x00 en caso contrario.
unsigned char daliLoopActualLevelValidGet (unsigned char address)
{
	return daliLoopActualLevelValid[address];	
}


unsigned char daliLoopAddressBeyondsToGroup (unsigned char dest, unsigned char address)
{
	unsigned char group;
	unsigned char valid;
	unsigned char answer=0x00;
	unsigned int groupsMatrix;	
	group=((dest&0x1E)/2);
	configDaliLoopGroupsGet (address, &valid, &groupsMatrix);
	if ((valid==0x01) && (group<16))
	{
		if (BITVAL(groupsMatrix,group)==0x01)
		{
			answer=0x01;
		}		
	}
	return answer;
}

unsigned char daliLoopAddressMatch (unsigned char dest, unsigned char address)
{
	if (((dest&0x7E)>>1)==address)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}


// Fija el nivel de luz de un equipo.
// En primer lugar comprueba si el comando es aplicable a un equipo en concreto o no.
// Estar� dirigido si:
// - existe en la instalaci�n.
// - es del tipo adecuado para recibir esos comandos.
// - puede en ese momento reaccionar a ese comando.
// - el comando est� dirigido a �l:
	// - Broadcast.
	// - Dirigido a su grupo.
	// - Dirigido especificamente a �l.
	// - Pertenece a la escena.

// El nivel de luz se fijar� entre el m�ximo y el m�nimo del equipo.

// En la �nica situaci�n que no podemos esperar una respuesta instant�nea es en el caso del uso del polo aparte.
// Esto puede hacerse haciendo polling al bit 7 del EMERGENCY MODE, pero es mejor soluci�n hacer polling del actual level.
// En todo caso haciendo polling al bit 7 del EMERGENCY MODE, en caso de que alguna vez de 1, puede conocerse qu� modelos son susceptibles de que su actual level haya cambiado.
// Otras posibilidades de que el nivel var�e son un interface failure y un power on event.

// Cuando el nivel se fija ese nivel es virtual en tanto a que no ha sido contrastado con la realidad.
// Al hacer un polling debemos comprobar la coherencia con el bit de LAMP POWER ON y el que informa del estado del FADDING.
// S�lamente en caso de que no haya un fadding ejecut�ndose y que haya coherencia con LAMP POWER ON y el estado de la l�mpara cambiar�amos el nivel.

// En un primer estado se realizar� as�:

void daliLoopActualLevelSet(unsigned char address, unsigned char level)
{
	if (daliLoopInstalledGet (address)==0x01)
	{	
		daliLoopActualLevelValid[address]=0x01;
		daliLoopActualLevel[address]=level;	
	}
}


// Devuelve 0 si alguna apagada, en caso contrario devuelve 1.
unsigned char daliLoopGroupActualLevelGet (unsigned char group)
{
	unsigned char answer=0x00;	
	unsigned char valid;
	unsigned char address;
	unsigned int groupsMatrix;		
	for (address=0;address<64;address++)
	{	
		configDaliLoopGroupsGet (address, &valid, &groupsMatrix);	
		if ((group<16) && (valid==0x01))
		{
			if (BITVAL(groupsMatrix,group)==0x01)
			{
				if ((daliLoopActualLevelValid[address]==0x01) && (daliLoopActualLevel[address]==0x00)) 		answer=0x01;
			}		
		}	
	}	
	return answer;
}










// Devuelve el Byte Actual Level de un equipo en concreto.
unsigned char daliLoopActualLevelGet (unsigned char address)
{
	unsigned char answerTmp=0xFF;			
	unsigned char daliLoopInstalled[64];
	unsigned char daliLoopDeviceTypeValid[64];
	unsigned char daliLoopDeviceType[64];	
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	configDaliLoopDeviceTypeGet(&daliLoopDeviceTypeValid[0],&daliLoopDeviceType[0]);
	
	if (daliLoopInstalled[address]==0x01)
	{
		 if ((daliLoopErrorCom[address]==0x00) && (daliLoopActualLevelValid[address]==0x01))
		 {
				answerTmp=daliLoopActualLevel[address]; 		 
		 }		
		 else
		 {		 	
			if (daliLoopActualLevelValid[address]==0x00)
			{
				answerTmp=answerTmp+0x40;
			}
			if (daliLoopErrorCom[address]==0x01)
			{
				answerTmp=answerTmp+0x80;
			}	
		}
	}
	return answerTmp;
}
























void daliLoopActualLevelApplyGet (unsigned char *apply)
{
	unsigned char i;
	unsigned char j;	
	for (i=0;i<8;i++)
	{	
		for (j=0;j<8;j++)
		{		
			if ((daliLoopStatusValid[((8*i)+j)]==0x01) && (BITVAL(daliLoopStatus[((8*i)+j)],4)==0x00))
			{		
				SETBIT(apply[i],j);
			}
			else
			{
				CLEARBIT(apply[i],j);
			}
		}
	}
}









void daliLoopBatchFrameProcess(unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char *daliSentApply, unsigned char *validArray, unsigned char *answerArray)
{
	unsigned char n;		
	unsigned char daliLoopInstalled[64];
	unsigned char daliLoopDeviceTypeValid[64];
	unsigned char daliLoopDeviceType[64];
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	configDaliLoopDeviceTypeGet(&daliLoopDeviceTypeValid[0],&daliLoopDeviceType[0]);
	for (n=0;n<64;n++)
	{	
		if ((daliLoopInstalled[n]==0x01) && (daliLoopDeviceBitFromByteArrayGet(daliSentApply,n)==0x01))			//El equipo existe y se le pregunt�
		{
			if (daliLoopDeviceBitFromByteArrayGet(validArray,n)==0x01)																				// Se realiz� la operaci�n y hay respuesta v�lida
			{
				if (daliLoopErrorComCntDecrease(n)==0x01)
				{
					fallosComunicacion(n, 0x00);
					daliLoopErrorCom[n]=0x00;
				}
				if ((daliSentCommandHigh==0x2E) && (daliSentCommandLow==0xA0))
				{
						daliLoopActualLevelValid[n]=0x01;
						if ((daliLoopActualLevel[n]!=answerArray[n]) && (answerArray[n]!=255))
						{									
							_actualLevelNotif=0x01;
							daliLoopActualLevel[n]=answerArray[n];	
						}
				}
				if ((daliSentCommandHigh==0x2E) && (daliSentCommandLow==0x90))
				{
					daliLoopStatusValid[n]=0x01;					
					daliLoopStatus[n]=answerArray[n];						
				}
				if ((daliSentCommandHigh==0x30) && (daliSentCommandLow==0xFA))
				{
					daliLoopEmergencyModeValid[n]=0x01;
					daliLoopEmergencyMode[n]=answerArray[n];															
				}
				if ((daliSentCommandHigh==0x30) && (daliSentCommandLow==0xFC))
				{
					daliLoopFailureStatusValid[n]=0x01;	
					daliLoopFailureStatus[n]=answerArray[n];																					
					fallosProcesar (n, 0x01,  daliLoopFailureStatus[n]);						
				}					
			}
			else
			{
				if (daliLoopErrorComCntIncrease(n)==0x01)
				{						
					fallosComunicacion(n, 0x01);
					daliLoopErrorCom[n]=0x01;
				}
			}			
		}
	}	
}
	







void daliLoopDoTask (void)
{
	unsigned char ip[4];
	unsigned char respuesta[76];
	unsigned char n;
	unsigned char valid[8];	
	if (_actualLevelNotif==0x01)
	{
		_actualLevelNotif=0x00;
		sensoresGruposReguladoresLuzEstadoActualizar (0xFFFF);				// Aqu� hay que llamar a actualizar todos los grupos de reguladores de luz.
		ip[0]=_configIpAddress[0];
		ip[1]=_configIpAddress[1];
		ip[2]=_configIpAddress[2];
		ip[3]=0xFF;		
		valid[0]=daliLoopValidByteGet(3,7);
		valid[1]=daliLoopValidByteGet(3,6);
		valid[2]=daliLoopValidByteGet(3,5);
		valid[3]=daliLoopValidByteGet(3,4);
		valid[4]=daliLoopValidByteGet(3,3);
		valid[5]=daliLoopValidByteGet(3,2);
		valid[6]=daliLoopValidByteGet(3,1);
		valid[7]=daliLoopValidByteGet(3,0);		
		respuesta[0]=0x80;
		respuesta[1]=0x40;
		respuesta[2]=0x60;
		for (n=0;n<8;n++)
		{
				respuesta[(3+n)]=valid[n];
		}		
		for (n=0;n<64;n++)
		{
				respuesta[(11+n)]=daliLoopActualLevel[(63-n)];
		}
		ethSendFrame(0,&respuesta[0],75,&ip[0], 2234);	
	}	
}









unsigned char daliLoopValidByteGet(unsigned char op, unsigned char byte)
{
	unsigned char answerTmp=0;
	unsigned char addressTmp;
	unsigned char n;
	
		unsigned char daliLoopInstalled[64];
	unsigned char daliLoopDeviceTypeValid[64];
	unsigned char daliLoopDeviceType[64];
	
	configDaliLoopInstalledGet(&daliLoopInstalled[0]);
	configDaliLoopDeviceTypeGet(&daliLoopDeviceTypeValid[0],&daliLoopDeviceType[0]);
	
	
	if (op==0)
	{
		
		for (n=0;n<8;n++)
		{
			addressTmp=(byte*8)+n;
			if ((daliLoopInstalled[addressTmp]==0x01) && (daliLoopErrorCom[addressTmp]==0x00) && (daliLoopActualLevelValid[addressTmp]))
			{
					answerTmp=(answerTmp+(0x01<<n));
			}		
		}		
	}
	
	if (op==1)
	{
		
		for (n=0;n<8;n++)
		{
			addressTmp=(byte*8)+n;
			if ((daliLoopInstalled[addressTmp]==0x01) && (daliLoopDeviceType[addressTmp]==0x01) && (daliLoopErrorCom[addressTmp]==0x00) && (daliLoopEmergencyModeValid[addressTmp]))
			{
					answerTmp=(answerTmp+(0x01<<n));
			}		
		}		
	}
	
		if (op==2)
	{
		
		for (n=0;n<8;n++)
		{
			addressTmp=(byte*8)+n;
			if ((daliLoopInstalled[addressTmp]==0x01) && (daliLoopDeviceType[addressTmp]==0x01) && (daliLoopErrorCom[addressTmp]==0x00) && (daliLoopFailureStatusValid[addressTmp]==0x01))
			{
					answerTmp=(answerTmp+(0x01<<n));
			}		
		}		
	}
	
	if (op==3)
	{
		
		for (n=0;n<8;n++)
		{
			addressTmp=(byte*8)+n;
			if ((daliLoopInstalled[addressTmp]==0x01) && (daliLoopDeviceTypeValid[addressTmp]==0x01))
			{
					answerTmp=(answerTmp+(0x01<<n));
			}		
		}		
	}
	
		
	if (op==4)
	{
		for (n=0;n<8;n++)
		{
			addressTmp=(byte*8)+n;
			if (daliLoopInstalled[addressTmp]==0x01)
			{
					answerTmp=(answerTmp+(0x01<<n));
			}		
		}		
	}
	
		if (op==5)
	{
		for (n=0;n<8;n++)
		{
			addressTmp=(byte*8)+n;
			if ((daliLoopInstalled[addressTmp]==0x01) && (daliLoopErrorCom[addressTmp]==0x00) && (daliLoopStatusValid[addressTmp]==0x01))
			{
					answerTmp=(answerTmp+(0x01<<n));
			}		
		}		
	}
	
	return answerTmp;
}


