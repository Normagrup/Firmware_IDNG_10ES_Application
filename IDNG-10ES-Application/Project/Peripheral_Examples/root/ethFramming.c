#include "normaLink62386P102.h"
#include "normaLink62386P202.h"
#include "normaLinkCfgInfoPasarela.h"
#include "normaLinkSingleLoop.h"
#include "normaLinkCmdPasarela.h"
#include "normaLinkDaliUtils.h"
#include "normalink.h"
#include "w5100/socket.h"
#include "w5100/w5100.h"

#define PORT 2234

// Received packet
unsigned char ipDest[4];	//Ip del host que me env�a el paquete
unsigned int portFrom;		//Puerto del host que me env�a el paquete
unsigned int portDest=PORT;		//Puerto del host que al que enviar� el paquete
unsigned int packetSize;		//Tama�o del paquete recibido
unsigned char packetHeader[8];		// Cabecera del paquete
unsigned char recibireth[320];		// B�ffer para paquete entrante


extern unsigned char ethTransceiverEstado;

extern unsigned char _configIpAddress[4];
extern unsigned char _configSubmaskAddress[4];

void ethSendFrame(unsigned char socket, unsigned char *buffer, unsigned int frameLenght, unsigned char *address, unsigned int port)
{
	unsigned char frame[300];
	unsigned int n;
	unsigned char crc=0;
	frame[0]=0x54;
	frame[1]=0x43;
	frame[2]=0x97;
	for (n=0;n<frameLenght;n++)
	{
		frame[n+3]=buffer[n];
		crc+=buffer[n];
	}	
	frame[frameLenght+3]=crc;	
	sendto(socket,&frame[0],(frameLenght+4),&address[0],port);		// Send Answer if needed				
}

unsigned char ethProcessFrame (unsigned char *recibireth, unsigned int rxPacketSize)
{
	unsigned char _dest;
	unsigned char _data;		
	unsigned char _comL;
	unsigned char _comH;
	unsigned char _tipoDeFrame;
	_tipoDeFrame=recibireth[0];	
	if (_tipoDeFrame==0x00)
	{
		_comH=recibireth[4];
		_comL=recibireth[5];			
		
		if (_comH==0x00)
		{
			normaLinkCfgInfoPasarelaT0(_comL);				
		}			
		
		if (_comH==0x01)
		{
			normaLinkCmdPasarelaT0(_comL);				
		}			
	
		if (_comH==0x40)
		{
			normaLinkSingleLoopT0(_comL);				
		}		
	}
	
	
	if (_tipoDeFrame==0x01)
	{
		_dest=recibireth[4];	
		_comH=recibireth[5];
		_comL=recibireth[6];		
		if (normaLinkDestCorrecto(_dest)==0x01)
		{		
			if (_comH==0x63)
			{
				normaLink62386P102T1(_comL, _dest);	
			}
			
			if (_comH==0x64)
			{
				//normaLink62386P202T1(_comL, _dest);	
			}
			
			if (_comH==0xE4)
			{
				normaLinkDaliUtilsT1(_comL, _dest);	
			}
		}	
	}
	
	if (_tipoDeFrame==0x03)
	{
		_dest=recibireth[4];	
		_comH=recibireth[5];
		_comL=recibireth[6];				
		_data=recibireth[7];	
		if (normaLinkDestCorrecto(_dest)==0x01)
		{		
			if (_comH==0x63)
			{
					normaLink62386P102T3(_comL,_dest, _data);
			}
			
			if (_comH==0xE4)
			{
				normaLinkDaliUtilsT3(_comL, _dest,_data);	
			}
		}	
	}

	if (_tipoDeFrame==0x05)
	{
		_comH=recibireth[4];
		_comL=recibireth[5];			
		
		if (_comH==0x01)		normaLinkCmdPasarelaT5(_comL);
	}
	
	
	if (_tipoDeFrame==0x04)
	{
		_comH=recibireth[4];
		_comL=recibireth[5];			
		
		if (_comH==0x00)
		{
			normaLinkCfgInfoPasarelaT4(_comL);				
		}					
		
		
		
		
		
		if (_comH==0x40)
		{
			normaLinkSingleLoopT4(_comL);				
		}					
		
		if (_comH==0xE4)
		{
			normaLinkDaliUtilsT4(_comL);	
		}
		
		if (_comH==0x77)
		{
			normaLinkSensores(_comL);	
		}
		
		if (_comH==0xA0)
		{
			normaLinkIES(_comL);
		}		
		
		if ((_comH==0x78) && (_comL==0x01))
		{
			normaLinkMasterPing(&ipDest[0]);
		}
		
	}		
	return 1;	
}


void ethDoTask (void)
{
			unsigned int rsize=0;
			unsigned char rawEth[512];
			unsigned int n;
			unsigned char crc;
	
			if (getSn_SR(0)==SOCK_UDP)
			{				
				rsize=getSn_RX_RSR(0);	
			if (rsize>8)
			{
				ethTransceiverTiempoSinRecibirCntRst();
				packetHeader[0]=0;
				packetHeader[1]=0;
				packetHeader[2]=0;
				packetHeader[3]=0;
				packetHeader[5]=0;
				packetHeader[6]=0;
				packetHeader[7]=0;
				recv(0,packetHeader,8);																	// Get packetHeader
				ipDest[0]=packetHeader[0];															// Get ip
				ipDest[1]=packetHeader[1];
				ipDest[2]=packetHeader[2];
				ipDest[3]=packetHeader[3];
				portFrom=(packetHeader[4]*256)+packetHeader[5];					// Get port
				packetSize=(packetHeader[6]*256)+packetHeader[7];				// Get packet size
				
				recv(0,rawEth,packetSize);															// Get packet
				
				#ifdef IGNORE_SUBNET_FILTER
					if ((rawEth[0]==0x54) && (rawEth[1]==0x43) && (rawEth[2]==0x97))
				#else
					if ((rawEth[0]==0x54) && (rawEth[1]==0x43) && (rawEth[2]==0x97) &&	((_configIpAddress[0]&_configSubmaskAddress[0])==(ipDest[0]&_configSubmaskAddress[0])) && ((_configIpAddress[1]&_configSubmaskAddress[1])==(ipDest[1]&_configSubmaskAddress[1])) &&	((_configIpAddress[2]&_configSubmaskAddress[2])==(ipDest[2]&_configSubmaskAddress[2])) && ((_configIpAddress[3]&_configSubmaskAddress[3])==(ipDest[3]&_configSubmaskAddress[3])))
				#endif
				{
						crc=0;
						for (n=0;n<(packetSize-4);n++)
						{
							recibireth[n]=rawEth[n+3];
							crc+=recibireth[n];							
						}
						
						//if (crc==rawEth[packetSize-1])
						//{
							ethProcessFrame(&recibireth[0],(packetSize-4));
						//}
						//else
						//{
							//CRC incorrecto
						//}						
				}
				else
				{
					//Preambulo Incorrecto
				}
			}
			// End of Ethernet Processing
		}
			else
			{
				if (ethTransceiverEstado==0x05)	
				{
					ethTransceiverEstado=0x02;
					//ethTransceiverW5100Init();	
				}
				
			}
}

