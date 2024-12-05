#include "normalink.h"
#include "config.h"
#include "rtcc.h"
#include "config.h"
#include "historico.h"
#include "storage.h"
#include "ethFramming.h"

extern unsigned char recibireth[320];		// Búffer para paquete entrante
extern unsigned char ipDest[4];	//Ip del host que me envía el paquete
extern unsigned int portDest;		//Puerto del host que me envía el paquete
extern unsigned char macAdrress[6];


void normaLinkCfgInfoPasarelaT4(unsigned char _comL)
{
	unsigned char _tipoDeFrame;
	unsigned char _data[16];
	unsigned char _respuesta[272];
	unsigned char _lenght;
	
	_lenght=recibireth[6];
	
	
	if ((_comL==0x01) && (_lenght==0x03))
	{
		unsigned char ipAddress[4];
		_data[3]=recibireth[7];
		_data[2]=recibireth[8];
		_data[1]=recibireth[9];
		_data[0]=recibireth[10];				
		ipAddress[0]=_data[3];
		ipAddress[1]=_data[2];
		ipAddress[2]=_data[1];
		ipAddress[3]=_data[0];		
		configIpAddressSet(&ipAddress[0], 0x01);
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;			
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);		
	}
	
	if ((_comL==0x03) && (_lenght==0x03))
	{
		unsigned char subMask[4];
		_data[3]=recibireth[7];
		_data[2]=recibireth[8];
		_data[1]=recibireth[9];
		_data[0]=recibireth[10];		
		subMask[0]=_data[3];
		subMask[1]=_data[2];
		subMask[2]=_data[1];
		subMask[3]=_data[0];			
		configSubmaskAddressSet(&subMask[0], 0x01);
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;		
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);				
	}
	
	if ((_comL==0x05) && (_lenght==0x03))
	{
		unsigned char gtwAddress[4];
		_data[3]=recibireth[7];
		_data[2]=recibireth[8];
		_data[1]=recibireth[9];
		_data[0]=recibireth[10];		
		gtwAddress[0]=_data[3];
		gtwAddress[1]=_data[2];
		gtwAddress[2]=_data[1];
		gtwAddress[3]=_data[0];		
		configGatewayAddressSet(&gtwAddress[0], 0x01);
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);				
	}
	
	if ((_comL==0x11) && (_lenght==0x0F))
	{
		unsigned char nombreEdificio[16];
		_data[15]=recibireth[7];
		_data[14]=recibireth[8];
		_data[13]=recibireth[9];
		_data[12]=recibireth[10];
		_data[11]=recibireth[11];
		_data[10]=recibireth[12];
		_data[9]=recibireth[13];
		_data[8]=recibireth[14];
		_data[07]=recibireth[15];
		_data[06]=recibireth[16];
		_data[05]=recibireth[17];
		_data[04]=recibireth[18];
		_data[03]=recibireth[19];
		_data[02]=recibireth[20];		
		_data[1]=recibireth[21];
		_data[0]=recibireth[22];			
		nombreEdificio[0]=_data[15];
		nombreEdificio[1]=_data[14];
		nombreEdificio[2]=_data[13];
		nombreEdificio[3]=_data[12];
		nombreEdificio[4]=_data[11];
		nombreEdificio[5]=_data[10];
		nombreEdificio[6]=_data[9];
		nombreEdificio[7]=_data[8];
		nombreEdificio[8]=_data[7];
		nombreEdificio[9]=_data[6];
		nombreEdificio[10]=_data[5];
		nombreEdificio[11]=_data[4];
		nombreEdificio[12]=_data[3];
		nombreEdificio[13]=_data[2];
		nombreEdificio[14]=_data[1];
		nombreEdificio[15]=_data[0];
		configNombreEdificioSet(&nombreEdificio[0], 0x01);
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;		
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);		
	}
	
	if ((_comL==0x21) && (_lenght==0x0F))
	{
		unsigned char nombreLinea[16];		
		_data[15]=recibireth[7];
		_data[14]=recibireth[8];
		_data[13]=recibireth[9];
		_data[12]=recibireth[10];
		_data[11]=recibireth[11];
		_data[10]=recibireth[12];
		_data[9]=recibireth[13];
		_data[8]=recibireth[14];
		_data[07]=recibireth[15];
		_data[06]=recibireth[16];
		_data[05]=recibireth[17];
		_data[04]=recibireth[18];
		_data[03]=recibireth[19];
		_data[02]=recibireth[20];		
		_data[1]=recibireth[21];
		_data[0]=recibireth[22];			
		nombreLinea[0]=_data[15];
		nombreLinea[1]=_data[14];
		nombreLinea[2]=_data[13];
		nombreLinea[3]=_data[12];
		nombreLinea[4]=_data[11];
		nombreLinea[5]=_data[10];
		nombreLinea[6]=_data[9];
		nombreLinea[7]=_data[8];
		nombreLinea[8]=_data[7];
		nombreLinea[9]=_data[6];
		nombreLinea[10]=_data[5];
		nombreLinea[11]=_data[4];
		nombreLinea[12]=_data[3];
		nombreLinea[13]=_data[2];
		nombreLinea[14]=_data[1];
		nombreLinea[15]=_data[0];
		configNombreLineaSet(&nombreLinea[0], 0x01);
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;		
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);				
	}
	
		if ((_comL==0x31) && (_lenght==0x03))
		{
			_data[3]=recibireth[7];
			_data[2]=recibireth[8];
			_data[1]=recibireth[9];
			_data[0]=recibireth[10];					
			rtccWeekdaySet(_data[3]);
			rtccDaySet(_data[2]);
			rtccMonthSet(_data[1]);
			rtccYearSet(_data[0]);
			_tipoDeFrame=0x82;			
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x01;
			_respuesta[4]=0x00;
			ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);					
		}
	
	if ((_comL==0x33) && (_lenght==0x02))
		{
			_data[2]=recibireth[7];
			_data[1]=recibireth[8];
			_data[0]=recibireth[9];					
			rtccHourSet(_data[2]);
			rtccMinutesSet(_data[1]);
			rtccSecondsSet(_data[0]);
			_tipoDeFrame=0x82;		
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x01;
			_respuesta[4]=0x00;			
			ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);				
		}
	
	if ((_comL==0x51) && (_lenght==0x0F))
	{
		unsigned char passAdmin[16];		
		_data[15]=recibireth[7];
		_data[14]=recibireth[8];
		_data[13]=recibireth[9];
		_data[12]=recibireth[10];
		_data[11]=recibireth[11];
		_data[10]=recibireth[12];
		_data[9]=recibireth[13];
		_data[8]=recibireth[14];
		_data[07]=recibireth[15];
		_data[06]=recibireth[16];
		_data[05]=recibireth[17];
		_data[04]=recibireth[18];
		_data[03]=recibireth[19];
		_data[02]=recibireth[20];		
		_data[1]=recibireth[21];
		_data[0]=recibireth[22];			
		passAdmin[0]=_data[15];
		passAdmin[1]=_data[14];
		passAdmin[2]=_data[13];
		passAdmin[3]=_data[12];
		passAdmin[4]=_data[11];
		passAdmin[5]=_data[10];
		passAdmin[6]=_data[9];
		passAdmin[7]=_data[8];
		passAdmin[8]=_data[7];
		passAdmin[9]=_data[6];
		passAdmin[10]=_data[5];
		passAdmin[11]=_data[4];
		passAdmin[12]=_data[3];
		passAdmin[13]=_data[2];
		passAdmin[14]=_data[1];
		passAdmin[15]=_data[0];
		configAdminPassSet(&passAdmin[0], 0x01);		
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;		
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
	
	if ((_comL==0x55) && (_lenght==0x0F))
	{
		unsigned char passMantenedor[16];		
		_data[15]=recibireth[7];
		_data[14]=recibireth[8];
		_data[13]=recibireth[9];
		_data[12]=recibireth[10];
		_data[11]=recibireth[11];
		_data[10]=recibireth[12];
		_data[9]=recibireth[13];
		_data[8]=recibireth[14];
		_data[07]=recibireth[15];
		_data[06]=recibireth[16];
		_data[05]=recibireth[17];
		_data[04]=recibireth[18];
		_data[03]=recibireth[19];
		_data[02]=recibireth[20];		
		_data[1]=recibireth[21];
		_data[0]=recibireth[22];			
		passMantenedor[0]=_data[15];
		passMantenedor[1]=_data[14];
		passMantenedor[2]=_data[13];
		passMantenedor[3]=_data[12];
		passMantenedor[4]=_data[11];
		passMantenedor[5]=_data[10];
		passMantenedor[6]=_data[9];
		passMantenedor[7]=_data[8];
		passMantenedor[8]=_data[7];
		passMantenedor[9]=_data[6];
		passMantenedor[10]=_data[5];
		passMantenedor[11]=_data[4];
		passMantenedor[12]=_data[3];
		passMantenedor[13]=_data[2];
		passMantenedor[14]=_data[1];
		passMantenedor[15]=_data[0];
		configMantenedorPassSet(&passMantenedor[0], 0x01);
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;		
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);				
	}
	
	if ((_comL==0x84) && (_lenght==0x01))
	{
		unsigned char _dataRx[2];		
		unsigned char _dataTx[16];		
		_dataRx[1]=recibireth[7];
		_dataRx[0]=recibireth[8];		
		 if (historicoRead (((_dataRx[1]*256)+_dataRx[0]), &_dataTx[0])==0x01)
		 {
			_tipoDeFrame=0x83;
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x08;
			_respuesta[4]=_dataTx[0];
			_respuesta[5]=_dataTx[1];
			_respuesta[6]=_dataTx[2];
			_respuesta[7]=_dataTx[3];
			_respuesta[8]=_dataTx[4];
			_respuesta[9]=_dataTx[5];
			_respuesta[10]=_dataTx[6];
			_respuesta[11]=_dataTx[7];
			_respuesta[12]=_dataTx[8];
			 ethSendFrame(0,&_respuesta[0],13,&ipDest[0], portDest);			
		}
		 else
		 {			 
			_tipoDeFrame=0x82;	
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x00;
			_respuesta[4]=0x00;
			 ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);					
		 }
		
	}	
	if ((_comL==0xA3) && (_lenght==0x00))	// Leer
	{
		unsigned char readBuffer[256];
		unsigned char dataRx[2];	
		unsigned int n;		
		dataRx[0]=recibireth[7];
		if (storageRead (&readBuffer[0], (dataRx[0]+1))==0x01)
		{
			_tipoDeFrame=0x83;
			
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=dataRx[0];
			for (n=0;n<=dataRx[0];n++)
			{
				_respuesta[(4+n)]=readBuffer[n];
			}		
			ethSendFrame(0,&_respuesta[0],(dataRx[0]+5),&ipDest[0], portDest);			
		}
		else
		{
			_tipoDeFrame=0x82;
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x00;
			_respuesta[4]=0x00;		
			ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);		
		}
	}	
	
	if (_comL==0xA4)	// Escribir
	{		
		unsigned char dataRx[256];	
		unsigned int n;
	
		for (n=0;n<=_lenght;n++)
		{
			dataRx[n]=recibireth[7+n];			
		}
		
		if (storageWrite (&dataRx[0],(_lenght+1))==0x01)
		{
			_tipoDeFrame=0x82;

			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x01;
			_respuesta[4]=0x00;
			ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);					
		}
		else
		{
			_tipoDeFrame=0x82;
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x00;
			_respuesta[4]=0x00;
			ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);		
		}		
	}		
		if (((_comL==0xB1) || (_comL==0xC1))  && (_lenght==0xFF))	// Escribir en bootloader
	{
			unsigned char dataRx[256];	
			unsigned int n;
	
			for (n=0;n<=256;n++)
			{
				dataRx[n]=recibireth[7+n];			
			}						
			if (_comL==0xB1)		embeddedBootloaderWrite (&dataRx[0]);
			if (_comL==0xC1)		eBootloaderTransceiver_Write (&dataRx[0]);
			
			_tipoDeFrame=0x82;
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x01;
			_respuesta[4]=0x00;
			ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);	
	}
	
	if (_comL==0xB4) 	// Go
	{
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[4]=0x00;					
			if (embeddedBootloaderGo(recibireth[7],recibireth[8])==0x01)
			{
				_respuesta[3]=0x01;
			}
			else
			{
				_respuesta[3]=0x00;
			}

		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
}




void normaLinkCfgInfoPasarelaT0(unsigned char _comL)
{
	unsigned char _respuesta[21];
	unsigned char _tipoDeFrame;
	unsigned char _lenght;
	unsigned char _data[16];
	
	unsigned char ipAddress[4];
	unsigned char submaskAddress[4];
	unsigned char gatewayAddress[4];
	
	unsigned char nombreEdificio[16];
	unsigned char nombreLinea[16];
	unsigned char passAdmin[16];
	unsigned char passMantenedor[16];	
	
	if (_comL==0x00)
	{
		_tipoDeFrame=0x83;
		_lenght=0x03;		
		configIpAddressGet(&ipAddress[0]);
		_data[3]=ipAddress[0];
		_data[2]=ipAddress[1];
		_data[1]=ipAddress[2];
		_data[0]=ipAddress[3];
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[3];
		_respuesta[5]=_data[2];
		_respuesta[6]=_data[1];
		_respuesta[7]=_data[0];
		ethSendFrame(0,&_respuesta[0],8,&ipDest[0], portDest);				
	}
	
	if (_comL==0x02)
	{
		_tipoDeFrame=0x83;
		_lenght=0x03;		
		configSubmaskAddressGet(&submaskAddress[0]);
		_data[3]=submaskAddress[0];
		_data[2]=submaskAddress[1];
		_data[1]=submaskAddress[2];
		_data[0]=submaskAddress[3];
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[3];
		_respuesta[5]=_data[2];
		_respuesta[6]=_data[1];
		_respuesta[7]=_data[0];		
		ethSendFrame(0,&_respuesta[0],8,&ipDest[0], portDest);						
	}
	
	if (_comL==0x04)
	{
		_tipoDeFrame=0x83;
		_lenght=0x03;		
		configGatewayAddressGet(&gatewayAddress[0]);
		_data[3]=gatewayAddress[0];
		_data[2]=gatewayAddress[1];
		_data[1]=gatewayAddress[2];
		_data[0]=gatewayAddress[3];
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[3];
		_respuesta[5]=_data[2];
		_respuesta[6]=_data[1];
		_respuesta[7]=_data[0];
		ethSendFrame(0,&_respuesta[0],8,&ipDest[0], portDest);				
	}
	
	if (_comL==0x08)
	{
		_tipoDeFrame=0x83;
		_lenght=0x05;
		_data[5]=macAdrress[0];
		_data[4]=macAdrress[1];
		_data[3]=macAdrress[2];
		_data[2]=macAdrress[3];
		_data[1]=macAdrress[4];
		_data[0]=macAdrress[5];
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[5];
		_respuesta[5]=_data[4];
		_respuesta[6]=_data[3];
		_respuesta[7]=_data[2];
		_respuesta[8]=_data[1];
		_respuesta[9]=_data[0];			
		ethSendFrame(0,&_respuesta[0],10,&ipDest[0], portDest);				
	}
	
	if (_comL==0x10)
	{
		_tipoDeFrame=0x83;
		_lenght=0x0F;		
		configNombreEdificioGet(&nombreEdificio[0]);
		_data[15]=nombreEdificio[0];
		_data[14]=nombreEdificio[1];
		_data[13]=nombreEdificio[2];
		_data[12]=nombreEdificio[3];
		_data[11]=nombreEdificio[4];
		_data[10]=nombreEdificio[5];
		_data[9]=nombreEdificio[6];
		_data[8]=nombreEdificio[7];
		_data[7]=nombreEdificio[8];
		_data[6]=nombreEdificio[9];
		_data[5]=nombreEdificio[10];
		_data[4]=nombreEdificio[11];
		_data[3]=nombreEdificio[12];
		_data[2]=nombreEdificio[13];
		_data[1]=nombreEdificio[14];
		_data[0]=nombreEdificio[15];
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[15];
		_respuesta[5]=_data[14];
		_respuesta[6]=_data[13];
		_respuesta[7]=_data[12];
		_respuesta[8]=_data[11];
		_respuesta[9]=_data[10];		
		_respuesta[10]=_data[9];		
		_respuesta[11]=_data[8];		
		_respuesta[12]=_data[7];		
		_respuesta[13]=_data[6];		
		_respuesta[14]=_data[5];		
		_respuesta[15]=_data[4];		
		_respuesta[16]=_data[3];		
		_respuesta[17]=_data[2];		
		_respuesta[18]=_data[1];		
		_respuesta[19]=_data[0];		
		ethSendFrame(0,&_respuesta[0],20,&ipDest[0], portDest);						
	}
	
	if (_comL==0x20)
	{
		_tipoDeFrame=0x83;
		_lenght=0x0F;		
		configNombreLineaGet(&nombreLinea[0]);		
		_data[15]=nombreLinea[0];
		_data[14]=nombreLinea[1];
		_data[13]=nombreLinea[2];
		_data[12]=nombreLinea[3];
		_data[11]=nombreLinea[4];
		_data[10]=nombreLinea[5];
		_data[9]=nombreLinea[6];
		_data[8]=nombreLinea[7];
		_data[7]=nombreLinea[8];
		_data[6]=nombreLinea[9];
		_data[5]=nombreLinea[10];
		_data[4]=nombreLinea[11];
		_data[3]=nombreLinea[12];
		_data[2]=nombreLinea[13];
		_data[1]=nombreLinea[14];
		_data[0]=nombreLinea[15];
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[15];
		_respuesta[5]=_data[14];
		_respuesta[6]=_data[13];
		_respuesta[7]=_data[12];
		_respuesta[8]=_data[11];
		_respuesta[9]=_data[10];		
		_respuesta[10]=_data[9];		
		_respuesta[11]=_data[8];		
		_respuesta[12]=_data[7];		
		_respuesta[13]=_data[6];		
		_respuesta[14]=_data[5];		
		_respuesta[15]=_data[4];		
		_respuesta[16]=_data[3];		
		_respuesta[17]=_data[2];		
		_respuesta[18]=_data[1];		
		_respuesta[19]=_data[0];	
		ethSendFrame(0,&_respuesta[0],20,&ipDest[0], portDest);								
	}
	
	if (_comL==0x30)
	{
		_tipoDeFrame=0x83;
		_lenght=0x03;
		_data[3]=rtccWeekdayGet();
		_data[2]=rtccDayGet();
		_data[1]=rtccMonthGet();
		_data[0]=rtccYearGet();
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[3];
		_respuesta[5]=_data[2];
		_respuesta[6]=_data[1];
		_respuesta[7]=_data[0];		
		ethSendFrame(0,&_respuesta[0],8,&ipDest[0], portDest);	
	}
	
	if (_comL==0x32)
	{
		_tipoDeFrame=0x83;
		_lenght=0x02;
		_data[2]=rtccHourGet();
		_data[1]=rtccMinutesGet();
		_data[0]=rtccSecondsGet();
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[2];
		_respuesta[5]=_data[1];
		_respuesta[6]=_data[0];
		ethSendFrame(0,&_respuesta[0],7,&ipDest[0], portDest);	
	}
	

	if (_comL==0x40)
	{
		_tipoDeFrame=0x83;
		_lenght=0x01;
		_data[0]=0;	// MODEL_HH
		_data[1]=4;	// MODEL_LOW
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[0];
		_respuesta[5]=_data[1];
		ethSendFrame(0,&_respuesta[0],6,&ipDest[0], portDest);	
		
	}
	
	if (_comL==0x42)
	{
		_tipoDeFrame=0x83;
		_lenght=0x01;
		_data[0]=4;	// FW_HIGH
		_data[1]=6;	// FW_LOW
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[0];
		_respuesta[5]=_data[1];
		ethSendFrame(0,&_respuesta[0],6,&ipDest[0], portDest);	
	}
	

		
	if (_comL==0x50)
	{
		_tipoDeFrame=0x83;
		_lenght=0x0F;		
		configAdminPassGet(&passAdmin[0]);
		_data[15]=passAdmin[0];
		_data[14]=passAdmin[1];
		_data[13]=passAdmin[2];
		_data[12]=passAdmin[3];
		_data[11]=passAdmin[4];
		_data[10]=passAdmin[5];
		_data[9]=passAdmin[6];
		_data[8]=passAdmin[7];
		_data[7]=passAdmin[8];
		_data[6]=passAdmin[9];
		_data[5]=passAdmin[10];
		_data[4]=passAdmin[11];
		_data[3]=passAdmin[12];
		_data[2]=passAdmin[13];
		_data[1]=passAdmin[14];
		_data[0]=passAdmin[15];
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[15];
		_respuesta[5]=_data[14];
		_respuesta[6]=_data[13];
		_respuesta[7]=_data[12];
		_respuesta[8]=_data[11];
		_respuesta[9]=_data[10];		
		_respuesta[10]=_data[9];		
		_respuesta[11]=_data[8];		
		_respuesta[12]=_data[7];		
		_respuesta[13]=_data[6];		
		_respuesta[14]=_data[5];		
		_respuesta[15]=_data[4];		
		_respuesta[16]=_data[3];		
		_respuesta[17]=_data[2];		
		_respuesta[18]=_data[1];		
		_respuesta[19]=_data[0];		
		ethSendFrame(0,&_respuesta[0],20,&ipDest[0], portDest);	
	}
	
	if (_comL==0x54)
	{
		_tipoDeFrame=0x83;
		_lenght=0x0F;		
		configMantenedorPassGet(&passMantenedor[0]);
		_data[15]=passMantenedor[0];
		_data[14]=passMantenedor[1];
		_data[13]=passMantenedor[2];
		_data[12]=passMantenedor[3];
		_data[11]=passMantenedor[4];
		_data[10]=passMantenedor[5];
		_data[9]=passMantenedor[6];
		_data[8]=passMantenedor[7];
		_data[7]=passMantenedor[8];
		_data[6]=passMantenedor[9];
		_data[5]=passMantenedor[10];
		_data[4]=passMantenedor[11];
		_data[3]=passMantenedor[12];
		_data[2]=passMantenedor[13];
		_data[1]=passMantenedor[14];
		_data[0]=passMantenedor[15];
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[15];
		_respuesta[5]=_data[14];
		_respuesta[6]=_data[13];
		_respuesta[7]=_data[12];
		_respuesta[8]=_data[11];
		_respuesta[9]=_data[10];		
		_respuesta[10]=_data[9];		
		_respuesta[11]=_data[8];		
		_respuesta[12]=_data[7];		
		_respuesta[13]=_data[6];		
		_respuesta[14]=_data[5];		
		_respuesta[15]=_data[4];		
		_respuesta[16]=_data[3];		
		_respuesta[17]=_data[2];		
		_respuesta[18]=_data[1];		
		_respuesta[19]=_data[0];		
		ethSendFrame(0,&_respuesta[0],20,&ipDest[0], portDest);	
		
	}
	
	if (_comL==0x80)		// Obtiene el numero de eventos leibles
	{
		unsigned int _historicoCnt;
		_tipoDeFrame=0x83;
		_lenght=0x01;				
		_historicoCnt=historicoReadableEventsCntGet();		
		_data[1]=(unsigned char)(_historicoCnt/0x0100);
		_data[0]=(unsigned char)(_historicoCnt&0x00FF);
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[1];
		_respuesta[5]=_data[0];		
		ethSendFrame(0,&_respuesta[0],6,&ipDest[0], portDest);		
	}
	
	if (_comL==0x88) 
	{
		historicoReset();
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}		
	
	if (_comL==0xA0)	// Iniciar R
	{
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[4]=0x00;		
		if (storageReadStart()==0x01)
		{
			_respuesta[3]=0x01;
		}
		else
		{
			_respuesta[3]=0x00;
		}	
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);	
	}	
	
	if (_comL==0xA1)	// Iniciar W
	{	
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[4]=0x00;
		if (storageWriteStart()==0x01)
		{
			_respuesta[3]=0x01;
		}
		else
		{
			_respuesta[3]=0x00;
		}
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}		
	
	if (_comL==0xA2)	// Obtener ID de Modificaciones
	{
		unsigned int storageId;				
		_tipoDeFrame=0x83;
		_lenght=0x01;				
		storageId=storageIdGet();
		_data[1]=(unsigned char)(storageId/0x0100);
		_data[0]=(unsigned char)(storageId&0x00FF);
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[1];
		_respuesta[5]=_data[0];
		ethSendFrame(0,&_respuesta[0],6,&ipDest[0], portDest);			
	}		
	
	if (_comL==0xA6)	// Finalizar R/W
	{
		storageReadWriteFinish();
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}	
	
		if (_comL==0xA6)	// Finalizar R/W
	{
		storageReadWriteFinish();
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}	
	
	if (_comL==0xB0) 	// Iniciar Embedded Bootloader Write
	{
		embeddedBootloaderWriteStart();
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}	
	
	if (_comL==0xB2)	// Iniciar Embedded Bootloader Read
	{
		embeddedBootloaderReadStart();
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}	
	
	if (_comL==0xB3)	// Leer
	{
		unsigned char readBuffer[256];
		unsigned int n;		
		embeddedBootloaderRead(&readBuffer[0]);		
		_tipoDeFrame=0x83;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0xFF;
		for (n=0;n<256;n++)
		{
			_respuesta[(4+n)]=readBuffer[n];
		}	
		ethSendFrame(0,&_respuesta[0],261,&ipDest[0], portDest);					
	}	
	


}


