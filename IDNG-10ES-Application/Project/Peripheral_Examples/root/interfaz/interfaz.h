/*
 * interfaz.h
 *
 * Created: 02/01/2018 7:54:27
 *  Author: DIEGOFH
 */ 


#ifndef INTERFAZ_H_
#define INTERFAZ_H_


unsigned char interfazTxPendingGet (void);
unsigned char interfazStatusRegisterGet (void);
void interfazTick (void);
void interfazInit(void);
void interfazModoUpdt (void);
void interfazModoSet (unsigned char modo);
void interfazSubmodoSet (unsigned char submodo);
unsigned char interfazDeshabilitadoGet (void);
void interfazHabilitar (void);
void interfazDeshabilitar (void);
unsigned char interfazModoGet (void);
unsigned char interfazSubmodoGet (void);
unsigned char interfazLevelGet (void);
void interfazDoTask (void);




#define REGISTRO_ESTADO_BIT_SHORT_PRESS                 0x01
#define REGISTRO_ESTADO_BIT_LONG_PRESS                  0x02
#define REGISTRO_ESTADO_BIT_LONG_RELEASE                0x04
#define REGISTRO_ESTADO_BIT_PRESENCY                    0x08
#define REGISTRO_ESTADO_BIT_VACANCY                     0x10
#define REGISTRO_ESTADO_BIT_REPORTE_LUZ                 0x20



#define AVG_SHIFT			3
#define MODO_INTERFAZ_NO_OPERATIVO								0x00	// 0	//0b000000/000
#define MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE			0x08	// 1	//0b000001/000
#define MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_DESCENDENTE			0x10	// 2	//0b000010/000
#define MODO_INTERFAZ_PULSADOR_SHORT_LONG						0x18	// 3	//0b000011/000
#define MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO					0x20	// 4	//0b000100/000
#define MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL					0x28	// 5	//0b000101/000
#define MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_LUZ			0x30	// 6	//0b000110/000
#define MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_LUZ			0x38	// 7	//0b000111/000
#define MODO_INTERFAZ_REG_LUZ									0x40	// 8	//0b001000/000


//#define DPR_TIM_INIT											2000	// Durante un tiempo inicial no se envian eventos.
//#define DPR_TIM_INIT_INC										0000	// Esto es función de la dirección para evitar un flooding.

//#define DPR_TIM_KEEP_ALIVE_CNT_PRELOAD							4000	// En este periodo cuando estamos funcionando por nivel, volvemos marcamos el flag para que nos consulten el registro de estado.
//#define DPR_TIM_KEEP_ALIVE_CNT_STARTUP							2000	// Al arranque quizás es conveniente hacerlo más rapido.
//#define DPR_TIM_KEEP_ALIVE_CNT_STARTUP_INC						0000	// Esto es función de la dirección para evitar un flooding

#define NIVEL_ALTO_CNT_LONG_PRESS								200	

#define DPR_TIM_INIT											31	// Durante un tiempo inicial no se envian eventos.
#define DPR_TIM_INIT_INC										10	// Esto es función de la dirección para evitar un flooding.

#define PUL_TIM_INIT											31	// Durante un tiempo inicial no se envian eventos.
#define PUL_TIM_INIT_INC										10	// Esto es función de la dirección para evitar un flooding.

#define DPR_TIM_KEEP_ALIVE_CNT_PRELOAD							1000	// En este periodo cuando estamos funcionando por nivel, volvemos marcamos el flag para que nos consulten el registro de estado.
#define DPR_TIM_KEEP_ALIVE_CNT_STARTUP							100	// Al arranque quizás es conveniente hacerlo más rapido.
#define DPR_TIM_KEEP_ALIVE_CNT_STARTUP_INC						10	// Esto es función de la dirección para evitar un flooding

#define REG_LUZ_TIM_KEEP_ALIVE_CNT_PRELOAD						1000	// En este periodo cuando estamos funcionando por nivel, volvemos marcamos el flag para que nos consulten el registro de estado.
#define REG_LUZ_KEEP_ALIVE_CNT_STARTUP							100	// Al arranque quizás es conveniente hacerlo más rapido.
#define REG_LUZ_KEEP_ALIVE_CNT_STARTUP_INC						10



				#define REGISTRO_ESTADO_BIT_SHORT_PRESS                 0x01
				#define REGISTRO_ESTADO_BIT_LONG_PRESS                  0x02
				#define REGISTRO_ESTADO_BIT_LONG_RELEASE                0x04
				#define REGISTRO_ESTADO_BIT_PRESENCY                    0x08
				#define REGISTRO_ESTADO_BIT_VACANCY                     0x10
				#define REGISTRO_ESTADO_BIT_REPORTE_LUZ                 0x20														// En todos estos tiempos 4000 son 8 segundos.


void interfazEventoGenerar (unsigned char registroEstado, unsigned char input);









#endif /* INTERFAZ_H_ */