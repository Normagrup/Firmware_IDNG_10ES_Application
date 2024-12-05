#define HISTORICO_FALLO_COMUNICACION									0x01
#define HISTORICO_FALLO_BATERIA												0x02
#define HISTORICO_FALLO_LAMPARA												0x03
#define HISTORICO_FALLO_AUTONOMIA											0x04
#define HISTORICO_FALLO_NO_FT													0x06
#define HISTORICO_FALLO_NO_DT													0x07
#define HISTORICO_MAP_0_PTR														0x0FE0
#define HISTORICO_COMPLETED_PTR												0x0FFF			
#define HISTORICO_COMPLETED_BYTE_PTR  								0xFF
#define HISTORICO_TIMEOUT_TICKS_LIMIT									5000


unsigned char historicoRead (unsigned int readPtr, unsigned char *data);
unsigned char historicoFlashMemPosGet (unsigned char currentPage, unsigned int ptr, unsigned char *flashMemPos);
void historicoPtrInc(unsigned char *currentPage, unsigned int *ptr);
void historicoInit (void);
unsigned char historicoAdd (unsigned char loop, unsigned char shortAddress, unsigned char day, unsigned char month, unsigned char year, unsigned char hour, unsigned char minutes, unsigned char event);
unsigned char historicoDataGet (unsigned char page, unsigned int ptr, unsigned char *data);
void historicoTick (void);
