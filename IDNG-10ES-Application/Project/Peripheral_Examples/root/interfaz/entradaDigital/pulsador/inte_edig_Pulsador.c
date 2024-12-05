//#include <avr/io.h>
//#include "defines.h"
#include "inte_edig_Pulsador.h"

#include "../../interfaz.h"





unsigned char				_pulsadorFlancoAscendenteUsed[10];
unsigned char				_pulsadorNivelAltoLargoUsed[10];
//unsigned char			_pulsadorTxPendingFlag;
unsigned char				_pulsadorShortPressFlag[10];
unsigned char				_pulsadorLongPressFlag[10];
unsigned char				_pulsadorLongReleaseFlag[10];
unsigned char				_pulsadorTimInitEnabled;
unsigned char				_pulsadorTimInitOverflow;
unsigned short int	_pulsadorTimInitCnt;

static void		pulsadorShortPress (unsigned char input)
{
	if (_pulsadorTimInitOverflow==0x01)
	{
		_pulsadorLongPressFlag[input]=0;
		_pulsadorLongReleaseFlag[input]=0;
		_pulsadorShortPressFlag[input]=1;		
		interfazEventoGenerar(REGISTRO_ESTADO_BIT_SHORT_PRESS ,input);
	}
}

static void		pulsadorLongPress (unsigned char input)
{
	if (_pulsadorTimInitOverflow==0x01)
	{
		_pulsadorShortPressFlag[input]=0;
		_pulsadorLongReleaseFlag[input]=0;
		_pulsadorLongPressFlag[input]=1;
		interfazEventoGenerar(REGISTRO_ESTADO_BIT_LONG_PRESS ,input);
		
	}
}
static void		pulsadorLongRelease (unsigned char input)
{
	if (_pulsadorTimInitOverflow==0x01)
	{
		_pulsadorShortPressFlag[input]=0;
		_pulsadorLongPressFlag[input]=0;
		_pulsadorLongReleaseFlag[input]=1;
		interfazEventoGenerar(REGISTRO_ESTADO_BIT_LONG_RELEASE ,input);
	}
}
void			pulsadorFlancoAscendenteHandler (unsigned char input) 
{
	if (_pulsadorFlancoAscendenteUsed[input]==0x01)	pulsadorShortPress(input);
}
void			pulsadorFlancoDescendenteHandler (unsigned char input, unsigned char nivelAltoLargo)
{	
	if (_pulsadorNivelAltoLargoUsed[input]==0x01)
	{		
		if (nivelAltoLargo==0x01)
		{		
			pulsadorLongRelease(input);	
		}
		else
		{
			if (_pulsadorFlancoAscendenteUsed[input]==0x00)	pulsadorShortPress(input);			
		}
	}
	else
	{		
		if (_pulsadorFlancoAscendenteUsed[input]==0x00)	pulsadorShortPress(input);
	}		
}
void			pulsadorNivelAltoLargoHandler (unsigned char input)
{
	if (_pulsadorNivelAltoLargoUsed[input]==0x01)		pulsadorLongPress(input);	
}
void			pulsadorTick (void)
{
	if (_pulsadorTimInitEnabled==0x01)
	{
		if  (_pulsadorTimInitCnt>0)
		{
			_pulsadorTimInitCnt--;
		}
		else
		{
			_pulsadorTimInitOverflow=0x01;
		}
	}
}



void			pulsadorInit (unsigned char input, unsigned char modo)
{
	if (modo==MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE)
	{
		_pulsadorFlancoAscendenteUsed[input]=0x01;
	}
	else
	{
		_pulsadorFlancoAscendenteUsed[input]=0x00;		
	}	
	if (modo==MODO_INTERFAZ_PULSADOR_SHORT_LONG)
	{
		_pulsadorNivelAltoLargoUsed[input]=0x01;
	}
	else
	{
		_pulsadorNivelAltoLargoUsed[input]=0x00;
	}	
	
	// >> Timer Init.
		_pulsadorTimInitEnabled=0x01;
		_pulsadorTimInitCnt=PUL_TIM_INIT+ (input*PUL_TIM_INIT_INC);
	// << Timer Init.
	
	// >> Flags Init.
		//_pulsadorTxPendingFlag=0x00;
		_pulsadorShortPressFlag[input]=0x00;
		_pulsadorLongPressFlag[input]=0x00;
		_pulsadorLongReleaseFlag[input]=0x00;
	// << Flags Init.
}

//unsigned char	pulsadorTxPendingGet (void)
//{
//	return _pulsadorTxPendingFlag;
//}
unsigned char	pulsadorStatusRegisterGet (unsigned char input)
{
	unsigned char registroEstado=0x00;	
	
	if (_pulsadorShortPressFlag[input]==0x01)
	{
		registroEstado=registroEstado+REGISTRO_ESTADO_BIT_SHORT_PRESS;
		_pulsadorShortPressFlag[input]=0x00;
	}	
	if (_pulsadorLongPressFlag[input]==0x01)
	{
		registroEstado=registroEstado+REGISTRO_ESTADO_BIT_LONG_PRESS;
		_pulsadorLongPressFlag[input]=0x00;
	}
	if (_pulsadorLongReleaseFlag[input]==0x01)
	{
		registroEstado=registroEstado+REGISTRO_ESTADO_BIT_LONG_RELEASE;
		_pulsadorLongReleaseFlag[input]=0x00;
	}		
	return registroEstado;
}
