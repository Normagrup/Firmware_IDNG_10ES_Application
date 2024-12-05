		#include "dali.h"
				
#define DAQCGL_TIMEOUT			15
#define NOK         				0x00
#define OK         					0x01
#define NA         					0xFF		
#define ERR_TIMEOUT					0x02			
#define TRUE                0x01
#define FALSE               0x00
#define NOT_SENDING         0x00
#define SENDING             0x01
#define SENT                0x02
#define NOT_STARTED         0x00
#define IN_PROGRESS         0x01
#define DONE                0x02

		#define STAGE_0             0x00
		#define STAGE_1             0x01



#define BOOL                unsigned char						

// External.
extern unsigned char 		_dAQueryDTLoopSent;
// Internal.

unsigned char 					_dAQDTLStatus;	
unsigned char						_dAQDTLTiposDispositivoMatriz[64];
unsigned char						_dAQDTLTiposDispositivoValid[8];

unsigned char						*_applyDT;


void dAQDTLDoneFlagClear (void)
{
	_dAQDTLStatus=NOT_STARTED;
}		

BOOL dAQDTLStart (unsigned char *applyDT)
{
	BOOL retVal;		

	return retVal;
}						
						
						
						
						
						
						
						


void dAQDTLDoTask(void)
{
	
}
