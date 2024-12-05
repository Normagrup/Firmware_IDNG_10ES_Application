	#include "dali.h"
	#include "config.h"

#define NOT_SENDING         0x00
#define SENDING             0x01
#define SENT                0x02
#define TRUE                0x01
#define FALSE               0x00
#define NOT_STARTED         0x00
#define IN_PROGRESS         0x01
#define DONE                0x02
#define BOOL                unsigned char	
	
#define STAGE_0             0x00
#define STAGE_1             0x01
#define STAGE_2             0x02
#define STAGE_3             0x03
#define STAGE_4             0x04
	
extern unsigned char _dAQCGLDireccionesOcupadasMatriz[64];
extern unsigned char _dAQCGLDireccionesOcupadasCnt;
extern unsigned char _dAQDTLTiposDispositivoMatriz[64];
extern unsigned char _dAQDTLTiposDispositivoValid[8];
extern unsigned char _dAQCGLStatus;
extern unsigned char _dAQDTLStatus;
unsigned char 			 _dAQGroupsLowStatus;
unsigned char 			 _dAQGroupsHighStatus;
unsigned char				 _dALSStatus;
unsigned char				 _dALSStage;



	
void dALSDoTask(void)
{
		
}
	
	
	
BOOL dALSStart(void)
{
	unsigned char retVal;	
	return retVal;	
}
	
	
	
void dALSDoneFlagClear (void)
{
	_dALSStatus=NOT_STARTED;	
}