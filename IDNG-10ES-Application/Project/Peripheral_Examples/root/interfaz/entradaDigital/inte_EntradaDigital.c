//#include "board.h"
//#include "defines.h"
#include "inte_EntradaDigital.h"
//#include "detectoresPresencia.h"

#include "../../board.h"


#include "../interfaz.h"

#include "pulsador/inte_edig_Pulsador.h"

// >> Nuevas variables.
	unsigned char	_entradaDigitalPulsadorUsed[10];
	unsigned char	_entradaDigitalDetPresUsed[10];

	unsigned char	_entradaDigitalNivel[10];
	unsigned char	_entradaDigitalInicioFlanco[10];
	unsigned char	_entradaDigitalNivelAltoLargo[10];
	unsigned int	_entradaDigitalNivelAltoCnt[10];
// << Nuevas variables.

static void entradaDigitalNivelAltoCntTick (void)
{
	unsigned char n;
	for (n=0;n<10;n++)
	{
		if (_entradaDigitalInicioFlanco[n]==0x01)
		{
			if (_entradaDigitalNivelAltoCnt[n]<0xFFFF)
			{
				_entradaDigitalNivelAltoCnt[n]++;
			}
		}
	}
}
static void entradaDigitalFlancoAscendenteHandler(unsigned char input)
{
	if (_entradaDigitalPulsadorUsed[input]==0x01)
	{
		pulsadorFlancoAscendenteHandler(input);
	}	
	if (_entradaDigitalDetPresUsed[input]==0x01)
	{
		detPresFlancoAscendenteHandler(input);
	}
}
static void entradaDigitalFlancoDescendenteHandler(unsigned char input, unsigned char nivelAltoLargo)
{
	if (_entradaDigitalPulsadorUsed[input]==0x01)
	{
		pulsadorFlancoDescendenteHandler(input, _entradaDigitalNivelAltoLargo[input]);
	}
}
static void entradaDigitalNivelAltoHandler(unsigned char input)
{
	if (_entradaDigitalDetPresUsed[input]==0x01)
	{
		detPresNivelAltoHandler(input);
	}
}
static void entradaDigitalNivelBajoHandler(unsigned char input)
{
	if (_entradaDigitalDetPresUsed[input]==0x01)
	{
		detPresNivelBajoHandler(input);
	}
}
static void entradaDigitalNivelAltoLargoHandler (unsigned char input)
{
	if (_entradaDigitalPulsadorUsed[input]==0x01)
	{
		pulsadorNivelAltoLargoHandler(input);
	}
}
/*
unsigned char entradaDigitalTxPendingGet(void)
{
	unsigned char answer=0x00;
	if ((_entradaDigitalPulsadorUsed==0x01) && (pulsadorTxPendingGet()==0x01))
	{
		answer=0x01;
	}
	if ((_entradaDigitalDetPresUsed==0x01) && (detPresTxPendingGet()==0x01))
	{
		answer=0x01;
	}
	return answer;
}
*/


unsigned char entradaDigitalStatusRegisterGet(unsigned char input)
{
	unsigned char answer=0x00;
	if (_entradaDigitalPulsadorUsed[input]==0x01)
	{
		answer=answer+pulsadorStatusRegisterGet(input);
	}
	//if (_entradaDigitalDetPresUsed==0x01)	
	//{
	//	answer=answer+detPresStatusRegisterGet();
	//}
	return answer;
}

void entradaDigitalInit (unsigned char input, unsigned char modo)
{
	if ((modo==MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE) || (modo==MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_DESCENDENTE) || (modo==MODO_INTERFAZ_PULSADOR_SHORT_LONG))
	{
		_entradaDigitalPulsadorUsed[input]=0x01;
		pulsadorInit(input, modo);
	}
	else
	{
		_entradaDigitalPulsadorUsed[input]=0x00;
	}
	
	if ((modo==MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO) || (modo==MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL) || (modo==MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_LUZ) || (modo==MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_LUZ))
	{
		_entradaDigitalDetPresUsed[input]=0x01;
		detPresInit(input, modo);
	}
	else
	{
		_entradaDigitalDetPresUsed[input]=0x00;
	}
}
void entradaDigitalTick (void)
{
	unsigned char input;
	
	//if (_entradaDigitalPulsadorUsed[input]==0x01)
	//{
		pulsadorTick();
	//}
	//if (_entradaDigitalDetPresUsed[input]==0x01)
	//{
		//detPresTick();
	//}	
	entradaDigitalNivelAltoCntTick();	
}


void entradaDigitalDoTask (unsigned char input)
{		
	// >> Análisis entrada digital.	
		if (boardGPIOPinValueGet (input)==0x00)
		{
			// [Entrada digital ACTIVA].
			if (_entradaDigitalInicioFlanco[input]==0x00)
			{
				// [Flanco ascendente].
				entradaDigitalFlancoAscendenteHandler(input);
				_entradaDigitalNivelAltoCnt[input]=0x00;
				_entradaDigitalNivelAltoLargo[input]=0x00;
				_entradaDigitalInicioFlanco[input]=0x01;
			}
				
			if ((_entradaDigitalNivelAltoCnt[input]>NIVEL_ALTO_CNT_LONG_PRESS) && (_entradaDigitalNivelAltoLargo[input]==0x00))
			{					
				entradaDigitalNivelAltoLargoHandler(input);					
				_entradaDigitalNivelAltoLargo[input]=0x01;
			}				
			entradaDigitalNivelAltoHandler(input);
			_entradaDigitalNivel[input]=0x01;
		}
		else
		{
			// [Entrada digital INACTIVA].
			if (_entradaDigitalInicioFlanco[input]==0x01)
			{
				// [Flanco descendente].
				entradaDigitalFlancoDescendenteHandler(input,_entradaDigitalNivelAltoLargo[input]);
				_entradaDigitalInicioFlanco[input]=0x00;
			}
			entradaDigitalNivelBajoHandler(input);
			_entradaDigitalNivel[input]=0x00;
		}			
	// << Análisis entrada digital.
}