

#include "../../interfaz.h"
#include "inte_edig_DetectorPresencia.h"


unsigned char _detPresFlancoUsed[10];
unsigned char _detPresNivelUsed[10];
unsigned char _detPresTxPendingFlag;
unsigned char _detPresPresencyFlag[10];
unsigned char _detPresVacancyFlag[10];

// >> Variables relacionadas con Timer de envío del Keep Alive. Funcionará en los modos por nivel.
	unsigned char	_detPresTimKeepAliveEnabled;
	unsigned char	_detPressTimKeepAliveOverflow;
	unsigned short int		_detPresTimKeepAliveCnt;
// << Variables relacionadas con Timer de envío del Keep Alive. Funcionará en los modos por nivel.

// >> Variables relacionadas con Timer de inicialización. Funcionará en todos los casos.
	unsigned char	_detPresTimInitEnabled;
	unsigned char	_detPresTimInitOverflow;
	unsigned short int		_detPresTimInitCnt;
// << Variables relacionadas con Timer de inicialización. Funcionará en todos los casos.




void			detPresFlancoAscendenteHandler (unsigned char input)
{	
	if (_detPresFlancoUsed[input]==0x01)
	{
		detPresPresency(input);	
	}	
}
void			detPresNivelAltoHandler (unsigned char input)
{
	if (_detPresNivelUsed[input]==0x01)
	{
		detPresPresency(input);
	}
}
void			detPresNivelBajoHandler (unsigned char input)
{
	if (_detPresNivelUsed[input]==0x01)
	{
		detPresVacancy(input);
	}		
}
void			detPresInit (unsigned char input, unsigned char modo)
{
	if ((modo==MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO) || (modo==MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_LUZ))
	{
		_detPresFlancoUsed[input]=0x01;
	}
	else
	{
		_detPresFlancoUsed[input]=0x00;
	}
	if ((modo==MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL) || (modo==MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_LUZ))
	{			
		_detPresTimKeepAliveEnabled=0x01;
		_detPresTimKeepAliveCnt=DPR_TIM_KEEP_ALIVE_CNT_STARTUP+(input*DPR_TIM_KEEP_ALIVE_CNT_STARTUP_INC);
		_detPresNivelUsed[input]=0x01;		
	}
	else
	{
		_detPresTimKeepAliveEnabled=0x00;
		_detPresTimKeepAliveCnt=0xFFFF;			
		_detPresNivelUsed[input]=0x00;
	}			
	_detPresTimInitEnabled=0x01;
	_detPresTimInitCnt=DPR_TIM_INIT+ (input*DPR_TIM_INIT_INC);	
	_detPresTxPendingFlag=0x00;
	_detPresPresencyFlag[input]=0x00;
	_detPresVacancyFlag[input]=0x00;
}
void			detPresTick(void)
{
	// >> Keep Alive Timer.
		if (_detPresTimKeepAliveEnabled==0x01)
		{
			if  (_detPresTimKeepAliveCnt>0)
			{
				_detPresTimKeepAliveCnt--;
			}
			else
			{
				_detPressTimKeepAliveOverflow=0x01;
			}
		}
	// << Keep Alive Timer.
	
	// >> Initialisation Timer.
		if (_detPresTimInitEnabled==0x01)
		{
			if  (_detPresTimInitCnt>0)
			{
				_detPresTimInitCnt--;
			}
			else
			{
				_detPresTimInitOverflow=0x01;
			}
		}
	// >> Initialisation Timer.
}
unsigned char	detPresTxPendingGet (void)
{
	if ((_detPresTxPendingFlag==0x01) || (_detPressTimKeepAliveOverflow==0x01))
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}	
}
unsigned char	detPresStatusRegisterGet (void)
{	
	unsigned char registroEstado=0;	
	/*
	if (_detPresPresencyFlag==1)
	{
		registroEstado=registroEstado+REGISTRO_ESTADO_BIT_PRESENCY;			
		if (_detPresFlancoUsed==0x01)	_detPresPresencyFlag=0;	
	}	
	if (_detPresVacancyFlag==1)
	{
		registroEstado=registroEstado+REGISTRO_ESTADO_BIT_VACANCY;
	}
	_detPresTxPendingFlag=0;	
	_detPressTimKeepAliveOverflow=0;	
	if (_detPresTimKeepAliveEnabled==0x01)		_detPresTimKeepAliveCnt=DPR_TIM_KEEP_ALIVE_CNT_PRELOAD;
	*/
	return registroEstado;	
}
void			detPresPresency (unsigned char input)
{
	_detPresVacancyFlag[input]=0;
	if (_detPresPresencyFlag[input]==0)
	{
		_detPresPresencyFlag[input]=1;			
		//if (_detPresTimInitOverflow==0x01)		_detPresTxPendingFlag=1;		
		interfazEventoGenerar(REGISTRO_ESTADO_BIT_PRESENCY ,input);
	}			
}
void			detPresVacancy (unsigned char input)
{
	_detPresPresencyFlag[input]=0;	
	if (_detPresVacancyFlag[input]==0)
	{
		_detPresVacancyFlag[input]=1;
		//if (_detPresTimInitOverflow==0x01)		_detPresTxPendingFlag=1;
		interfazEventoGenerar(REGISTRO_ESTADO_BIT_VACANCY ,input);
	}
}
