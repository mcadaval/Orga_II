/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "defines.h"

void sched_inicializar();
unsigned short sched_proximo_indice();
unsigned short dame_tarea_actual();
unsigned short obtener_siguiente_tarea_viva(unsigned int desde);
void matar_tarea();
void actualizar_flag_idle();
unsigned char tarea_activa(unsigned int tarea);
unsigned char es_tarea();
unsigned char actual_es_idle();
unsigned short dame_tarea_no_idle();


#endif	/* !__SCHED_H__ */
