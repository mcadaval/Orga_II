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
unsigned short obtener_siguiente_tarea_viva(unsigned int desde);
void matar_tarea();

#endif	/* !__SCHED_H__ */
