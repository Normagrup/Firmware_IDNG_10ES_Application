#define DALI_FRAME_TYPE_SINGLE_SEND                 1
#define DALI_FRAME_TYPE_BATCH_SEND                  0
#define DALI_FRAME_TYPE_SINGLE_RECEIVE							3
#define DALI_FRAME_TYPE_BATCH_RECEIVE								2



#define DALI_BUFFERS_CNT														8
#define DALI_SENSORS																0
#define DALI_NORMALINK															1
#define DALI_WEBSERVER															2
#define DALI_TEST																		3
#define DALI_TEMP																		4
#define DALI_ADDRESSING															5
#define DALI_UTILS																	6
#define DALI_POLLINGS																7





static unsigned char daliSendInProgressGet(void);
static unsigned char daliPacketIdCntGet (void);
static void daliPacketIdCntInc (void);

unsigned char daliAccessHandlerAllowOnlyThisModule (unsigned char module, unsigned char stop, unsigned char wait);
void daliAccessHandlerAllowAllModules (void);


unsigned char daliSend(unsigned char module, unsigned int senderPacketId, unsigned char frameType, unsigned char dest, unsigned char *apply, unsigned char commandHigh, unsigned char commandLow);


unsigned char daliDestCorrecto (unsigned char dest);

unsigned char daliPollingSend (unsigned char frameType, unsigned char *apply, unsigned char commandHigh, unsigned char commandLow);
void daliStop (void);
void daliDoTask (void);
