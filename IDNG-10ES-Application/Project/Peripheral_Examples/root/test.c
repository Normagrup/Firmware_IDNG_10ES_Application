#include "daliCommands.h"
#include "dali.h"
#include "daliLoop.h"
#include "rtcc.h"
#include "m25p.h"
#include "test.h"
#include "config.h"

volatile unsigned char testOverflow;

void testDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray)									
{
}







void testSet (unsigned char grupo, unsigned char activo, unsigned char *testTime)
{
}

unsigned char testGet (unsigned char grupo, unsigned char *testTime)
{
		return 0x01;
}


void testRTCC_Tick (void)
{
}




void testInit (void)
{
}

