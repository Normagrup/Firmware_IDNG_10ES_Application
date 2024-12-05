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
#define STAGE_2             0x02
#define STAGE_3             0x03
#define STAGE_4             0x04
#define STAGE_5             0x05
#define STAGE_6             0x06
#define STAGE_7             0x07
#define STAGE_8             0x08
#define STAGE_9             0x09
#define STAGE_10            0x0A
#define STAGE_11            0x0B
#define STAGE_12            0x0C
#define STAGE_13            0x0D
#define STAGE_14            0x0E
#define STAGE_15            0x0F




#define BOOL                unsigned char	

// Externas.

extern unsigned char		_dATerminateSent;
extern unsigned char		_dAInitializeSent;
extern unsigned char		_dASearchAddrHSent;
extern unsigned char		_dASearchAddrMSent;
extern unsigned char		_dASearchAddrLSent;
extern unsigned char		_dAQuerySAddrSent;
extern unsigned char 		_dAProgramSAddrSent;

extern BOOL							_dAQuerySAddrReplyValid;

extern unsigned char 									_dASRAEncontradosList[4][64];                                   // Salida		

extern unsigned char									_dAQCGLDireccionesOcupadasMatriz[64];


// Internas.





unsigned char   				_dAPSAStage;
unsigned char   				_dAPSAInitializeMode;																						// Varible utilizada para el algoritmo de búsqueda.
//unsigned char						_dAPSAGlobalProcTimeout;
//unsigned char						_dAPSADeviceProcTimeout;    
unsigned char   				_dAPSAStatus;																										// Salida
unsigned char   				_dAPSAResult;																										// Salida	
unsigned char   				_dAPSAExitCode;																									// Salida	
unsigned char						_dAPSAEquiposCnt;
unsigned char						_dAPSAEquiposCntLimit;
unsigned char						_dAPSADireccionesLibresList[64];															// Aquí grabarías en otro formato. ej 0,2,3,4,FF,FF,FF
unsigned char						_dAPSAReintentosCnt;

unsigned char dASRAEncontradosHGet(unsigned char equipo);
unsigned char dASRAEncontradosMGet(unsigned char equipo);
unsigned char dASRAEncontradosLGet(unsigned char equipo);
				
static void dAPSAGotoStage (unsigned char stage)
{	
}

void dAPSARtcTick (void)
{
}

BOOL dAPSAStart (unsigned char initializeMode, unsigned char equiposCntLimit)
{	
	
}

void dAPSAStop (void)
{	
}

void dAPSADoTask (void)
{	
}

void dAPSADoneFlagClear (void)
{
}