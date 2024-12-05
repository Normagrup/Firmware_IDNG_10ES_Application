


// ARRANCAN EN PRESENCY¿?



#define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA
    #include "sensoresGruposDetectoresPresencia.h"
#undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA


#define DETECTORES_PRESENCIA_TIEMPO_PRESENCY_CUANDO_ARRANCA			10


// VARIABLES
    sensoresGruposDetectoresPresenciaEstados    _sensoresGruposDetectoresPresenciaEstado[MAX_GRUPOS];
    UINT16                                      _sensoresGruposDetectoresPresenciaRetardoCnt[MAX_GRUPOS];
    unsigned char                               _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[MAX_GRUPOS];
    BOOL                                        _sensoresGruposDetectoresPresenciaVacancyInhibido[MAX_GRUPOS];
    BOOL                                        _sensoresGruposDetectoresPresenciaPresencyInhibido[MAX_GRUPOS];
    BOOL                                        _sensoresGruposDetectoresPresenciaRtcOverflow;
    sensoresGruposDetectoresPresenciaEstados    _sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[MAX_SENSORES_NORMALINK];
    UINT16                                      _sensoresGruposDetectoresPresenciaMatrizGruposAsociadosModoNivel[MAX_SENSORES_NORMALINK];   //¿?Flash
// VARIABLES
// PROTOTIPOS STATIC.
    static void detectoresPresenciaModoNivelEstadoIndividualSet(unsigned char sensorAddress, sensoresEventos evento);
    static void detectoresPresenciaModoNivelEstadoIndividualGet(unsigned char numeroGrupo, sensoresGruposDetectoresPresenciaEstados *estadoIndividual);
    static sensoresGruposDetectoresPresenciaEstados detectoresPresenciaModoNivelEstadoGlobalGet(sensoresGruposDetectoresPresenciaEstados *estado);
    static void sensoresGruposDetectoresPresenciaAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, BOOL presency);
    static void sensoresGruposDetectoresPreseciaModoNivelEstadoActualizar(unsigned char numeroGrupo,  sensoresEventos evento, sensoresGruposDetectoresPresenciaEstados estadoGlobal);
    static void sensoresGruposDetectoresPresenciaModoFlancoEstadoActualizar (unsigned char numeroGrupo,  sensoresEventosTipos evento, UINT16 retardo);
// PROTOTIPOS STATIC.
// FUNCIONES
    static void detectoresPresenciaModoNivelEstadoIndividualSet(unsigned char sensorAddress, sensoresEventos evento)
    {
        if (evento.eventoTipo==SENSORES_EVENTO_PRESENCY)
        {
            _sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[sensorAddress]=DETECTORES_PRESENCIA_ESTADO_PRESENCY;
        }
        else if (evento.eventoTipo==SENSORES_EVENTO_VACANCY)
        {
            _sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[sensorAddress]=DETECTORES_PRESENCIA_ESTADO_VACANCY;
        }
        // Gestión del KEEPALIVE.
        //else if (evento.eventoTipo==)
        //{
        //    _sensoresGruposDetectoresPresenciaEstadoIndividualModoNivel[sensorAddress]=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO;
        //}
    }
    static void detectoresPresenciaModoNivelEstadoIndividualGet(unsigned char numeroGrupo, sensoresGruposDetectoresPresenciaEstados *estadoIndividual)
    {
        unsigned char sensorAddress;
        for (sensorAddress=0;sensorAddress<MAX_SENSORES_NORMALINK;sensorAddress++)
        {
            if  (_sensoresGruposForMeGet(_sensoresGruposDetectoresPresenciaMatrizGruposAsociadosModoNivel[sensorAddress],numeroGrupo)==TRUE)
            {
                estadoIndividual[sensorAddress]=_sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[sensorAddress];
            }
            else
            {
                estadoIndividual[sensorAddress]=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO;
            }
        }
    }
    static sensoresGruposDetectoresPresenciaEstados detectoresPresenciaModoNivelEstadoGlobalGet(sensoresGruposDetectoresPresenciaEstados *estado)
    {
       /*  En esta función en caso de detectar alguno en presency se da la salida PRESENCY
        *  En caso de no detectar alguno en presency y detectar alguno en vacancy se da la salida VACANCY
        *  En caso de no detectar alguno operativo se puede dar la salida PRESENCY, la salida VACANCY o dar NO OPERATIVO que hará que todo siga en el mismo estado.
        */
        unsigned char sensorAddress;
        BOOL algunoEnVacancy=FALSE;
        BOOL algunoEnPresency=FALSE;
        for (sensorAddress=0;sensorAddress<MAX_SENSORES_NORMALINK;sensorAddress++)
        {
            if (estado[sensorAddress]==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
            {
                algunoEnPresency=TRUE;
            }
            else if (estado[sensorAddress]==DETECTORES_PRESENCIA_ESTADO_VACANCY)
            {
                algunoEnVacancy=TRUE;
            }
        }
        if (algunoEnPresency==TRUE)
        {
            return DETECTORES_PRESENCIA_ESTADO_PRESENCY;
        }
        else
        {
            if (algunoEnVacancy==TRUE)
            {
                return DETECTORES_PRESENCIA_ESTADO_VACANCY;
            }
            else
            {
                // [Esto es lo que suceede cuando no hay ningún dispositivo operativo en el grupo y debería haberlo].
                return DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO;    // [w] A definir por el usuario.
                //return DETECTORES_PRESENCIA_ESTADO_VACANCY;
                //return DETECTORES_PRESENCIA_ESTADO_PRESENCY;
            }
        }
    }
    static void sensoresGruposDetectoresPresenciaAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, BOOL presency)
    {
        BOOL transmitirOrden=TRUE;
        // >> A N A L I S I S     C O N F L I C T O S .
            // >> INTERRUPTOR GENERAL.
            // << INTERRUPTOR GENERAL.
            // >> PULSADORES ACCION REGULACION.
                if (sensoresGruposPulsadoresAccionRegulacionEstadoGet(numeroGrupo)!=PULSADORES_ACCION_REGULACION_ESTADO_NO_OPERATIVO)
                {
                    if (comportamiento.pulsadoresAccionRegulacion==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
                        if ((sensoresGruposPulsadoresAccionRegulacionEstadoGet(numeroGrupo)==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_ASCENDENTE) || (sensoresGruposPulsadoresAccionRegulacionEstadoGet(numeroGrupo)==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_DESCENDENTE))
                        {
                        }
                    }
                }
            // << PULSADORES ACCION REGULACION.
            // >> REGULADORES DE LUZ.
                if (sensoresGruposReguladoresLuzEstadoGet(numeroGrupo)!=REGULADORES_LUZ_ESTADO_NO_OPERATIVO)
                {
                    if (comportamiento.reguladoresLuz==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
												sensoresGruposReguladoresLuzEstadoActualizar ((1<<numeroGrupo));											
                    }
                }
            // << REGULADORES DE LUZ.
            // >> DETECTORES DE PRESENCIA Y PULSADORES DE ACCION TEMPORIZADA.
                
								if (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)!=PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO)
								{
									 if (comportamiento.detectoresPresenciaPulsadoresAccionTemporizada==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
									 {										 
										 if  ((presency==FALSE) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_CORRIENDO))
											 {
												 transmitirOrden=FALSE;												 
											 }
									 }
									
									
								}
								
								/*
								if ((sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)!=PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)!=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO))
                {
                    if (comportamiento.detectoresPresenciaPulsadoresAccionTemporizada==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
                        if ((sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_VACANCY) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO))
                        {
                            // Si los detectores de presencia están en ESTADO_VACANCY y los pulsadores de acción temporizada están en ESTADO_DETENIDO bloquea la orden.
                            transmitirOrden=FALSE;
                        }
                        else
                        {
                            // En caso contrario transmite la orden.
                        }
                    }
                }
								*/
            // >> DETECTORES DE PRESENCIA Y PULSADORES DE ACCION TEMPORIZADA.
        // << A N A L I S I S     C O N F L I C T O S .
        // >> E J E C U C I O N .
            if (transmitirOrden==TRUE)
            {
                if (presency==TRUE)
                {
                    _sensoresGruposAccionRecallMaxLevelSet(numeroGrupo);
                }
                else
                {
                _sensoresGruposAccionOffSet(numeroGrupo);
                }
            }
        // << E J E C U C I O N .
    }
    static void sensoresGruposDetectoresPreseciaModoNivelEstadoActualizar(unsigned char numeroGrupo,  sensoresEventos evento, sensoresGruposDetectoresPresenciaEstados estadoGlobal)
    {
        sensoresMatrizConflictoComportamientos comportamiento;
        if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
        {
            //if ((evento.eventoTipo==SENSORES_EVENTO_VACANCY) || (evento.eventoTipo==SENSORES_EVENTO_ERROR_COMUNICACION))
            if (evento.eventoTipo==SENSORES_EVENTO_VACANCY)
            {
                if (estadoGlobal==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
                {
                }
                else if (estadoGlobal==DETECTORES_PRESENCIA_ESTADO_VACANCY)
                {
                    if (_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]==FALSE)
                    {
                        //  [No inhibido].
                        // Los comportamientos se sacarían en función de los argumentos de la llamada (todavía no lo estoy pasando) y la selección obtenida de la configuración del grupo.
                        // configuracion=sensoresGruposConfiguracionGet(numeroGrupo);
                        comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, FALSE);
                    }
                    _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;
                    _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_VACANCY;
										_sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=DETECTORES_PRESENCIA_REITERACION_PERIODO;
                }
            }
        }
        else if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_VACANCY)
        {
            //if ((evento.eventoTipo==SENSORES_EVENTO_PRESENCY) || (evento.eventoTipo==SENSORES_EVENTO_ERROR_COMUNICACION))
            if (evento.eventoTipo==SENSORES_EVENTO_PRESENCY)
            {
                if (estadoGlobal==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
                {
                    if (_sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]==FALSE)
                    {
                        //  [No inhibido].
                        // Los comportamientos se sacarían en función de los argumentos de la llamada (todavía no lo estoy pasando) y la selección obtenida de la configuración del grupo.
                        // configuracion=sensoresGruposConfiguracionGet(numeroGrupo);
                        comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, TRUE);
                    }
                    _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=FALSE;
                    _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_PRESENCY;
										

                }
            }
        }
    }
    static void sensoresGruposDetectoresPresenciaModoFlancoEstadoActualizar (unsigned char numeroGrupo,  sensoresEventosTipos evento, UINT16 retardo)
    {
				// PTE. Mejor ordenación.
			
				sensoresMatrizConflictoComportamientos comportamiento;
        if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
        {
            if (evento==SENSORES_EVENTO_PRESENCY)
            {
                _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]=retardo;
							
										// ¿? inhibiciones.
										// Los comportamientos se sacarían en función de los argumentos de la llamada (todavía no lo estoy pasando) y la selección obtenida de la configuración del grupo.
                    // configuracion=sensoresGruposConfiguracionGet(numeroGrupo);
                    comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, TRUE);				
							
            }
            else if (evento==SENSORES_EVENTO_VACANCY)
            {						
                if (_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]==FALSE)
                {
                    // Los comportamientos se sacarían en función de los argumentos de la llamada (todavía no lo estoy pasando) y la selección obtenida de la configuración del grupo.
                    // configuracion=sensoresGruposConfiguracionGet(numeroGrupo);
                    comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, FALSE);
                }								
							_sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;
							_sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=DETECTORES_PRESENCIA_REITERACION_PERIODO;
							_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_VACANCY;							
            }
        }
        else if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_VACANCY)
        {
            if (evento==SENSORES_EVENTO_PRESENCY)
            {
                _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=FALSE;
                _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]=retardo;
                _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_PRESENCY;
                if (_sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]==FALSE)
                {
                    // Los comportamientos se sacarían en función de los argumentos de la llamada (todavía no lo estoy pasando) y la selección obtenida de la configuración del grupo.
                    // configuracion=sensoresGruposConfiguracionGet(numeroGrupo);
                    comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
									
									
                    sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, TRUE);									
                }
            }
            else if (evento==SENSORES_EVENTO_VACANCY)
            {
            }
        }
    }
    sensoresGruposDetectoresPresenciaEstados sensoresGruposDetectoresPresenciaEstadoGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposDetectoresPresenciaEstado[numeroGrupo];
    }
    UINT16 sensoresGruposDetectoresPresenciaRetardoCntGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo];
    }
    unsigned char sensoresGruposDetectoresPresenciaReiteracionPeriodoCntGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo];
    }
    BOOL sensoresGruposDetectoresPresenciaVacancyInhibicionGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo];
    }
    BOOL sensoresGruposDetectoresPresenciaPresencyInhibicionGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo];
    }
    void sensoresGruposDetectoresPresenciaVacancyInhibir(unsigned char numeroGrupo)
    {
        _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=TRUE;
    }
    void sensoresGruposDetectoresPresenciaPresencyInhibir(unsigned char numeroGrupo)
    {
        _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=TRUE;
    }
    void sensoresGruposDetectoresPresenciaInhibicionesBorrar (unsigned char numeroGrupo)
    {
        _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=FALSE;
        _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;
    }
    void sensoresGruposDetectoresPresenciaNotificar (unsigned char sensorAddress, UINT16 matrizGruposAplicable, sensoresEventos evento)
    {
        sensoresGruposConfiguracion configuracion;
        unsigned char numeroGrupo;
        sensoresGruposDetectoresPresenciaEstados estadoIndividual[MAX_SENSORES_NORMALINK];
        sensoresGruposDetectoresPresenciaEstados estadoGlobal;

        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            if (_sensoresGruposForMeGet(matrizGruposAplicable,numeroGrupo)==TRUE)
            {
                // [Aplicable a este grupo].
                configuracion=sensoresGruposConfiguracionGet(numeroGrupo);
                if (configuracion.detectoresPresenciaModo==DETECTORES_PRESENCIA_MODO_NIVEL)
                {
                    // [Modo NIVEL].
                    // [PTE: Gestión KeepAlive].
                    detectoresPresenciaModoNivelEstadoIndividualSet(sensorAddress, evento);                         // Fija el estado individual de un sensor.
                    detectoresPresenciaModoNivelEstadoIndividualGet(numeroGrupo,&estadoIndividual[0]);              // Obtiene la matriz de estados individuales.
                    estadoGlobal=detectoresPresenciaModoNivelEstadoGlobalGet(&estadoIndividual[0]);                 // Obtiene el estado global del grupo.
                    sensoresGruposDetectoresPreseciaModoNivelEstadoActualizar(numeroGrupo, evento, estadoGlobal);
                }
                else if (configuracion.detectoresPresenciaModo==DETECTORES_PRESENCIA_MODO_FLANCO)
                {
                    // [Modo FLANCO].
                    // [PTE: Gestión KeepAlive].
                    sensoresGruposDetectoresPresenciaModoFlancoEstadoActualizar (numeroGrupo, evento.eventoTipo, configuracion.detectoresPresenciaRetardo);
                }
            }
        }
    }
    void sensoresGruposDetectoresPresenciaReiteracionPeriodoCntReinicializar (unsigned char numeroGrupo)
    {
      _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=DETECTORES_PRESENCIA_REITERACION_PERIODO;
    }
    void sensoresGruposDetectoresPresenciaInit(void)
    {
        unsigned char numeroGrupo;
        unsigned char sensorAddress;
        sensoresConfiguracion configuracionSensor;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
						_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO;
						if ((_sensoresGruposAlgunoOperativoGet (numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL)==TRUE) || (_sensoresGruposAlgunoOperativoGet (numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL_REGULADOR_LUZ)==TRUE))
            {
                _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_PRESENCY; // [w] Revisarlo. También los interfaces deberían tener un tiempo desde el arranque en que no den ninguna señal de vacancy.
								_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=FALSE;
                _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;
                _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]=0xFFFF;
                _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=0xFF;
            }
						
						if ((_sensoresGruposAlgunoOperativoGet (numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_FLANCO)==TRUE) || (_sensoresGruposAlgunoOperativoGet (numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL_REGULADOR_LUZ)==TRUE) || (_sensoresGruposAlgunoOperativoGet (numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_FLANCO_REGULADOR_LUZ)==TRUE))
            {
                _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_PRESENCY; // [w] Revisarlo. También los interfaces deberían tener un tiempo desde el arranque en que no den ninguna señal de vacancy.
								_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=FALSE;
                _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;
                _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]=DETECTORES_PRESENCIA_TIEMPO_PRESENCY_CUANDO_ARRANCA;
                _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=0xFF;
            }					
        }
        for (sensorAddress=0;sensorAddress<MAX_SENSORES_NORMALINK;sensorAddress++)
        {
            configuracionSensor=sensoresConfiguracionGet(sensorAddress);
            if ((configuracionSensor.tipoSensor==SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL_REGULADOR_LUZ) || (configuracionSensor.tipoSensor==SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL))
            {
                _sensoresGruposDetectoresPresenciaMatrizGruposAsociadosModoNivel[sensorAddress]=configuracionSensor.matrizGrupos;                // [w] Estoy obviando la desactivación.
            }
            else
            {
                _sensoresGruposDetectoresPresenciaMatrizGruposAsociadosModoNivel[sensorAddress]=0;
            }
            _sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[sensorAddress]=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO; // [w] Revisarlo en relación con lo de arriba.
        }
    }
    void sensoresGruposDetectoresPresenciaDoTask (void)
    {
        unsigned char numeroGrupo;
        sensoresGruposConfiguracion configuracion;
        sensoresMatrizConflictoComportamientos comportamiento;

        if (_sensoresGruposDetectoresPresenciaRtcOverflow==TRUE)
        {
            for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
            {
                // >> FLANCO
                if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
                {
                    configuracion=sensoresGruposConfiguracionGet(numeroGrupo);

                    if (configuracion.detectoresPresenciaModo==DETECTORES_PRESENCIA_MODO_FLANCO)
                    {
                        if (_sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]>0)
                        {
                            _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]--;
                        }
                        else
                        {
                            sensoresGruposDetectoresPresenciaModoFlancoEstadoActualizar (numeroGrupo, SENSORES_EVENTO_VACANCY, 0xFFFF);
                        }
                    }
               }
                // << FLANCO

                // >> ACCION REITERATIVA
                if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_VACANCY)
                {
                    if (_sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]>0)
                    {
                        _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]--;
                    }
                    else
                    {
                        if (_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]==FALSE)
                        {
                            // Los comportamientos se sacarían en función de los argumentos de la llamada (todavía no lo estoy pasando) y la selección obtenida de la configuración del grupo.
                            // configuracion=sensoresGruposConfiguracionGet(numeroGrupo);
                            comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, FALSE);
                        }
                        _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=DETECTORES_PRESENCIA_REITERACION_PERIODO;
                    }
                }
                // << ACCION REITERATIVA
            }
            _sensoresGruposDetectoresPresenciaRtcOverflow=FALSE;
        }
    }
    void sensoresGruposDetectoresPresenciaRtcTick (void)
    {
        _sensoresGruposDetectoresPresenciaRtcOverflow=TRUE;
    }
    void sensoresGruposDetectoresPresenciaSysTick (void)
    {
    }
// FUNCIONES
