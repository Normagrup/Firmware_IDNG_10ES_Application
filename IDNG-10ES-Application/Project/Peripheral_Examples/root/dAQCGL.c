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
#define BOOL                unsigned char						

// External.
extern unsigned char 		_dAQueryCGLoopSent;
// Internal.
unsigned char 					_dAQCGLStatus;			
unsigned char						_dAQCGLDireccionesOcupadasMatriz[64];
unsigned char						_dAQCGLDireccionesOcupadasCnt;
unsigned char 					_dAQCGLStatus;		
unsigned char						_dAQCGLTimeout;
unsigned char						_dAQCGLResult;
unsigned char						_dAQCGLExitCode;

void dAQCGLDoneFlagClear (void)
{
	_dAQCGLStatus=NOT_STARTED;
}		
void dAQCGLRtcTick(void)
{
}

BOOL dAQCGLStart (void)
{
	BOOL retVal;		
	
	return retVal;
}						
						
void dAQCGLDoTask(void)
{

}
