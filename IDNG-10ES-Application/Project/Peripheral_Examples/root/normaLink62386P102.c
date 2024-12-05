#include "normalink.h"
#include "daliLoop.h"
#include "systick.h"
#include "dataTypes.h"

extern unsigned char recibireth[320];		// Búffer para paquete entrante
extern unsigned char ipDest[4];	//Ip del host que me envía el paquete

extern unsigned char _configDaliLoopGroupLow[64];
extern unsigned char _configDaliLoopGroupHigh[64];
extern unsigned char _dtr[11];


void normaLink62386P102T1(unsigned char _comL, unsigned char _dest)
{
	unsigned int _packetId;	
	unsigned char n;
	unsigned char respuesta[5];	
	_packetId=recibireth[2]*0x0100+recibireth[3];
	
	// [001] OFF
	if (_comL==0x01) 
	{					
		idngIesCom(_dest, _comL);		
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;	
		respuesta[4]=0x00;	
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}				
	// [006] RECALL MAX LEVEL
	if (_comL==0x06) 
	{
		idngIesCom(_dest, _comL);
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=0x00;	
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}				
	// [007] RECALL MIN LEVEL
	if (_comL==0x07)
	{				
		idngIesCom(_dest, _comL);
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=0x00;	
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}
			
	// [009] ON AND STEP UP
	if (_comL==0x09)
	{
		idngIesCom(_dest, _comL);
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=0x00;	
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}
	
	// [017] GOTO SCENE 00 - GOTO SCENE 15
	if ((_comL>=0x11) && (_comL<=0x20))
	{	
		idngIesCom(_dest, _comL);
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=0x00;	
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}
		


	
	// [046] STORE DTR AS POWER ON LEVEL
	if (_comL==0x2E)
	{	
		idngIesCom(_dest, _comL);
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=0x00;	
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}
			

	// [065] STORE DTR AS SCENE 00 - 15
	if ((_comL>=0x41) && (_comL<=0x50))
	{	
		idngIesCom(_dest, _comL);
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=0x00;	
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}
			
	// [081] REMOVE FROM SCENE 00 - 15
	if ((_comL>=0x51) && (_comL<=0x60))
	{	
		idngIesCom(_dest, _comL);
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=0x00;	
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}
	

	
	// [097] ADD TO GROUP 00
	if ((_comL>=0x61) && (_comL<=0x70))
	{	
		idngIesCom(_dest, _comL);
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=0x00;	
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}				
	// [113] REMOVE FROM GROUP 00
	if ((_comL>=0x71) && (_comL<=0x80))
	{				
		idngIesCom(_dest, _comL);
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=0x00;	
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}					
	// [164] QUERY POWER ON LEVEL
	if (_comL==0xA4)
	{				
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=idngIesCom(_dest, _comL);
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}			
	
	// [177] QUERY SCENE LEVEL 00
	if ((_comL>=0xB1) && (_comL==0xC0))
	{
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=idngIesCom(_dest, _comL);
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}
		
	// [193] QUERY GROUPS 0-7
	if (_comL==0xC1)
	{
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=_configDaliLoopGroupLow[((_dest&0x7E)>>1)];
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}						
			
	// [194] QUERY GROUPS 8-15
	if (_comL==0xC2)
	{
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=_configDaliLoopGroupHigh[((_dest&0x7E)>>1)];
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}								

	
}
void normaLink62386P102T3(unsigned char _comL, unsigned char _dest, unsigned char _data)
{
	unsigned int _packetId;
	unsigned char respuesta[5];
	unsigned char n;
	_packetId=recibireth[2]*0x0100+recibireth[3];	
	
	// [000] DIRECT ARC POWER CONTROL
	if (_comL==0)
	{		
		if (_data!=0xFF)
		{			
			if (_data==0x00)
			{				
				idngIesCom(_dest, 0x01);
			}
			else
			{
				idngIesCom(_dest, 0x06);
			}			
		}
	}							
			
	// [227] DTR
	if ((_comL==0xE3) && (_dest==0xFF))
	{		
		unsigned char n;
		
		for (n=0;n<11;n++)	_dtr[n]=_data;
		
		
		respuesta[0]=0x82;
		respuesta[1]=(_packetId/0x0100);
		respuesta[2]=(_packetId&0x00FF);	
		respuesta[3]=0x01;
		respuesta[4]=0;
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);	
	}			
}


