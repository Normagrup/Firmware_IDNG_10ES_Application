#include "daliCommands.h"
#include "dali.h"
#include "daliLoop.h"
#include "config.h"




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



#define COMANDO_DALI_QUERY_TYPE														0x00
#define	COMANDO_DALI_QUERY_ENABLED													0x10
#define	COMANDO_DALI_QUERY_MODE														0x11

#define	COMANDO_DALI_QUERY_SERIAL_1													0x02
#define	COMANDO_DALI_QUERY_SERIAL_2													0x03
#define	COMANDO_DALI_QUERY_SERIAL_3													0x04
#define	COMANDO_DALI_QUERY_SERIAL_4													0x05


unsigned char _querySensorType;
unsigned char _querySensorEnabled;
unsigned char _querySensorMode;
unsigned char _querySensorSubMode;
unsigned char _querySensorSerial[4];


unsigned char _sensorData[8];


unsigned char _sensorScanningStatus;
unsigned char _sensorScanningStage;

unsigned char _err;

unsigned char _num;
unsigned char _cnt;

unsigned char _sensorScanningIp[4];

unsigned char _rei;


void SendQuerySensorType (unsigned char dest)
{
}
void SendQuerySensorEnabled (unsigned char dest)
{
}
void SendQuerySensorMode (unsigned char dest)
{
}
void SendQuerySensorSerial1 (unsigned char dest)
{
}
void SendQuerySensorSerial2 (unsigned char dest)
{
}
void SendQuerySensorSerial3 (unsigned char dest)
{
}
void SendQuerySensorSerial4 (unsigned char dest)
{
}
void QuerySensorTypeCb (unsigned char command, unsigned char valid, unsigned char answer)
{
}
void QuerySensorEnabledCb (unsigned char command, unsigned char valid, unsigned char answer)
{
}
void QuerySensorModeCb (unsigned char command, unsigned char valid, unsigned char answer)
{
}
void QuerySensorSerial1Cb (unsigned char command, unsigned char valid, unsigned char answer)
{
}
void QuerySensorSerial2Cb (unsigned char command, unsigned char valid, unsigned char answer)
{
}
void QuerySensorSerial3Cb (unsigned char command, unsigned char valid, unsigned char answer)
{
}
void QuerySensorSerial4Cb (unsigned char command, unsigned char valid, unsigned char answer)
{
}
void sensorScanningGotoStage(unsigned char stage)
{
}
void sensorScanningNotify(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, unsigned char byte5, unsigned char byte6)
{	
}
void sensorScanningStart (unsigned char *ip)
{	
}
void sensorScanningDoTask (void)
{
}