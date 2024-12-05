// De cara a futuro guardaré tipo de interfaz y canal y en base a eso generaré un tipo u otro de evento.
// Por ejemplo el de 8 entradas solo genera eventos ShortPress.

/* Sea como fuere cuando se recibe a través de DALI o NORMALINK información de un sensor
 * se refresca un keepAlive, cuando expira se genera un evento ERROR_COMUNICACION y se marca un flag.
 * además de marcar el error de comunicación, ese evento pone el estado de los detectores de presencia en modo no operativo
 * lo que hace que no sea tenido en cuenta y por ejemplo permite que un lazo con los detectores de presencia en presency
 * si uno se desconecta, todo pasa a depender completamente del estado del otro.
*/

#define INC_SENSORES_ACCIONES_E
    #include "sensores.h"
#undef INC_SENSORES_ACCIONES_E
#define INC_SENSORES_TIPOS_E
    #include "sensores.h"
#undef INC_SENSORES_TIPOS_E
#define INC_SENSORES_CONFLICTO_COMPORTAMIENTOS_E
    #include "sensores.h"
#undef INC_SENSORES_CONFLICTO_COMPORTAMIENTOS_E
#define INC_SENSORES_CONFIGURACION_S
    #include "sensores.h"
#undef INC_SENSORES_CONFIGURACION_S
#define INC_SENSORES_EVENTOS_TIPOS_E
    #include "sensores.h"
#undef INC_SENSORES_EVENTOS_TIPOS_E
#define INC_SENSORES_EVENTOS_S
    #include "sensores.h"
#undef INC_SENSORES_EVENTOS_S
#define INC_SENSORES_TIPO_ORIGEN_NOTIFICACION_E
    #include "sensores.h"
#undef INC_SENSORES_TIPO_ORIGEN_NOTIFICACION_E
#define INC_SENSORES_ORIGEN_NOTIFICACION_S
    #include "sensores.h"
#undef INC_SENSORES_ORIGEN_NOTIFICACION_S
#define INC_SENSORES_PRT
    #include "sensores.h"
#undef INC_SENSORES_PRT

#define INC_INTERFACES_PRT
    #include "interfaces.h"
#undef INC_INTERFACES_PRT

#define INC_SENSORES_GRUPOS_PRT
    #include "sensoresGrupos/sensoresGrupos.h"
#undef INC_SENSORES_GRUPOS_PRT
#include "sensoresConfig.h"
#include "daliSensores.h"
#include "utils.h"
#include "config.h"


void sensoresInit (void);



// >> VARIABLES.    
		unsigned char byte[MAX_SENSORES_NORMALINK][5];    		
		unsigned char byteNotif[MAX_SENSORES_NORMALINK];																	// Indica grupo asociado sólo de los sensores que deben notificarse.		
		unsigned char byteNotifPending[MAX_SENSORES_NORMALINK];														// Indica que debe notificarse un cambio.
		// 384 bytes, pueden quedar en 72 bytes, se ahorrarían 312 bytes.		
// << VARIABLES.
// >> PROTOTIPOS STATIC.
    static BOOL sensoresInterfazNumeroSerieCoincideGet(interfacesNumeroSerie numeroSerieEntrada, interfacesNumeroSerie numeroSerie);
    static BOOL sensoresInterfazCanalCoincideGet (unsigned char canalEntrada, unsigned char canal);
// << PROTOTIPOS STATIC.
// >> FUNCIONES.

		
		
		// 01022018 - Esta función notifica a la parte de sensores que se ha producido un cambio en nivel de luz que afectará a ciertos grupos.		
		// Además de las posibles notificaciones que se enviarán para el sistema de alerta de orden externa,
		// Se notificará a la parte de Reguladores de Luz y a la parte de reguladores de presencia para que reinicien el timer de reiteración.
		// Los reguladores de luz, el timer por ejemplo puede ser de 5 segundos en los cuales comenzará a funcionar. La detención en cambio es inmediata.
		// Por tanto si es una orden de off o DAPC off debería detenerse. Sin embargo las ordenes que lo encienden ponen 5 segundos en el timer de actualización.
		
		void sensoresCambioActualLevelNotificar(unsigned char *gruposAfectados)
		{
				// >> PARTE NOTIFICACIONES EXTERNAS [DE MOMENTO SE QUITA].
				//unsigned char n;			
				//for (n=0;n<MAX_SENSORES_NORMALINK;n++)
        //{
				//byteNotif[n]=0xFF;
				//}
				//byteNotif[0]=0;
				//	for (n=0;n<MAX_SENSORES_NORMALINK;n++)
        //{
				//if ((byteNotif[n]<16) &&	(gruposAfectados[byteNotif[n]]==0x01))
					//{
					//						byteNotifPending[n]=0x01;		//Marca la notificacion pendiente.						
					//	normaLinkSensoresInterfazNotifPendingSet();						
					//}
				//}			
				// << PARTE NOTIFICACIONES EXTERNAS.
				
				// >> PARTE NOTIFICACIONES INTERNAS.
				
				// << PARTE NOTIFICACIONES INTERNAS.
				
	
				sensoresGruposReguladoresLuzEstadoActualizar (0xffff);
				
				
				
				
				
		}
		
		BOOL sensoresPeticionNiveles(unsigned char *numeroSerie, unsigned char *valores)
		{
			// PENDIENTE OPTIMIZACION PARA EN EL CASO DE MISMO NUMERO SERIE EN VARIAS ACCIONES. DEBERIA INTEGRARLO TODO.
			
			unsigned char n;
			BOOL retVal=FALSE;			
			for (n=0;n<MAX_SENSORES_NORMALINK;n++)
			{
				if (((numeroSerie[0]==byte[n][0])&&(numeroSerie[1]==byte[n][1])&&(numeroSerie[2]==byte[n][2])&&(numeroSerie[3]==byte[n][3])) && (byteNotif[n]<16)) 
				{						
					 actualLevel_GrupoValoresGet (byteNotif[n], &valores[0]);				
					 retVal=TRUE;
				}
			}						
			return retVal;		
		}
		
		
		
		
		
		
		
		
		
		
		BOOL sensoresIniciarConfiguracion(void)
		{
			return configSensoresIniciarConfiguracion(); 
		}
		
		BOOL sensoresFinalizarConfiguracion(void)
		{
			unsigned char answer;
			answer=configSensoresFinalizarConfiguracion();
			sensoresInit ();			
			return answer;
		}
		unsigned char sensoresMatrizGruposToGrupoGet (UINT16 matrizGrupos)
		{
			unsigned char grupo=0xFF;
			if (matrizGrupos==0x0001) {grupo=0;}
			if (matrizGrupos==0x0002)	{grupo=1;}
			if (matrizGrupos==0x0004)	{grupo=2;}
			if (matrizGrupos==0x0008)	{grupo=3;}
			if (matrizGrupos==0x0010)	{grupo=4;}
			if (matrizGrupos==0x0020)	{grupo=5;}
			if (matrizGrupos==0x0040)	{grupo=6;}
			if (matrizGrupos==0x0080)	{grupo=7;}
			if (matrizGrupos==0x0100)	{grupo=8;}
			if (matrizGrupos==0x0200)	{grupo=9;}
			if (matrizGrupos==0x0400)	{grupo=10;}
			if (matrizGrupos==0x0800)	{grupo=11;}
			if (matrizGrupos==0x1000)	{grupo=12;}
			if (matrizGrupos==0x2000)	{grupo=13;}
			if (matrizGrupos==0x4000)	{grupo=14;}
			if (matrizGrupos==0x8000)	{grupo=15;}
			return grupo;		
		}
		
		UINT16 sensoresGruposToMatrizGruposGet (unsigned char grupo)
		{
			UINT16 matrizGrupos=0x0000;
			if (grupo==0)		{matrizGrupos=0x0001;}
			if (grupo==1)		{matrizGrupos=0x0002;}
			if (grupo==2)		{matrizGrupos=0x0004;}
			if (grupo==3)		{matrizGrupos=0x0008;}
			if (grupo==4)		{matrizGrupos=0x0010;}
			if (grupo==5)		{matrizGrupos=0x0020;}
			if (grupo==6)		{matrizGrupos=0x0040;}
			if (grupo==7)		{matrizGrupos=0x0080;}
			if (grupo==8)		{matrizGrupos=0x0100;}
			if (grupo==9)		{matrizGrupos=0x0200;}
			if (grupo==10)	{matrizGrupos=0x0400;}
			if (grupo==11)	{matrizGrupos=0x0800;}
			if (grupo==12)	{matrizGrupos=0x1000;}
			if (grupo==13)	{matrizGrupos=0x2000;}
			if (grupo==14)	{matrizGrupos=0x4000;}
			if (grupo==15)	{matrizGrupos=0x8000;}
			return matrizGrupos;	
		}
		
		BOOL sensoresConfiguracionSet (unsigned char accion, unsigned char *cfg)
		{			
			return configSensoresConfiguracionSet(accion, &cfg[0]);
		}


		sensoresConfiguracion sensoresConfiguracionGet (unsigned char sensorAddress)
		{			
			sensoresConfiguracion configuracion;	
			unsigned char arrayCfg[14];
			unsigned short int ptr=0;	
			
			if (configSensoresConfiguracionGet(sensorAddress, &arrayCfg[0])==TRUE)
			{
				bytePop(&arrayCfg[ptr],(unsigned char*)&configuracion.instalado,&ptr);											//1				
			}
			else
			{
				configuracion.instalado=FALSE;
			}					
			if (configuracion.instalado==TRUE)
			{
				bytePop(&arrayCfg[ptr],(unsigned char*)&configuracion.desactivado,&ptr);										//
				bytePop(&arrayCfg[ptr],(unsigned char*)&configuracion.tipoSensor,&ptr);											//
				arrayPop(&arrayCfg[ptr], &configuracion.interfazNumeroSerie.byte[0],&ptr,4);								//
				uint16Pop(&arrayCfg[ptr], &configuracion.matrizGrupos,&ptr);																//
				configuracion.matrizDesactivacionGrupos=0x0000;																							//
				bytePop(&arrayCfg[ptr],(unsigned char*)&configuracion.accion[0][0],&ptr);										//			
				configuracion.accion[0][1]=ACCION_NOP;
				configuracion.accion[1][0]=ACCION_NOP;
				configuracion.accion[1][1]=ACCION_NOP;
				configuracion.accion[2][0]=ACCION_NOP;
				configuracion.accion[2][1]=ACCION_NOP;
				configuracion.accion[3][0]=ACCION_NOP;
				configuracion.accion[3][1]=ACCION_NOP;		
				configuracion.comportamiento[0][0]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[0][1]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[0][2]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[0][3]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[0][4]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[1][0]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[1][1]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[1][2]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[1][3]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[1][4]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[2][0]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[2][1]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[2][2]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[2][3]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[2][4]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[3][0]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[3][1]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[3][2]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[3][3]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[3][4]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;			
			}
			else
			{
				configuracion.instalado=FALSE;
				configuracion.desactivado=FALSE;				
				configuracion.tipoSensor=SENSORES_TIPOS_NINGUNO;				
				configuracion.matrizGrupos=0x0000;
				configuracion.matrizDesactivacionGrupos=0x0000;		
				configuracion.interfazNumeroSerie.byte[0]=0xFF;
				configuracion.interfazNumeroSerie.byte[1]=0xFF;
				configuracion.interfazNumeroSerie.byte[2]=0xFF;
				configuracion.interfazNumeroSerie.byte[3]=0xFF;
				configuracion.accion[0][0]=ACCION_NOP;
				configuracion.accion[0][1]=ACCION_NOP;
				configuracion.accion[1][0]=ACCION_NOP;
				configuracion.accion[1][1]=ACCION_NOP;
				configuracion.accion[2][0]=ACCION_NOP;
				configuracion.accion[2][1]=ACCION_NOP;
				configuracion.accion[3][0]=ACCION_NOP;
				configuracion.accion[3][1]=ACCION_NOP;
				configuracion.comportamiento[0][0]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[0][1]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[0][2]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[0][3]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[0][4]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[1][0]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[1][1]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[1][2]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[1][3]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[1][4]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[2][0]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[2][1]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[2][2]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[2][3]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[2][4]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[3][0]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[3][1]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[3][2]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[3][3]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
				configuracion.comportamiento[3][4]=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;	
			}			
			return configuracion;        
		}
    
				
		void sensoresInit (void)
    {
        sensoresConfiguracion configuracion;			
        unsigned char sensorAddress;
			
				_sensoresGruposAlgunoOperativoClear();
        
				for (sensorAddress=0;sensorAddress<MAX_SENSORES_NORMALINK;sensorAddress++)
        {
            configuracion=sensoresConfiguracionGet(sensorAddress);
            if ((configuracion.instalado==TRUE) && (configuracion.desactivado==FALSE))
            {
								byte[sensorAddress][0]=configuracion.interfazNumeroSerie.byte[0];
                byte[sensorAddress][1]=configuracion.interfazNumeroSerie.byte[1];
                byte[sensorAddress][2]=configuracion.interfazNumeroSerie.byte[2];
								byte[sensorAddress][3]=configuracion.interfazNumeroSerie.byte[3];														
								byte[sensorAddress][4]=((((unsigned char)(configuracion.tipoSensor))&0xF0)>>4);							
								_sensoresGruposAlgunoOperativoSet(sensoresMatrizGruposToGrupoGet(configuracion.matrizGrupos), configuracion.tipoSensor);							
            }						
        }
								
        interfacesInit();
        sensoresGruposInit();				
	  }
    void sensoresDoTask (void)
    {
        sensoresGruposDoTask();
        interfacesDoTask();
				normaLinkSensoresDoTask();			
    }
    void sensoresRtcTick (void)
    {
        sensoresGruposRtcTick();
        interfacesRtcTick();
    }
    void sensoresSysTick (void)
    {
        sensoresGruposSysTick();
        interfacesSysTick();			
				daliSensoresSysTick();
				normaLinkSensoresSysTick();
    }
    static BOOL sensoresInterfazNumeroSerieCoincideGet(interfacesNumeroSerie numeroSerieEntrada, interfacesNumeroSerie numeroSerie)
    {
				if ((numeroSerieEntrada.byte[0]==numeroSerie.byte[0]) && (numeroSerieEntrada.byte[1]==numeroSerie.byte[1]) && (numeroSerieEntrada.byte[2]==numeroSerie.byte[2]) && (numeroSerieEntrada.byte[3]==numeroSerie.byte[3]))
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}			
    }
    static BOOL sensoresInterfazCanalCoincideGet (unsigned char canalEntrada, unsigned char canal)
    {
        if ((canalEntrada==canal) || (canalEntrada==0xFF))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    static unsigned char numeroSerieIdCoincideGet (interfacesNumeroSerie numeroSerie, unsigned char canal, unsigned char *sensorAddress)
    {
        // Devuelve el número de sensores coincidente y da valores a una matriz con direcciones.
        unsigned char n;
        unsigned char cnt=0;
        for (n=0;n<MAX_SENSORES_NORMALINK;n++)
        {				
					if ((numeroSerie.byte[0]==byte[n][0])&&(numeroSerie.byte[1]==byte[n][1])&&(numeroSerie.byte[2]==byte[n][2])&&(numeroSerie.byte[3]==byte[n][3]) && (canal==byte[n][4]))
					{
						sensorAddress[cnt]=n;
						cnt++;
					}					
        }
        return cnt;
    }
    void sensoresNotificar (sensoresOrigenNotificacion origenNotificacion, interfacesEventos eventoInterfaz)
    {
        unsigned char iteratorCoincidenciasId;
        unsigned char coincidenciasIdCnt;
        unsigned char sensorAddressCoincidentes[MAX_SENSORES_NORMALINK];
        sensoresConfiguracion configuracion;
        sensoresEventos eventoSensor;
        unsigned char sensorAddress;
        coincidenciasIdCnt=numeroSerieIdCoincideGet(origenNotificacion.interfazNumeroSerie, eventoInterfaz.canal, &sensorAddressCoincidentes[0]);
        for (iteratorCoincidenciasId=0;iteratorCoincidenciasId<coincidenciasIdCnt;iteratorCoincidenciasId++)
        {
            sensorAddress=sensorAddressCoincidentes[iteratorCoincidenciasId];
            configuracion=sensoresConfiguracionGet(sensorAddress);
						configuracion.tipoSensor=((configuracion.tipoSensor)&0x0F);
            if (configuracion.instalado==TRUE)
            {
                if (configuracion.desactivado==FALSE)
                {
                    eventoSensor.eventoTipo=(sensoresEventosTipos)(eventoInterfaz.eventoTipo);   
										eventoSensor.posicionEncender=eventoInterfaz.posicionEncender;
                    eventoSensor.sentidoAscendente=eventoInterfaz.sentidoAscendente;
                    eventoSensor.lectura=eventoInterfaz.lectura;
                    sensoresGruposNotificar (sensorAddress, origenNotificacion, configuracion, eventoSensor);
                }
            }
        }
    }
    void _sensoresDALIrecallMaxLevelCb (unsigned char dest)
    {
        //Pone los equipos a 254.
        if (dest!=0xFF)
        {
        }
    }
    void _sensoresDALIoffCb (unsigned char dest)
    {
        //Pone los equipos a 0.
        if (dest!=0xFF)
        {
        }
    }
    void _sensoresDALIupCb (unsigned char dest)
    {
        // No hace nada.
        // En el futuro sí debería utilizar algún tipo de algoritmo.
        if (dest!=0xFF)
        {
        }
    }
    void _sensoresDALIdownCb (unsigned char dest)
    {
        // No hace nada.
        // En el futuro sí debería utilizar algún tipo de algoritmo.
        if (dest!=0xFF)
        {
        }
    }
    void _sensoresDALIcomandoArcoDirectoCb (unsigned char dest, unsigned char nivel)
    {
        // Pone los equipos a nivel.
        if ((dest!=0xFF) && (nivel!=0xFF))
        {
        }
    }
    BOOL _sensoresDALIrecallMaxLevelEnviar (unsigned char numeroGrupo)
    {
        return daliSensoresRecallMaxLevelEnviar (numeroGrupo);
    }
    BOOL _sensoresDALIoffEnviar (unsigned char numeroGrupo)
    {
        return daliSensoresOffEnviar (numeroGrupo);
    }
    BOOL _sensoresDALIupEnviar (unsigned char numeroGrupo)
    {
        return daliSensoresUpEnviar (numeroGrupo);
    }
		BOOL _sensoresDALIonAndStepUpEnviar (unsigned char numeroGrupo)
    {
        return daliSensoresOnAndStepUpEnviar (numeroGrupo);
    }
    BOOL _sensoresDALIdownEnviar (unsigned char numeroGrupo)
    {
        return daliSensoresDownEnviar (numeroGrupo);
    }
    BOOL _sensoresDALIcomandoArcoDirectoEnviar(unsigned char numeroGrupo, unsigned char nivel)
    {
        return daliSensoresComandoArcoDirectoEnviar(numeroGrupo, nivel);
    }
    BOOL _sensoresDALIquerySensorStatusEnviar(unsigned char sensorAddress)
    {
        return daliSensoresQuerySensorStatusEnviar(sensorAddress);
    }
		
		BOOL _sensoresDALIgoToSceneEnviar(unsigned char sensorAddress)
    {
        return daliSensoresGoToSceneEnviar(sensorAddress);
    }

// << FUNCIONES.
