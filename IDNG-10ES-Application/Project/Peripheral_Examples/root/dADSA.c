#include "dali.h"

#define NOT_SENDING         0x00
#define SENDING             0x01
#define SENT                0x02
#define TRUE                0x01
#define FALSE               0x00
#define NOT_STARTED         0x00
#define IN_PROGRESS         0x01
#define DONE                0x02
#define BOOL                unsigned char	

// External.
extern unsigned char		_dADTRSent;		
extern unsigned char		_dAStoreDTRAsAddrSent;

// Internal.
unsigned char 					_dADSAStatus;			
unsigned char						_dADSATimeout;


BOOL dADSAStart (void)
{	
	BOOL retVal;
	return retVal;
}


void dADSADoTask (void)
{
}

void dADSADoneFlagClear (void)
{
}
