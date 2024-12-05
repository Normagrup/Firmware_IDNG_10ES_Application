// 0 		nop
// 1		off
// 2 		recall max level
// 3 		recall min level
// 4 		goto scene  0
// 5 		goto scene  1
// 6 		goto scene  2
// 7 		goto scene  3
// 8 		goto scene	4
// 9 		goto scene	5
// 10 	goto scene	6
// 11 	goto scene	7
// 12 	goto scene	8
// 13 	goto scene	9
// 14 	goto scene	10
// 15 	goto scene	11
// 16 	goto scene	12
// 17 	goto scene	13
// 18 	goto scene	14
// 19 	goto scene	15

#include "temp.h"

unsigned char tempStartPendiente[16];
unsigned char tempStopPendiente[16];
unsigned char tempStartPendienteCnt[16];
unsigned char tempStopPendienteCnt[16];
unsigned int tempStartPendientePacketId[16];
unsigned int tempStopPendientePacketId[16];
unsigned int tempPacketId;
volatile unsigned char tempOverflow;
const unsigned char tempActionDaliComand[20]={0x91,0x01,0x06,0x07,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20};

void tempDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray)									
{
	unsigned char _n;
	for (_n=0;_n<16;_n++)
	{
		if ((tempStartPendiente[_n]==0x01) && (tempStartPendientePacketId[_n]==daliSentSenderPacketId))
		{
			(tempStartPendiente[_n]=0x00);
		}
	}
	
	for (_n=0;_n<16;_n++)
	{
		if ((tempStopPendiente[_n]==0x01) && (tempStopPendientePacketId[_n]==daliSentSenderPacketId))
		{
			(tempStopPendiente[_n]=0x00);
		}
	}	
}

unsigned char tempPacketIdGet(void)
{
	return tempPacketId;
}


void tempDoTask (void)
{
	unsigned char _n;
	unsigned char apply[8];
	unsigned char tempData[11];
	if (tempOverflow==0x01)
	{
	for (_n=0;_n<16;_n++)
	{
		configTempGet (_n, &tempData[0]);

		if (tempStartPendiente[_n]==0x01)
		{
			if (tempStartPendienteCnt[_n]>0)
			{			
				if ((((tempData[TEMP_START_ACTION_DEST]&0x01)==0x01) &&	(tempData[TEMP_START_ACTION_COMAND]!=0x00) && (tempData[TEMP_START_ACTION_COMAND]<=19)) || (((tempData[TEMP_START_ACTION_DEST]&0x01)==0x00) && (tempData[TEMP_START_ACTION_COMAND]!=0xFF)))
				{
					tempStartPendientePacketId[_n]=tempPacketIdGet();
					if (tempData[TEMP_START_ACTION_DEST]&0x01==0x01)
					{
						idngIesCom(tempData[TEMP_START_ACTION_DEST], tempActionDaliComand[tempData[TEMP_START_ACTION_COMAND]]);
						//if (daliSend(DALI_TEMP, tempStartPendientePacketId[_n], 0x01, tempData[TEMP_START_ACTION_DEST], &apply[0], 0x2B, tempActionDaliComand[tempData[TEMP_START_ACTION_COMAND]])==0x01)
						//{				
						//}
						//else
						//{
						//}	
					}
					else
					{
							if (tempActionDaliComand[tempData[TEMP_START_ACTION_COMAND]]!=0xff)
							{
							if (tempActionDaliComand[tempData[TEMP_START_ACTION_COMAND]]==0)
							{
							idngIesCom((tempData[TEMP_START_ACTION_DEST]+1), 0x01);
							}
							else
							{
							idngIesCom((tempData[TEMP_START_ACTION_DEST]+1), 0x06);
							}
							}						
					}
					tempStartPendienteCnt[_n]--;						
				}
				else
				{					
					tempStartPendiente[_n]=0;
				}					
			}
			else
			{
				// No se ha podido enviar.
				tempStartPendiente[_n]=0;
			}	
		}
		
		if (tempStopPendiente[_n]==0x01)
		{
			if (tempStopPendienteCnt[_n]>0)
			{			
					if ((((tempData[TEMP_STOP_ACTION_DEST]&0x01)==0x01) &&	(tempData[TEMP_STOP_ACTION_COMAND]!=0x00) && (tempData[TEMP_STOP_ACTION_COMAND]<=19)) || (((tempData[TEMP_STOP_ACTION_DEST]&0x01)==0x00) && (tempData[TEMP_STOP_ACTION_COMAND]!=0xFF)))
					{
						tempStopPendientePacketId[_n]=tempPacketIdGet();
									
						if ((tempData[TEMP_STOP_ACTION_DEST]&0x01)==0x01)
						{
							idngIesCom(tempData[TEMP_STOP_ACTION_DEST], tempActionDaliComand[tempData[TEMP_STOP_ACTION_COMAND]]);
							//if (daliSend(DALI_TEMP, tempStopPendientePacketId[_n], 0x01, tempData[TEMP_STOP_ACTION_DEST], &apply[0], 0x2B, tempActionDaliComand[tempData[TEMP_STOP_ACTION_COMAND]])==0x01)
							//{				
							//}
							//else
							//{
							//}
						}
						else
						{
							if (tempActionDaliComand[tempData[TEMP_STOP_ACTION_COMAND]]!=0xff)
							{
							if (tempActionDaliComand[tempData[TEMP_STOP_ACTION_COMAND]]==0)
							{
							idngIesCom((tempData[TEMP_STOP_ACTION_DEST]+1), 0x01);
							}
							else
							{
							idngIesCom((tempData[TEMP_STOP_ACTION_DEST]+1), 0x06);
							}
							}
							
							
							//if (daliSend(DALI_TEMP, tempStopPendientePacketId[_n], 0x01, tempData[TEMP_STOP_ACTION_DEST], &apply[0], 0x2B, tempData[TEMP_STOP_ACTION_COMAND])==0x01)
							//{				
							//}
							//else
							//{
							//}						
						}
						tempStopPendienteCnt[_n]--;						
					}
					else
					{					
						tempStopPendiente[_n]=0;
					}					
			}
			else
			{
				// No se ha podido enviar.
				tempStopPendiente[_n]=0;
			}	
		}
	}	
	tempOverflow=0x00;
	}
	tempPacketId++;	
}










void tempSet (unsigned char temp, unsigned char activo, unsigned char *tempDataIn)
{
	unsigned char tempData[11];	
	if (temp<16)
	{	
		tempData[TEMP_ACTIVO]=activo;
		tempData[TEMP_START_WEEKDAY]=tempDataIn[0]&0xFE;
		tempData[TEMP_START_HOUR]=tempDataIn[1];
		tempData[TEMP_START_MINUTES]=tempDataIn[2];
		tempData[TEMP_START_ACTION_DEST]=(tempDataIn[3]);
		tempData[TEMP_START_ACTION_COMAND]=tempDataIn[4];
		tempData[TEMP_STOP_WEEKDAY]=tempDataIn[5]&0xFE;
		tempData[TEMP_STOP_HOUR]=tempDataIn[6];
		tempData[TEMP_STOP_MINUTES]=tempDataIn[7];
		tempData[TEMP_STOP_ACTION_DEST]=(tempDataIn[8]);
		tempData[TEMP_STOP_ACTION_COMAND]=tempDataIn[9];
		configTempSet(temp,&tempData[0],0x01);
	}	
}

unsigned char tempGet (unsigned char temp, unsigned char *tempDataRet)
{
	unsigned char tempData[11];	
	if (temp<16)
	{
		configTempGet(temp,&tempData[0]);		
		if (tempData[TEMP_ACTIVO]==0x01)
		{
			tempDataRet[0]=tempData[TEMP_START_WEEKDAY];
			tempDataRet[1]=tempData[TEMP_START_HOUR];
			tempDataRet[2]=tempData[TEMP_START_MINUTES];
			tempDataRet[3]=tempData[TEMP_START_ACTION_DEST];
			tempDataRet[4]=tempData[TEMP_START_ACTION_COMAND];
			tempDataRet[5]=tempData[TEMP_STOP_WEEKDAY];
			tempDataRet[6]=tempData[TEMP_STOP_HOUR];
			tempDataRet[7]=tempData[TEMP_STOP_MINUTES];
			tempDataRet[8]=tempData[TEMP_STOP_ACTION_DEST];
			tempDataRet[9]=tempData[TEMP_STOP_ACTION_COMAND];
			return 0x01;
		}	
		else
		{
			tempDataRet[0]=tempData[TEMP_START_WEEKDAY];
			tempDataRet[1]=tempData[TEMP_START_HOUR];
			tempDataRet[2]=tempData[TEMP_START_MINUTES];
			tempDataRet[3]=tempData[TEMP_START_ACTION_DEST];
			tempDataRet[4]=tempData[TEMP_START_ACTION_COMAND];
			tempDataRet[5]=tempData[TEMP_STOP_WEEKDAY];
			tempDataRet[6]=tempData[TEMP_STOP_HOUR];
			tempDataRet[7]=tempData[TEMP_STOP_MINUTES];
			tempDataRet[8]=tempData[TEMP_STOP_ACTION_DEST];
			tempDataRet[9]=tempData[TEMP_STOP_ACTION_COMAND];			
			return 0x00;
		}
	}
	else
	{
		return 0x00;
	}
}

void tempInit (void)
{
}


void tempCheck (void)
{
	unsigned char _rtccWeekday;
	unsigned char _rtccHour;
	unsigned char _rtccMinutes;
	unsigned char _n;	
	unsigned char tempData[11];
	_rtccWeekday=rtccWeekdayGet();
	_rtccHour=rtccHourGet();
	_rtccMinutes=rtccMinutesGet();		
	for (_n=0;_n<16;_n++)
	{
		configTempGet(_n,&tempData[0]);		
		if (tempData[TEMP_ACTIVO]==0x01)
		{
			if ((BITVAL(tempData[TEMP_START_WEEKDAY],_rtccWeekday)==0x01) && (_rtccHour==tempData[TEMP_START_HOUR]) && (_rtccMinutes==tempData[TEMP_START_MINUTES]))
			{
				tempStartPendiente[_n]=0x01;
				tempStartPendienteCnt[_n]=1;
			}
			
			if ((BITVAL(tempData[TEMP_STOP_WEEKDAY],_rtccWeekday)==0x01) && (_rtccHour==tempData[TEMP_STOP_HOUR]) && (_rtccMinutes==tempData[TEMP_STOP_MINUTES]))
			{
				tempStopPendiente[_n]=0x01;
				tempStopPendienteCnt[_n]=1;
			}		
		}	
	}		
	tempOverflow=0x01;
}








