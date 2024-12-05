#include "config.h"
#include "rtcc.h"
#include "daliLoop.h"
#include "polling.h"
#include "historico.h"
#include "ethTransceiver.h"
#include "ethFramming.h"
#include "normalink.h"
#include "wdt.h"

extern unsigned char recibireth[320];		// Búffer para paquete entrante
extern unsigned char ipDest[4];	//Ip del host que me envía el paquete
extern unsigned int portDest;		//Puerto del host que me envía el paquete



void normaLinkCmdPasarelaT5(unsigned char _comL)
{
	unsigned short int _lenghtRx;
	unsigned char _respuesta[268];
	_lenghtRx=(recibireth[6]*256)+recibireth[7];
	
	if ((_comL==0xA0) && (_lenghtRx==2))		// LEER FLASH [SECTOR][PAGINA]
	{		
		_respuesta[0]=0x84;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];			
		_respuesta[3]=1;								// Lenght HIGH.
		_respuesta[4]=2;								// Lenght LOW.
		_respuesta[5]=recibireth[8];		// Rebote SECTOR.
		_respuesta[6]=recibireth[9];		// Rebote PÁGINA.			
		if (recibireth[8]<8)
		{		
			m25pReadBytes(recibireth[8], recibireth[9],0,&_respuesta[7],256);	
			ethSendFrame(0,&_respuesta[0],263,&ipDest[0], portDest);		
		}		
	}
	
	if ((_comL==0xA1) && (_lenghtRx==258))		// GRABAR FLASH.	[SECTOR][PÁGINA][CONTENIDO].
	{
		unsigned char status;			
		m25pWriteEnable();
		m25pWriteBytes(recibireth[8],recibireth[9],0,&recibireth[10],256);	
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);	
		_respuesta[0]=0x84;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];			
		_respuesta[3]=0;								// Lenght HIGH.
		_respuesta[4]=3;								// Lenght LOW.
		_respuesta[5]=recibireth[8];		// Rebote SECTOR.
		_respuesta[6]=recibireth[9];		// Rebote PÁGINA.	
		_respuesta[7]=0x01;							// Hecho.
		ethSendFrame(0,&_respuesta[0],8,&ipDest[0], portDest);	
	}
	
	if ((_comL==0xA2) && (_lenghtRx==1))		// BORRAR SECTOR [SECTOR].
	{		
		unsigned char status;			
		if (recibireth[7]<8)
		{		
			m25pWriteEnable();
			m25pSectorErase (recibireth[7],0,0);
			do
			{		
				status=m25pStatusRead();
			}while((status&0x01)==0x01);
			_respuesta[0]=0x84;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];			
			_respuesta[3]=0;								// Lenght HIGH.
			_respuesta[4]=2;								// Lenght LOW.
			_respuesta[5]=recibireth[8];		// Rebote SECTOR.
			_respuesta[6]=0x01;							// Hecho.
			ethSendFrame(0,&_respuesta[0],7,&ipDest[0], portDest);				
		}			
	}
	
	
	
	
}






void normaLinkCmdPasarelaT0(unsigned char _comL)
{
	unsigned char _tipoDeFrame;
	unsigned char _respuesta[6];
	
	// [000] REINICIALIZAR PASARELA
	if (_comL==0x00) 
	{
		wdtReboot();
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}	
	
		// [001] REINICIALIZAR CONFIGURACION DE RED
	if (_comL==0x01) 
	{
		ethTransceiverReset();
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);	
	}	
		
	// [025] ACCESO EXCLUSIVO A BUS
	if (_comL==0x25) 
	{
		if (normaLinkDaliAccesoExclusivoABusEnable(&ipDest[0])==0x01)
		{
			_tipoDeFrame=0x82;
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x01;
			_respuesta[4]=0x00;							
		}
		else
		{
			_tipoDeFrame=0x82;			
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0;
			_respuesta[4]=0;			
		}
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);	
	}	
	// [026] FIN ACCESO EXCLUSIVO A BUS
	if (_comL==0x26) 
	{
		normaLinkDaliAccesoExclusivoABusDisable();
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;		
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}	
	// [025] ESTADO BLOQUEO ACCESO A BUS
	// [026] ESTADO ACCESO EXCLUSIVO A BUS
	
	if (_comL==0xFF)																		// PING
	{
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x04;		
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);	
	}
}


