		#include "dali.h"
		


		#define NOK         				0x00
		#define OK         					0x01
		#define NA         					0xFF		
		#define ERR_NE       				0x00
		#define ERR_TOO_MUCH				0x01		
		#define ERR_TIMEOUT					0x02			
		#define ALL_DEVICES         0x00
		#define NEW_DEVICES         0xFF
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

unsigned char								_dASEOpSRAReintentosCnt;

unsigned char 					_dASEStatus;
unsigned char 					_dASEStage;


unsigned short int			_dASETimeout;
		
		
	extern unsigned char 					_dASRAStatus;			
	extern unsigned char					_dASRAResult;			
	extern unsigned char 					_dASRAEncontradosCnt;
		
						
	extern unsigned char 					_dAQCGLStatus;	
	extern unsigned char 					_dAPSAStatus;	
		
	extern unsigned char 					_dASRAEncontradosCnt;                                           // Salida
  extern unsigned char 					_dASRAEncontradosList[4][64];                                   // Salida		
		
	extern unsigned char					_dAQCGLDireccionesOcupadasMatriz[64];


static void dASEGotoStage (unsigned char stage)
{

}

void dASERtcTick (void)
{

}

BOOL dASEStart (void)
{
	unsigned char retVal;
	return retVal;
}

void dASEStop (void)
{
}

void dASEDoTask (void)
{		

}


