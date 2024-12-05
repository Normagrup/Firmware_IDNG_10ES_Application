#define INTERFACES_DEF
    #include "interfaces.h"
#undef INTERFACES_DEF
#define INC_INTERFACES_NUMERO_SERIE_S
    #include "interfaces.h"
#undef INC_INTERFACES_NUMERO_SERIE_S
#define INC_INTERFACES_MODOS_INTERFAZ_E
    #include "interfaces.h"
#undef INC_INTERFACES_MODOS_INTERFAZ_E
#define INC_INTERFACES_TIPO_INTERFAZ_E
    #include "interfaces.h"
#undef INC_INTERFACES_TIPO_INTERFAZ_E
#define INC_INTERFACES_EVENTOS_TIPOS_E
    #include "interfaces.h"
#undef INC_INTERFACES_EVENTOS_TIPOS_E
#define INC_INTERFACES_EVENTOS_S
    #include "interfaces.h"
#undef INC_INTERFACES_EVENTOS_S
#define INC_INTERFACES_CONFIGURACION_S
    #include "interfaces.h"
#undef INC_INTERFACES_CONFIGURACION_S

#define INC_SENSORES_ORIGEN_NOTIFICACION_S
    #include "sensores.h"
#undef INC_SENSORES_ORIGEN_NOTIFICACION_S

#define INC_INTERFACES_PRT
    #include "interfaces.h"
#undef INC_INTERFACES_PRT

#define INC_SENSORES_PRT
    #include "sensores.h"
#undef INC_SENSORES_PRT
#include "dataTypes.h"
#include "sensoresConfig.h"
#include "normaLinkSensores.h"
#include "utils.h"

/* Los KEEP_ALIVE son muy importantes en el caso de DETECTORES_PRESENCIA_FLANCO y PULSADORES DE ACCION TEMPORIZADA.
 *
 * Se generarán cuando no hay motivos para establecer comunicación teniendo en cuenta únicamente esas funciones
 * quiere decir que si está mandando LIGHT_REPORTS da igual, generará también el KEEP ALIVE.
 * Si por cualquier otro motivo tampoco se estableció comunicación se generará un KEEP ALIVE igualmente.
 * DET_PRESENCIA modo nivel comunica continuamente, REG_LUZ también.
 */


#define NIVEL_NO_APLICABLE                              	0xFF
/* IDNG-EAD */
#define REGISTRO_ESTADO_BIT_SHORT_PRESS                 	0x01
#define REGISTRO_ESTADO_BIT_LONG_PRESS                  	0x02
#define REGISTRO_ESTADO_BIT_LONG_RELEASE                	0x04
#define REGISTRO_ESTADO_BIT_PRESENCY                    	0x08
#define REGISTRO_ESTADO_BIT_VACANCY                     	0x10
#define REGISTRO_ESTADO_BIT_REPORTE_LUZ                 	0x20

/* IDNG-4P */
#define REGISTRO_ESTADO_BIT_SHORT_PRESS_1                 0x01
#define REGISTRO_ESTADO_BIT_SHORT_PRESS_2                 0x02
#define REGISTRO_ESTADO_BIT_SHORT_PRESS_3                 0x04
#define REGISTRO_ESTADO_BIT_SHORT_PRESS_4                 0x08







unsigned char 				_interfazNotificarPendiente;
unsigned char 				_interfazAddress;
unsigned char 				_registroEstado;
unsigned char 				_lectura;
BOOL 									_inhibido[INTERFACES_MAX];

//BOOL 									_sentidoAscendente[INTERFACES_MAX];
//BOOL 									_posicionEncendido[INTERFACES_MAX];

BOOL 									_sentidoAscendente[INTERFACES_MAX][4];
BOOL 									_posicionEncendido[INTERFACES_MAX][4];


unsigned char 				_buscandoInterfaces;
unsigned char 				_buscandoInterfacesCnt;
unsigned char					_buscandoInterfacesIp[4];
unsigned char 				_interfacesEncontrados;
unsigned char 				_interfacesInstaladosCnt;
unsigned char					interfazCalculoPendiente[16];			// Por cada interfaz; Me han indicado que se ha producido un cambio por tanto iniciaré la consulta.
unsigned char					interfazCalculoPendienteCnt[16];	// Por cada interfaz; Como no sé cuantos equipos tienen mi interfaz lo cierro por timeout.
unsigned short int		interfazCalculoEquipos[16];				// Por cada interfaz
unsigned short int		interfazCalculoApagados[16];			// Por cada interfaz
unsigned short int 		interfazCalculoNivelAcu[16];			// Por cada interfaz

/////////////////////////////////////////// 188 bytes.

static void interfacesSolicitar (interfacesConfiguracion configuracionInterfaz, unsigned char interfazAddress, interfacesNumeroSerie numeroSerie, unsigned char registroEstado, unsigned char lectura);
static void interfacesSolicitar (interfacesConfiguracion configuracionInterfaz, unsigned char interfazAddress, interfacesNumeroSerie numeroSerie, unsigned char registroEstado, unsigned char lectura)
{
    sensoresOrigenNotificacion  origenNotificacion;
    interfacesEventos           evento;	
		origenNotificacion.tipoOrigen=SENSORES_ORIGEN_NOTIFICACION_INTERFAZ;
    origenNotificacion.interfazAddressOrigen=interfazAddress;
    origenNotificacion.interfazNumeroSerie=numeroSerie;			
		do{				
			
			//if (configuracionInterfaz.tipoInterfaz==INTERFACES_TIPO_INTERFAZ_IDNGIES)	evento=interfacesIDNGIESEventoGet(configuracionInterfaz.modoInterfaz, &registroEstado, lectura, &_posicionEncendido[interfazAddress][0], &_sentidoAscendente[interfazAddress][0]);
			evento=interfacesIDNGIESEventoGet(configuracionInterfaz.modoInterfaz, &registroEstado, lectura, &_posicionEncendido[interfazAddress][0], &_sentidoAscendente[interfazAddress][0]);
			if ((evento.eventoTipo!=INTERFACES_EVENTO_NINGUNO) && (evento.eventoTipo!=INTERFACES_EVENTO_KEEP_ALIVE))
			{	
				evento.canal=interfazAddress;
				evento.lectura=lectura;
				sensoresNotificar (origenNotificacion, evento);
				normaLinkSensoresEnviar(origenNotificacion.interfazNumeroSerie, evento);
			}						
		} while (evento.eventoTipo!=INTERFACES_EVENTO_NINGUNO);	
}
BOOL interfacesBuscar (unsigned char *ip)
{	
	return TRUE;
}

void interfacesConfiguracionArrayFromStructGet (interfacesConfiguracion configuracion, unsigned char *cfg)
{
	unsigned short int ptr=0;	
	bytePush(configuracion.instalado, &cfg[ptr],&ptr);
	bytePush(configuracion.desactivado, &cfg[ptr],&ptr);
	bytePush(configuracion.tipoInterfaz, &cfg[ptr],&ptr);
	bytePush(configuracion.modoInterfaz, &cfg[ptr],&ptr);
	arrayPush(&configuracion.numeroSerie.byte[0], &cfg[ptr],&ptr,4);
}

BOOL interfacesConfiguracionSet (unsigned char interfazAddress, unsigned char *cfg)
{		
	return configInterfacesConfiguracionSet(interfazAddress, &cfg[0]);
}

interfacesConfiguracion interfacesConfiguracionGet (unsigned char interfazAddress)
{
    interfacesConfiguracion configuracionRet;
		unsigned char arrayCfg[13];
		unsigned short int ptr=0;	
		if (configInterfacesConfiguracionGet(interfazAddress, &arrayCfg[0])==TRUE)
		{
			bytePop(&arrayCfg[ptr],(unsigned char*)&configuracionRet.instalado,&ptr);										//1				
		}
		else
		{
			configuracionRet.instalado=FALSE;
		}		
		if (configuracionRet.instalado==TRUE)
		{
			bytePop(&arrayCfg[ptr],(unsigned char*)&configuracionRet.desactivado,&ptr);									//2
			bytePop(&arrayCfg[ptr],(unsigned char*)&configuracionRet.tipoInterfaz,&ptr);								//3
			bytePop(&arrayCfg[ptr],(unsigned char*)&configuracionRet.modoInterfaz,&ptr);								//4
			arrayPop(&arrayCfg[ptr], &configuracionRet.numeroSerie.byte[0],&ptr,4);											//8			
		}
		else
		{
			configuracionRet.instalado=FALSE;
			configuracionRet.desactivado=FALSE;
			configuracionRet.modoInterfaz=INTERFACES_MODO_INTERFAZ_NO_OPERATIVO;
			configuracionRet.tipoInterfaz=INTERFACES_TIPO_INTERFAZ_NO_OPERATIVO;
			configuracionRet.numeroSerie.byte[0]=0xFF;
			configuracionRet.numeroSerie.byte[1]=0xFF;
			configuracionRet.numeroSerie.byte[2]=0xFF;			
			configuracionRet.numeroSerie.byte[3]=0xFF;			
		}	
		return configuracionRet;
}

void interfacesStore (unsigned char interfazAddress, unsigned char registroEstado, unsigned char lectura)
{
	_interfazNotificarPendiente=1;
	_interfazAddress=interfazAddress;
	_registroEstado=registroEstado;
	_lectura=lectura;	
}

void interfacesNotificar (unsigned char interfazAddress, unsigned char registroEstado, unsigned char lectura)
{
    // En este caso al pulsar un botón llamamos. 	
    interfacesConfiguracion configuracionInterfaz;
    configuracionInterfaz=interfacesConfiguracionGet(0);
    if (configuracionInterfaz.instalado==TRUE) 
    {
        if (configuracionInterfaz.desactivado==FALSE) 
        {
					interfacesSolicitar (configuracionInterfaz, interfazAddress, configuracionInterfaz.numeroSerie, registroEstado, lectura);
        }
        else
        {
            // [Desactivado].
        }
    }
    else
    {
        //  [No Instalado].
    }
}

interfacesEventos interfacesIDNGIESEventoGet(interfacesModosInterfaz modoInterfaz, unsigned char *registroEstado, unsigned char lectura, BOOL *posicionEncendido, BOOL *sentidoAscendente)
{
	interfacesEventos evento;
	evento.eventoTipo=INTERFACES_EVENTO_NINGUNO;
	if (*registroEstado==REGISTRO_ESTADO_BIT_LONG_RELEASE)
	{
		evento.eventoTipo=INTERFACES_EVENTO_LONG_RELEASE;
		evento.sentidoAscendente=0xFF;
		evento.posicionEncender=0xFF;
		evento.lectura=0xFF;				
	}
	if (*registroEstado==REGISTRO_ESTADO_BIT_SHORT_PRESS) 
	{
		evento.eventoTipo=INTERFACES_EVENTO_SHORT_PRESS;	
		if (*posicionEncendido==FALSE)
		{
			*posicionEncendido=TRUE;								
		}
		else
		{
			*posicionEncendido=FALSE;								
		}				
		if (*posicionEncendido==TRUE)
		{
			*sentidoAscendente=FALSE;
		}
		else
		{
			*sentidoAscendente=TRUE;
		}		
		evento.eventoTipo=INTERFACES_EVENTO_SHORT_PRESS;	
		evento.posicionEncender=*posicionEncendido;		
	}	
	if (*registroEstado==REGISTRO_ESTADO_BIT_LONG_PRESS) 
	{
		if ((*posicionEncendido==FALSE) && (*sentidoAscendente==TRUE))
		{
			*posicionEncendido=TRUE;							
		}
		else
		{			
		}				
		evento.sentidoAscendente=*sentidoAscendente;
    evento.eventoTipo=INTERFACES_EVENTO_LONG_PRESS;
		evento.posicionEncender=0xFF;
		evento.lectura=0xFF;						
		if (*sentidoAscendente==FALSE)
		{
			*sentidoAscendente=TRUE;
		}
		else
		{
			*sentidoAscendente=FALSE;
		}					
	}
	if (*registroEstado==REGISTRO_ESTADO_BIT_PRESENCY) 
	{
		evento.eventoTipo=INTERFACES_EVENTO_PRESENCY;
	}
	if (*registroEstado==REGISTRO_ESTADO_BIT_VACANCY) 
	{
		evento.eventoTipo=INTERFACES_EVENTO_VACANCY;
	}
	*registroEstado=0x00;	
	return evento;
}

void interfacesInit (void)
{	
	unsigned char 						interfazAddress;		
	interfacesConfiguracion 	interfazConfiguracion;	
	_interfacesInstaladosCnt=0;
	for (interfazAddress=0;interfazAddress<16;interfazAddress++)
	{
		interfazConfiguracion=interfacesConfiguracionGet(interfazAddress);				
		
		if (interfazConfiguracion.instalado==TRUE)
		{
			_interfacesInstaladosCnt++;
		}		
	}	
}

void interfacesDoTask(void)
{
	 if (_interfazNotificarPendiente==0x01)
	 {
			_interfazNotificarPendiente=0x00;
			interfacesNotificar (_interfazAddress, _registroEstado, _lectura);		 
	 }	  
}

void interfacesSysTick (void)
{
}

void interfacesRtcTick (void)
{	
}
