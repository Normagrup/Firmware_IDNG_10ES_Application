
#define INC_INTERFACES_CONFIGURACION_S
    #include "interfaces.h"
#undef INC_INTERFACES_CONFIGURACION_S

#define INC_INTERFACES_PRT
    #include "interfaces.h"
#undef INC_INTERFACES_PRT

#include "macros.h"


//#include <avr/io.h>
//#include <avr/eeprom.h>
#include "interfaz.h"
//#include "defines.h"
//#include "entradaDigital.h"
//#include "entradaAnalogica.h"

unsigned char _interfazModo[10];
unsigned char _interfazSubmodo;
unsigned char _interfazDeshabilitado[10];
unsigned char _interfazEntradaDigitalUsed[10];
unsigned char _interfazEntradaAnalogicaUsed;


extern unsigned short int	_idngiesCfg_uso;

// De momento son todas ILU.
//unsigned short int	_idngiesCfg_ilu = 0x07C0;								// 0b11111000000
//unsigned short int	_idngiesCfg_res;
extern unsigned short int	_idngiesCfg_es;
extern unsigned short int	_idngiesCfg_pulShort;
extern unsigned short int	_idngiesCfg_pulShortLong;
extern unsigned short int	_idngiesCfg_int;




/*
unsigned char interfazTxPendingGet (void)
{
	unsigned char answer=0x00;
	if (_interfazDeshabilitado==0x00)
	{
		if ((_interfazEntradaDigitalUsed==0x01) && (entradaDigitalTxPendingGet()==0x01))
		{
			answer=0x01;
		}
		if ((_interfazEntradaAnalogicaUsed==0x01) && (entradaAnalogicaTxPendingGet()==0x01))
		{
			answer=0x01;
		}
	}	
	return answer;
}
*/
/*
unsigned char interfazStatusRegisterGet (void)
{
	unsigned char answer=0x00;
	if (_interfazDeshabilitado==0x00)
	{
		if (_interfazEntradaDigitalUsed==0x01)
		{
			answer=answer+entradaDigitalStatusRegisterGet();
		}
		if (_interfazEntradaAnalogicaUsed==0x01)
		{
			answer=answer+entradaAnalogicaStatusRegisterGet();
		}
	}
	return answer;
}
*/
void interfazTick (void)
{
	// De momento lo dejo así. no sé si hay que hacerlo individualmente para cada entrada o valdría para todas.
	
	//if (_interfazDeshabilitado[n]==0x00)
	//{
		//if (_interfazEntradaDigitalUsed[n]==0x01)
		//{
			entradaDigitalTick();
		//}
		//if (_interfazEntradaAnalogicaUsed==0x01)
		//{
		//	entradaAnalogicaTick();
		//}
	//}
}
/*
void interfazInit(void)
{
	// >> Load values from EEPROM.
		_interfazModo=eeprom_read_byte((uint8_t*)EEPROM_ADDRESS_MODE);
		_interfazSubmodo=eeprom_read_byte((uint8_t*)EEPROM_ADDRESS_SUBMODE);	
		_interfazDeshabilitado=eeprom_read_byte((uint8_t*)EEPROM_ADDRESS_DISABLED);	
	// << Load values from EEPROM.
	
	// >> Modo Init.
		interfazModoUpdt();
	// << Modo Init.
}
*/


void interfazEventoGenerar (unsigned char registroEstado, unsigned char input)
{
	/*
	unsigned char answer=0;
	
	//if (_interfazDeshabilitado==0x00)
	//{
	//	if (_interfazEntradaDigitalUsed==0x01)
	//	{
			answer=answer+entradaDigitalStatusRegisterGet(0);
	//	}
	//	if (_interfazEntradaAnalogicaUsed==0x01)
	//	{
	//		answer=answer+entradaAnalogicaStatusRegisterGet();
	//	}
	//}	
	*/
	
	
	
	interfacesNotificar(input,registroEstado,0xFF);
	
	
}





void interfazInit (void)
{
		unsigned char input;	
		// Dependiendo del esquema.
	
		for (input=0;input<10;input++)
		{
			
			if ((BITVAL(_idngiesCfg_uso,input)==0x01) && (BITVAL(_idngiesCfg_es,input)==0x00) && ((BITVAL(_idngiesCfg_pulShort,input)==0x01) ||	(BITVAL(_idngiesCfg_pulShortLong,input)==0x01)	|| (BITVAL(_idngiesCfg_int,input)==0x01)))
			{
				if (BITVAL(_idngiesCfg_pulShort,input)==0x01)								entradaDigitalInit(input, MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE);
				else if (BITVAL(_idngiesCfg_pulShortLong,input)==0x01)			entradaDigitalInit(input, MODO_INTERFAZ_PULSADOR_SHORT_LONG);
				else if (BITVAL(_idngiesCfg_int,input)==0x01)								entradaDigitalInit(input, MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL);					
				_interfazEntradaDigitalUsed[input]=0x01;				
			}
			else
			{
				entradaDigitalInit(input,MODO_INTERFAZ_NO_OPERATIVO);
				_interfazEntradaDigitalUsed[input]=0x00;
			}	
		}
		
	
	
	
		/*
		unsigned char input;
		interfacesConfiguracion 	interfazConfiguracion;	
		for	(input=0;input<10;input++)
		{		
			interfazConfiguracion=interfacesConfiguracionGet (input);					
			_interfazModo[input]=MODO_INTERFAZ_NO_OPERATIVO;			
			if (interfazConfiguracion.modoInterfaz==INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE)						_interfazModo[input]=MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE;
			if (interfazConfiguracion.modoInterfaz==INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_DESCENDENTE)						_interfazModo[input]=MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_DESCENDENTE;
			if (interfazConfiguracion.modoInterfaz==INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_LONG)													_interfazModo[input]=MODO_INTERFAZ_PULSADOR_SHORT_LONG;
			if (interfazConfiguracion.modoInterfaz==INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO)										_interfazModo[input]=MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO;
			if (interfazConfiguracion.modoInterfaz==INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL)										_interfazModo[input]=MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL;								
			if ((_interfazModo[input]==MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE) || (_interfazModo[input]==MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_DESCENDENTE) || (_interfazModo[input]==MODO_INTERFAZ_PULSADOR_SHORT_LONG) || (_interfazModo[input]==MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO) || (_interfazModo[input]==MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL))
			{
				_interfazEntradaDigitalUsed[input]=0x01;
				entradaDigitalInit(input,_interfazModo[input]);
			}
			else
			{
				_interfazEntradaDigitalUsed[input]=0x00;
			}
		}
		*/
}


//void interfazModoSet (unsigned char modo)
//{
//	_interfazModo=modo;		
//	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_MODE,_interfazModo);
//	interfazModoUpdt();
//}

//void interfazSubmodoSet (unsigned char submodo)
//{
//	_interfazSubmodo=submodo;
//	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_SUBMODE,_interfazSubmodo);
//}


//unsigned char interfazDeshabilitadoGet (void)
//{
//	return _interfazDeshabilitado;
//}


//void interfazHabilitar (void)
//{
//	_interfazDeshabilitado=0x00;
//	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_DISABLED,_interfazDeshabilitado);
//}
//void interfazDeshabilitar (void)
//{
//	_interfazDeshabilitado=0x01;
//	eeprom_write_byte((uint8_t*)EEPROM_ADDRESS_DISABLED,_interfazDeshabilitado);
//}

//unsigned char interfazModoGet (void)
//{
//	return _interfazModo;
//}
//unsigned char interfazSubmodoGet (void)
//{
//	return _interfazSubmodo;
//}

void interfazDoTask (void)
{	
	unsigned char input;
	for (input=0;input<10;input++)
	{
	
	if (_interfazDeshabilitado[input]==0x00)
	{
		if (_interfazEntradaDigitalUsed[input]==0x01)
		{
			entradaDigitalDoTask(input);
		}
			
		//if (_interfazEntradaAnalogicaUsed==0x01)
		//{
		//	entradaAnalogicaDoTask();
		//}
	}
	}
}



//unsigned char interfazLevelGet (void)
//{
//	unsigned char answer;
//	answer=entradaAnalogicaLevelGet();
//	return answer;
//}


