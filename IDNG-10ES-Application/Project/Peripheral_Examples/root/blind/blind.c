//  >> BLIND MOV.

#define	BLIND_MOV_DOWN										0x00
#define	BLIND_MOV_UP											0x01



#define BLIND_MOV_STATUS_STOPPED					0x00
#define BLIND_MOV_STATUS_UP								0x01
#define BLIND_MOV_STATUS_DOWN							0x02
//  << BLIND MOV.

//  >> BLIND OP.
#define BLIND_OP_TYPE_NOP									0x00
#define BLIND_OP_TYPE_UP									0x01
#define BLIND_OP_TYPE_DOWN								0x02
#define BLIND_OP_TYPE_GOTO_POS						0x03
#define BLIND_OP_TYPE_STEP_UP							0x04
#define BLIND_OP_TYPE_STEP_DOWN						0x05
//  << BLIND OP.

//  >> BLIND MOV.
unsigned char 				_blindMovStatus[5];														// BLIND_MOV_STATUS_STOPPED,BLIND_MOV_STATUS_UP,BLIND_MOV_STATUS_DOWN
unsigned short int 		_blindMovTimeout[5];													// {0-2540}
unsigned char					_blindMovStartUpRequestFlag[5];
unsigned char					_blindMovStartDownRequestFlag[5];
unsigned char					_blindMovStopRequestFlag[5];
unsigned short int		_blindMovRequestTimeout[5];
unsigned char					_blindMovTimerOverflow;
//  >> BLIND MOV.

//  >> BLIND OP.
unsigned char					_blindOpType[5];
unsigned short int		_blindOpGotoPos[5]={0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};
unsigned char					_blindOpFlags[5];
unsigned char					_blindOpPendingType[5];
//unsigned char					_blindOpTimerOverflow;
//  << BLIND OP.

// >> BLIND.
unsigned short int		_blindPeriod[5]={100,100,100,100,100};
unsigned char					_blindStep[5]={22,22,22,22,22};
unsigned short int		_blindPosition[5]={0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};


// << BLIND.


unsigned char _blindOpCal[5];


unsigned char _blindUsed[5];
unsigned char _blindPin[11];


extern unsigned char _idngIesCfgPendingCnt;

volatile unsigned short int tmp;

//volatile float tmpFloat;

//  >> BLIND MOV.
void blindMovTick (void);
void blindMovDoTask (void);
//  >> BLIND MOV.

//  >> BLIND OP.
void blindOpTick (void);
void blindOpDoTask (void);
//  >> BLIND OP.


void blindOpGotoPos(unsigned char blind, unsigned char pos);

//  >> BLIND.

unsigned char blindPosDec (unsigned short int pos16, unsigned short int period);


void blindParametersSet (unsigned char blind, unsigned char *parameters)
{
	_blindPeriod[blind]=(256*parameters[0])+parameters[1];
	_blindStep[blind]=parameters[2];	
	_idngIesCfgPendingCnt=3;
}

void blindParametersGet (unsigned char blind, unsigned char *parameters)
{
	parameters[0]=(unsigned char)(_blindPeriod[blind]>>8);
	parameters[1]=(unsigned char)(_blindPeriod[blind]&0x00FF);
	parameters[2]=_blindStep[blind];	
}

unsigned char blindPositionGet (unsigned char blind)
{
		return blindPosDec (_blindPosition[blind], _blindPeriod[blind]);
}


void blindTick (void)
{
	blindMovTick();
	blindOpTick();
}
void blindDoTask (void)
{
	blindMovDoTask();
	blindOpDoTask();
}
unsigned short int blindPosEnc (unsigned char pos8, unsigned short int period)
{		
	float 								tmpFloat;		
	unsigned short int		tmpShort;	
	tmpFloat=((float)(pos8)) * (((float)(period))/254.0);
	tmpShort=(unsigned short int)(tmpFloat);	
	if (tmpFloat-((float)(tmpShort))>0.5) tmpShort++;	
	return tmpShort;
}
unsigned char blindPosDec (unsigned short int pos16, unsigned short int period)
{
	float									tmpFloat;
	unsigned char					tmpChar;
	tmpFloat=(254.0 * (((float)(pos16))/((float)(period))));
	tmpChar=(unsigned char)(tmpFloat);	
	if (tmpFloat-((float)(tmpChar))>0.5) tmpChar++;		
	return tmpChar;	
}
//  >> BLIND.

//  >> BLIND MOV.
void blindMovStart(unsigned char blind, unsigned char sense, unsigned short int timeout)
{
	if (sense==BLIND_MOV_UP)
	{
		_blindMovRequestTimeout[blind]=timeout;
		_blindMovStartUpRequestFlag[blind]=0x01;
		
	}
	else if (sense==BLIND_MOV_DOWN)	
	{
		_blindMovRequestTimeout[blind]=timeout;
		_blindMovStartDownRequestFlag[blind]=0x01;
	}
}

void blindMovStop (unsigned char blind)
{
	_blindMovStopRequestFlag[blind]=0x01;
}
void blindMovTick (void)
{
	_blindMovTimerOverflow=0x01;
}
void blindMovDoTask (void)
{			
	unsigned char n;
	for (n=0;n<5;n++)
	{			
		if ((_blindPosition[n]<_blindOpGotoPos[n]) || (_blindOpCal[n]==1))
		{
			_blindMovStatus[n]=BLIND_MOV_STATUS_UP;
		}
		else if (_blindPosition[n]>_blindOpGotoPos[n])
		{
			_blindMovStatus[n]=BLIND_MOV_STATUS_DOWN;
		}
		else
		{
			_blindMovStatus[n]=BLIND_MOV_STATUS_STOPPED;
		}
	}
	
		//  > >  T I M E R S	
		if (_blindMovTimerOverflow==0x01)
		{	
			for (n=0;n<5;n++)
			{
				if (_blindMovTimeout[n]>0)	
				{
					_blindMovTimeout[n]--;
				}				
				if (_blindMovStatus[n]==BLIND_MOV_STATUS_UP)
				{
					if ((_blindPosition[n]<_blindPeriod[n]) && (_blindPosition[n]!=0xFFFF))
					{
						_blindPosition[n]++;
					}
					else
					{
						_blindOpCal[n]=0;
					}						
				}
				else if (_blindMovStatus[n]==BLIND_MOV_STATUS_DOWN)
				{
					if ((_blindPosition[n]>0) && (_blindPosition[n]!=0xFFFF))
					{
						_blindPosition[n]--;
					}	
				}
				
			}
			_blindMovTimerOverflow=0x00;
		}	
	//  < <  T I M E R S	
	//  > >	 O U T P U T S
		for (n=0;n<5;n++)
		{
			if (_blindMovStatus[n]==BLIND_MOV_STATUS_STOPPED)
			{
				_blindPin[((2*n)+0)]=0;
				_blindPin[((2*n)+1)]=0;
			}
			else if (_blindMovStatus[n]==BLIND_MOV_STATUS_UP)
			{
				_blindPin[((2*n)+0)]=0;
				_blindPin[((2*n)+1)]=1;		
			}
			else if (_blindMovStatus[n]==BLIND_MOV_STATUS_DOWN)
			{
				_blindPin[((2*n)+0)]=1;
				_blindPin[((2*n)+1)]=0;				
			}	
		}
	//  < <	 O U T P U T S			
}
//  >> BLIND MOV.

//  >> BLIND OP.
void blindOpUp(unsigned char blind)
{	
	unsigned char n;
	if (blind==0xFF)
	{
		for (n=0;n<5;n++)		_blindPosition[n]=0;
	}
	else
	{
		_blindPosition[blind]=0;
	}
	blindOpGotoPos(blind, 254);
}
void blindOpDown(unsigned char blind)
{
	unsigned char n;
	if (blind==0xFF)
	{
		for (n=0;n<5;n++)		_blindPosition[n]=_blindPeriod[n];
	}
	else
	{
		_blindPosition[blind]=_blindPeriod[blind];
	}	
	blindOpGotoPos(blind, 0);
}
void blindOpStepUp(unsigned char blind)
{
	unsigned char n;
	if (blind==0xFF)
	{
		for (n=0;n<5;n++)
		{	
			if (_blindUsed[n]==0x01)	
			{	
				_blindOpPendingType[n]=BLIND_OP_TYPE_STEP_UP;
				_blindOpFlags[n]=0x00;
			}
		}
	}
	else
	{
		if (_blindUsed[blind]==0x01)	
		{	
			_blindOpPendingType[blind]=BLIND_OP_TYPE_STEP_UP;
			_blindOpFlags[blind]=0x00;
		}
	}
}
void blindOpStepDown(unsigned char blind)
{
	unsigned char n;
	if (blind==0xFF)
	{
		for (n=0;n<5;n++)
		{	
			if (_blindUsed[n]==0x01)	
			{	
				_blindOpPendingType[n]=BLIND_OP_TYPE_STEP_DOWN;
				_blindOpFlags[n]=0x00;
			}
		}
	}
	else
	{
		if (_blindUsed[blind]==0x01)	
		{	
			_blindOpPendingType[blind]=BLIND_OP_TYPE_STEP_DOWN;
			_blindOpFlags[blind]=0x00;
		}
	}
}
void blindOpGotoPos(unsigned char blind, unsigned char pos)
{	
	unsigned char n;
	if (blind==0xFF)
	{
		for (n=0;n<5;n++)
		{	
			if (_blindUsed[n]==0x01)	
			{	
				if (_blindPosition[n]==0xFFFF)
				{
					if (pos==254)
					{
						_blindPosition[n]=0;
						//_blindOpCal[n]=1;
					}
					else if (pos==0)
					{
						_blindPosition[n]=_blindPeriod[n];
						//_blindOpCal[n]=1;
					}
					else
					{	
						_blindPosition[n]=0;						
						_blindOpCal[n]=1;
					}					
				}
				_blindOpGotoPos[n]=blindPosEnc (pos, _blindPeriod[n]);
			}
		}
	}
	else
	{
		if (_blindUsed[blind]==0x01)	
		{	
			if (_blindPosition[blind]==0xFFFF)
			{
					if (pos==254)
					{
						_blindPosition[blind]=0;
						//_blindOpCal[n]=1;
					}
					else if (pos==0)
					{
						_blindPosition[blind]=_blindPeriod[blind];
						//_blindOpCal[n]=1;
					}
					else
					{	
						_blindPosition[blind]=0;						
						_blindOpCal[blind]=1;
					}			
			}			
			_blindOpGotoPos[blind]=blindPosEnc (pos, _blindPeriod[blind]);

		}
	}
}

void blindOpStop (unsigned char blind)
{
	unsigned char n;
	if (blind==0xFF)
	{
		for (n=0;n<5;n++)
		{	
			if (_blindUsed[n]==0x01)	
			{	
				_blindOpPendingType[n]=BLIND_OP_TYPE_NOP;		
				blindMovStop(n);
				_blindOpFlags[n]=0x00;
			}
		}
	}
	else
	{
		if (_blindUsed[blind]==0x01)	
		{	
			_blindOpPendingType[blind]=BLIND_OP_TYPE_NOP;		
			blindMovStop(blind);
			_blindOpFlags[blind]=0x00;
		}
	}	
}

void blindOpTick (void)
{
}
void blindOpDoTask (void)
{
}



//  << BLIND OP.