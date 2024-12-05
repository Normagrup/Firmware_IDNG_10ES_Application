#include "config.h"
#include "rtcc.h"
#include "w5100/socket.h"
#include "w5100/w5100.h"
#include "normalink.h"
#include "dali.h"

unsigned char normaLinkReply;
unsigned int normaLinkPacketId;
unsigned char normaLinkReplyToIp[4];

unsigned char normaLinkDaliBloquearAccesoABusEnabled;
unsigned char normaLinkDaliAccesoExclusivoABusEnabled;
unsigned char normaLinkDaliAccesoExclusivoABusIp[4];
unsigned char normaLinkDaliAccesoExclusivoABusTimeoutCnt;
unsigned char normaLinkDaliBloquearAccesoABusTimeoutCnt;

extern unsigned int portDest;



unsigned char _normaLinkMasterIp[4];

unsigned char _normaLinkMasterPingTicks;

extern unsigned char _broadcastAddress[4];


void normaLinkMasterPing (unsigned char *address)
{
	_normaLinkMasterIp[0]=address[0];
	_normaLinkMasterIp[1]=address[1];
	_normaLinkMasterIp[2]=address[2];
	_normaLinkMasterIp[3]=address[3];
	_normaLinkMasterPingTicks=2;
}

void normaLinkTick (void)
{
	if (_normaLinkMasterPingTicks>0)
	{
		_normaLinkMasterPingTicks--;		
	}
	else
	{
		_normaLinkMasterIp[0]=0;
		_normaLinkMasterIp[1]=0;
		_normaLinkMasterIp[2]=0;
		_normaLinkMasterIp[3]=0;
	}	
}
unsigned char normaLinkDaliAccesoExclusivoABusEnable (unsigned char *address)
{
		return 0x01;
}

void normaLinkDaliBloquearAccesoABusEnable (void)
{

}

void normaLinkDaliAccesoExclusivoABusDisable (void)
{

}

void normaLinkDaliBloquearAccesoABusDisable (void)
{

}






















void normalinkDaliSend (unsigned char *address, unsigned int packetId, unsigned char daliFrameType, unsigned char daliTransceiverDest, unsigned char daliTransceiverCommandHigh, unsigned char daliTransceiverCommandLow)
{
}
void normalinkDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray)									
{
}
unsigned char normaLinkDestCorrecto (unsigned char _dest)
{
	if ((_dest==0xFF) ||	((_dest&0xE1)==0x81) ||	((_dest&0x81)==0x01))
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}	
}

