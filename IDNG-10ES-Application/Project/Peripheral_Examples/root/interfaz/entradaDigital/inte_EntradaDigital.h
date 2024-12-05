/*
 * entradaDigital.h
 *
 * Created: 02/01/2018 7:42:01
 *  Author: DIEGOFH
 */ 


#ifndef ENTRADADIGITAL_H_
#define ENTRADADIGITAL_H_


unsigned char entradaDigitalTxPendingGet(void);
unsigned char entradaDigitalStatusRegisterGet(unsigned char input);
void entradaDigitalInit (unsigned char input, unsigned char modo);
void entradaDigitalTick (void);
void entradaDigitalDoTask (unsigned char input);





#endif /* ENTRADADIGITAL_H_ */