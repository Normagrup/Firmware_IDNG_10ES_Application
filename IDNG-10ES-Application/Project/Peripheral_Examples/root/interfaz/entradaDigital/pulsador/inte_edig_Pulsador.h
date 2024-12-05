/*
 * pulsador.h
 *
 * Created: 02/01/2018 7:39:24
 *  Author: DIEGOFH
 */ 


#ifndef PULSADOR_H_
#define PULSADOR_H_

void			pulsadorFlancoAscendenteHandler (unsigned char input);
void			pulsadorFlancoDescendenteHandler (unsigned char input, unsigned char nivelAltoLargo);
void			pulsadorNivelAltoLargoHandler (unsigned char input);
void			pulsadorTick (void);
void			pulsadorInit (unsigned char input, unsigned char modo);
//unsigned char	pulsadorTxPendingGet (void);
unsigned char	pulsadorStatusRegisterGet (unsigned char input);





#endif /* PULSADOR_H_ */