/*
 * detectoresPresencia.h
 *
 * Created: 02/01/2018 7:37:14
 *  Author: DIEGOFH
 */ 


#ifndef DETECTORESPRESENCIA_H_
#define DETECTORESPRESENCIA_H_

void			detPresFlancoAscendenteHandler (unsigned char input);
void			detPresNivelAltoHandler (unsigned char input);
void			detPresNivelBajoHandler (unsigned char input);
void			detPresTick(void);
//unsigned char	detPresTxPendingGet (void);
//unsigned char	detPresStatusRegisterGet (void);
void			detPresPresency (unsigned char input);
void			detPresVacancy (unsigned char input);
void			detPresInit (unsigned char input, unsigned char modo);



#endif /* DETECTORESPRESENCIA_H_ */