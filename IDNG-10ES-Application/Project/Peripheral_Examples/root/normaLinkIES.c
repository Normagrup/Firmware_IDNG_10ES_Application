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




void	normaLinkIES(unsigned char com)
{
	unsigned short int ptr=0;	
	unsigned char respuesta[20];
	unsigned short int cfg[10];
	unsigned char serial[4];
	unsigned char parameters[3];
	
	if (com==0x00)	//	LEER CFG		ENTRA:0		SALE:16		SIN PARAMETROS.
	{
		setupIES_Get (&cfg[0]);
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);								// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);								// Cmd High.
		bytePush(0x00,&respuesta[ptr],&ptr);								// Cmd Low.
		bytePush(0x13,&respuesta[ptr],&ptr);								// Lenght=20
		
		cfg[0]|=0x0400;
		cfg[1]|=0x0400;
		cfg[2]|=0x0400;
		cfg[3]&=0xFBFF;
		cfg[4]&=0xFBFF;
		cfg[5]&=0xFBFF;
		cfg[6]&=0xFBFF;
		
		
		uint16Push(cfg[0], &respuesta[ptr], &ptr);
		uint16Push(cfg[1], &respuesta[ptr], &ptr);
		
		
		
		uint16Push(cfg[2], &respuesta[ptr], &ptr);
		uint16Push(cfg[3], &respuesta[ptr], &ptr);
		
		
		
		uint16Push(cfg[4], &respuesta[ptr], &ptr);
		uint16Push(cfg[5], &respuesta[ptr], &ptr);
		
		uint16Push(cfg[6], &respuesta[ptr], &ptr);
		uint16Push(cfg[7], &respuesta[ptr], &ptr);		
		
		uint16Push(cfg[8], &respuesta[ptr], &ptr);	
		uint16Push(cfg[9], &respuesta[ptr], &ptr);	
		
		

		
		
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
	}
	
	if (com==0x01)	//	GRABAR CFG	ENTRA:16	SALE: CONFIRMACION.
	{		
		ptr=7;
		uint16Pop(&recibireth[ptr], &cfg[0], &ptr);
		uint16Pop(&recibireth[ptr], &cfg[1], &ptr);
		uint16Pop(&recibireth[ptr], &cfg[2], &ptr);
		uint16Pop(&recibireth[ptr], &cfg[3], &ptr);
		uint16Pop(&recibireth[ptr], &cfg[4], &ptr);
		uint16Pop(&recibireth[ptr], &cfg[5], &ptr);
		uint16Pop(&recibireth[ptr], &cfg[6], &ptr);
		uint16Pop(&recibireth[ptr], &cfg[7], &ptr);		
		uint16Pop(&recibireth[ptr], &cfg[8], &ptr);
		uint16Pop(&recibireth[ptr], &cfg[9], &ptr);		
		
		
		cfg[0]|=0x0400;
		cfg[1]|=0x0400;
		cfg[2]|=0x0400;
		cfg[3]&=0xFBFF;
		cfg[4]&=0xFBFF;
		cfg[5]&=0xFBFF;
		cfg[6]&=0xFBFF;
		
		
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);								// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);								// Cmd High.
		bytePush(0x01,&respuesta[ptr],&ptr);								// Cmd Low.
		bytePush(0x00,&respuesta[ptr],&ptr);								// Lenght=1.
		bytePush(0x01,&respuesta[ptr],&ptr);								// Means OK.
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);		
		
		setupIES_Set (&cfg[0]);		
		
	}
	
	if (com==0x02)	//	LEER NS			ENTRA:0		SALE: 4 BYTES
	{
		iesSerialGet(&serial[0]);
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
		bytePush(0x02,&respuesta[ptr],&ptr);										// Cmd Low.
		bytePush(0x03,&respuesta[ptr],&ptr);										// Lenght=4.
		bytePush(serial[0],&respuesta[ptr],&ptr);								// Serial1.
		bytePush(serial[1],&respuesta[ptr],&ptr);								// Serial2.
		bytePush(serial[2],&respuesta[ptr],&ptr);								// Serial3.
		bytePush(serial[3],&respuesta[ptr],&ptr);								// Serial4.
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
	}
	if (com==0x03)	//	GRABAR NS.	ENTRA:4 BYTES		SALE: CONFIRMACION
	{
		iesSerialSet(&recibireth[7]);
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
		bytePush(0x03,&respuesta[ptr],&ptr);										// Cmd Low.
		bytePush(0x00,&respuesta[ptr],&ptr);										// Lenght=1.
		bytePush(0x01,&respuesta[ptr],&ptr);										// Means OK.
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
	}
	if (com==0x04)	//	LEER PARAMETROS PERSIANA.
	{	
		unsigned char blind;
		
		blind=recibireth[7];
		
		if (blind<=5)
		{		
			blindParametersGet (blind, &parameters[0]);				
			ptr=0;		
			bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
			bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
			bytePush(0x04,&respuesta[ptr],&ptr);										// Cmd Low.
			bytePush(0x03,&respuesta[ptr],&ptr);										// Lenght=3.		
			bytePush(recibireth[7],&respuesta[ptr],&ptr);						// blind
			bytePush(parameters[0],&respuesta[ptr],&ptr);						// p0
			bytePush(parameters[1],&respuesta[ptr],&ptr);						// p1
			bytePush(parameters[2],&respuesta[ptr],&ptr);						// p2				
			ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
		}
		else
		{
			for (blind=0;blind<5;blind++)
			{
				blindParametersGet (blind, &parameters[0]);				
				ptr=0;		
				bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
				bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
				bytePush(0x04,&respuesta[ptr],&ptr);										// Cmd Low.
				bytePush(0x03,&respuesta[ptr],&ptr);										// Lenght=3.		
				bytePush(blind,&respuesta[ptr],&ptr);										// blind
				bytePush(parameters[0],&respuesta[ptr],&ptr);						// p0
				bytePush(parameters[1],&respuesta[ptr],&ptr);						// p1
				bytePush(parameters[2],&respuesta[ptr],&ptr);						// p2				
				ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
			}
		}		
	}	
	if (com==0x05)	//	GRABAR PARAMETROS PERSIANA.
	{
		blindParametersSet (recibireth[7], &recibireth[8]);
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
		bytePush(0x05,&respuesta[ptr],&ptr);										// Cmd Low.
		bytePush(0x00,&respuesta[ptr],&ptr);										// Lenght=1.
		bytePush(0x01,&respuesta[ptr],&ptr);										// Means OK.
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
		
		
		
	}
	
	if (com==0x06)	//	LEER POSICION.
	{
		unsigned char blind;	
		unsigned char position;
		blind=recibireth[7];			
		if (blind<=5)
		{		
			position=blindPositionGet (blind);
			ptr=0;		
			bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
			bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
			bytePush(0x06,&respuesta[ptr],&ptr);										// Cmd Low.
			bytePush(0x01,&respuesta[ptr],&ptr);										// Lenght=3.		
			bytePush(blind,&respuesta[ptr],&ptr);										// blind
			bytePush(position,&respuesta[ptr],&ptr);								// position
			ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
		}
		else
		{
			for (blind=0;blind<5;blind++)
			{
				position=blindPositionGet (blind);
				ptr=0;		
				bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
				bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
				bytePush(0x06,&respuesta[ptr],&ptr);										// Cmd Low.
				bytePush(0x01,&respuesta[ptr],&ptr);										// Lenght=3.		
				bytePush(blind,&respuesta[ptr],&ptr);										// blind
				bytePush(position,&respuesta[ptr],&ptr);								// position
				ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
			}
		}
		
		
	}
	
	

	
	
	
	
	
	if (com==0x07)	// 	UP
	{
		unsigned char blind;
		blind=recibireth[7];
		blindOpUp(blind);
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
		bytePush(com,&respuesta[ptr],&ptr);											// Cmd Low.
		bytePush(0x00,&respuesta[ptr],&ptr);										// Lenght=1.
		bytePush(0x01,&respuesta[ptr],&ptr);										// Means OK.
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
	}
	if (com==0x08)	// 	DOWN
	{
		unsigned char blind;
		blind=recibireth[7];
		blindOpDown(blind);
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
		bytePush(com,&respuesta[ptr],&ptr);											// Cmd Low.
		bytePush(0x00,&respuesta[ptr],&ptr);										// Lenght=1.
		bytePush(0x01,&respuesta[ptr],&ptr);										// Means OK.
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
	}
	if (com==0x09)	// 	GOTO POS
	{
		unsigned char blind;
		unsigned char pos;
		blind=recibireth[7];
		pos=recibireth[8];
		blindOpGotoPos (blind,pos);
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
		bytePush(com,&respuesta[ptr],&ptr);											// Cmd Low.
		bytePush(0x00,&respuesta[ptr],&ptr);										// Lenght=1.
		bytePush(0x01,&respuesta[ptr],&ptr);										// Means OK.
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
		
	}
	if (com==0x0A)	// 	STEP UP
	{
		unsigned char blind;
		blind=recibireth[7];
		blindOpStepUp(blind);
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
		bytePush(com,&respuesta[ptr],&ptr);											// Cmd Low.
		bytePush(0x00,&respuesta[ptr],&ptr);										// Lenght=1.
		bytePush(0x01,&respuesta[ptr],&ptr);										// Means OK.
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
	}
	if (com==0x0B)	// 	STEP DOWN
	{
		unsigned char blind;
		blind=recibireth[7];
		blindOpStepDown(blind);
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
		bytePush(com,&respuesta[ptr],&ptr);											// Cmd Low.
		bytePush(0x00,&respuesta[ptr],&ptr);										// Lenght=1.
		bytePush(0x01,&respuesta[ptr],&ptr);										// Means OK.
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
	}
	if (com==0x0C)	// 	STOP
	{
		unsigned char blind;
		blind=recibireth[7];
		blindOpStop(blind);
		ptr=0;		
		bytePush(0x83,&respuesta[ptr],&ptr);										// Frame Type.
		bytePush(0xA0,&respuesta[ptr],&ptr);										// Cmd High.
		bytePush(com,&respuesta[ptr],&ptr);											// Cmd Low.
		bytePush(0x00,&respuesta[ptr],&ptr);										// Lenght=1.
		bytePush(0x01,&respuesta[ptr],&ptr);										// Means OK.
		ethSendFrame(0,&respuesta[0],ptr,&ipDest[0], 2234);			
	}
}