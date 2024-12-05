#include "qSN.h"
#include "dali.h"
#include "ethFramming.h"

#define NOT_SENDING 	0
#define SENDING 			1
#define SENT 					2

unsigned char 		_qSNReadPending[7];
unsigned char    	_qSNDTRSent;
unsigned char    	_qSNDTR1Sent;
unsigned char    	_qSNReadMemoryLocationSent;
unsigned char    	_qSNReadMemoryLocationValid;
unsigned char    	_qSNReadMemoryLocationAnswer;
unsigned char			_qSNResult[6];

static void qSNDTRSend (unsigned char value);
static void qSNDTR1Send (unsigned char value);
static void qSNReadMemoryLocationSend (unsigned char address);




void qSNDoTask (void)
{

}



static void qSNDTRSend (unsigned char value)
{
	
}

static void qSNDTR1Send (unsigned char value)
{
	
}

static void qSNReadMemoryLocationSend (unsigned char dest)
{
	
}

void qSNDTRCb (unsigned char daliSentDest)
{

}

void qSNDTR1Cb (unsigned char daliSentDest)
{

}


void qSNReadMemoryLocationCb (unsigned char daliSentCommandLow, unsigned char daliSentReplyValid, unsigned char daliSentReplyAnswer)
{

}

void qSNRead (unsigned char address, unsigned char *origen)
{
}
	





