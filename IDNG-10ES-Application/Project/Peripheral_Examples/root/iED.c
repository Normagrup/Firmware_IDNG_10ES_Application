#include "stm32f0xx.h"
#include "dali.h"
#include "iED.h"



unsigned char 						_iEDi1Estado;
unsigned char 						_iEDi2Estado;
unsigned char 						_iEDi1EstadoPin;
unsigned char 						_iEDi2EstadoPin;
volatile unsigned char 		_iEDOverflow;
unsigned int 							_iEDi1EstadoAbiertoCnt;
unsigned int 							_iEDi1EstadoCerradoCnt;
unsigned int 							_iEDi2EstadoAbiertoCnt;
unsigned int 							_iEDi2EstadoCerradoCnt;

void iEDTick (void)
{
}
void iEDDoTask (void)
{
}