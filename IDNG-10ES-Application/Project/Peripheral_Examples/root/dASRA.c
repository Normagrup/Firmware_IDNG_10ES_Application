		#include "dali.h"
				
		#define DASRA_GLOBAL_PROCESS_TIMEOUT		600
		#define DASRA_DEVICE_PROCESS_TIMEOUT		30
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
	
		void dAQueryShortAddrSend (void);
		void dAInitializeSend (unsigned char parameter);
		void dARandomizeSend (void);
		void dAWithdrawSend (void);
		void dATerminateSend (void);
		void dAQueryMissingSAddrSend (void);
		void dASearchAddrHSend (unsigned char value);
		void dASearchAddrMSend (unsigned char value);
		void dASearchAddrLSend (unsigned char value);
		void dACompareSend (void);
		void dAQueryCGLoopSend (void);

		// External.
    extern unsigned char    _dASearchAddrHSent;
    extern unsigned char    _dASearchAddrMSent;
    extern unsigned char    _dASearchAddrLSent;
    extern unsigned char    _dACompareSent;
    extern BOOL             _dACompareResult;
    extern unsigned char    _dAQueryCGLoopSent;
    extern unsigned char    _dAQueryMissingSAddrSent;
    extern BOOL             _dAQueryMissingSAddrResult;
    extern unsigned char    _dAInitializeSent;
    extern unsigned char    _dAWithdrawSent;
    extern unsigned char    _dATerminateSent;
    extern unsigned char    _dARandomizeSent;
    extern unsigned char    _dAQuerySAddrSent;
    extern BOOL             _dAQuerySAddrReplyValid;
    extern unsigned char    _dAQuerySAddrReplyAnswer;
		// Internal.
		unsigned char 					_dASRAReintentosCnt;		
		unsigned char   				_dASRAStage;																										// Varible utilizada para el algoritmo de búsqueda.
    BOOL            				_dASRAFound;    																								// Varible utilizada para el algoritmo de búsqueda.
    BOOL            				_dASRARandomizeIsNeeded;																				// Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAInitializeMode;																						// Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRABusqueda;                                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAContador;                                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloSuperiorSearchH;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloInferiorSearchH;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloSuperiorSearchM;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloInferiorSearchM;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloSuperiorSearchL;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloInferiorSearchL;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAHalladoSearchH;                                           // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAHalladoSearchM;                                           // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAHalladoSearchL;                                           // Varible utilizada para el algoritmo de búsqueda.		
		unsigned short int			_dASRAGlobalProcTimeout;
		unsigned char						_dASRADeviceProcTimeout;    
		unsigned char   				_dASRAStatus;																										// Salida
    unsigned char   				_dASRAResult;																										// Salida	
		unsigned char   				_dASRAExitCode;																									// Salida	
		unsigned char 					_dASRAEncontradosCnt;                                           // Salida
    unsigned char 					_dASRAEncontradosList[4][64];                                   // Salida		
		
		
		unsigned char dASRAEncontradosHGet(unsigned char equipo)
		{
			return (_dASRAEncontradosList[0][equipo]);
		}
		unsigned char dASRAEncontradosMGet(unsigned char equipo)
		{
			return (_dASRAEncontradosList[1][equipo]);
		}
		unsigned char dASRAEncontradosLGet(unsigned char equipo)
		{
			return (_dASRAEncontradosList[2][equipo]);
		}
		
		
		void dASRARtcTick (void)
		{

		}
		
		
		static void dASRAGotoStage (unsigned char stage)
    {
        _dAInitializeSent=NOT_SENDING;
        _dARandomizeSent=NOT_SENDING;
        _dATerminateSent=NOT_SENDING;
        _dASearchAddrHSent=NOT_SENDING;
        _dASearchAddrMSent=NOT_SENDING;
        _dASearchAddrLSent=NOT_SENDING;
        _dACompareSent=NOT_SENDING;
        _dAWithdrawSent=NOT_SENDING;
        _dAQuerySAddrSent=NOT_SENDING;
        _dASRAStage=stage;
    }
    
		BOOL dASRAStart (unsigned char initializeMode, BOOL randomizeIsNeeded)
    {
				// también debería devolverte que NO en el caso de que el transceptor esté bloqueado para otra cosa.
				BOOL retVal;
       
        return retVal;
    }
    
		
		
		
		
		
		
		
		void dASRAStop (void)
		{
				if (_dASRAStatus==IN_PROGRESS)
				{
					dASRAGotoStage(STAGE_14);				
				}
		}
		
		
		
		void dASRADoneFlagClear (void)
		{
			_dASRAStatus=NOT_STARTED;
		}
		
		
		
		void dASRADoTask (void)
    {

    }


