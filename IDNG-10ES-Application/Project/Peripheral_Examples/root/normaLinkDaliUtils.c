// 01 00 00 00 FF E4 00 00			Reboot
// 01 00 00 00 FF E4 10 00			daliBootloaderModelHGet
// 01 00 00 00 FF E4 11 00			daliBootloaderModelMGet
// 01 00 00 00 FF E4 12 00			daliBootloaderVersionGet
// 01 00 00 00 FF E4 13 00			daliBootloaderStatusGet
// 03 00 00 00 FF E4 10 DD 00 	daliBootloaderModelHSet
// 03 00 00 00 FF E4 11 DD 00 	daliBootloaderModelMSet
// 03 00 00 00 FF E4 12 DD 00 	daliBootloaderVersionSet
// 03 00 00 00 FF E4 13 DD 00 	daliBootloaderStatusSet




#include "stm32f0xx.h"
#include "normalink.h"
#include "ethFramming.h"
#include "daliUtils.h"


extern unsigned char recibireth[320];		// Búffer para paquete entrante
extern unsigned char ipDest[4];	//Ip del host que me envía el paquete
extern unsigned int portDest;		//Puerto del host que me envía el paquete

void normaLinkDaliUtilsT3 (unsigned char comL, unsigned char dest, unsigned char data)
{

}

void normaLinkDaliUtilsT4 (unsigned char comL)
{
	
}

void normaLinkDaliUtilsT1 (unsigned char comL, unsigned char dest)
{
}
