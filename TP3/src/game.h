/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "mmu.h"

void game_fondear(unsigned int ancla_dir_fisica, unsigned int cr3);
void game_canonear(unsigned int dir_misil_fisica, unsigned int dir_buffer_absoluta);
void game_navegar(unsigned int dir_primera_pag_fisica, unsigned int dir_segunda_pag_fisica, unsigned int cr3);
void game_service(unsigned int codigo_servico, unsigned int dir_fisica1, unsigned int dir_fisica2, unsigned int cr3);

#endif  /* !__GAME_H__ */
