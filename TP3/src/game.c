/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "screen.h"
#include "sched.h"

void game_fondear(unsigned int ancla_dir_fisica, unsigned int cr3) {
    // unsigned char* ancla_fuente = (unsigned char*) 0x40002000;
    // unsigned char* ancla_destino = (unsigned char*) ancla_dir_fisica;
    // for (int i = 0; i < 4096; i++){
    //     ancla_destino[i] = ancla_fuente[i];
    // }
    // breakpoint();
    mmu_mapear_pagina(0x40002000, cr3, ancla_dir_fisica, 0, 0);
    // breakpoint();
    // desregistrar_memoria_tarea(dame_tarea_actual(), 3);
    // breakpoint();
    registar_memoria_tarea(dame_tarea_actual(), 3, ancla_dir_fisica);
}

void game_canonear(unsigned int dir_misil_fisica, unsigned int dir_buffer_absoluta) {
    char (*memoria_origen)[97] = (char (*)[97]) dir_buffer_absoluta;
    char (*memoria_destino)[97] = (char (*)[97]) dir_misil_fisica;
    for (int i = 0; i < 97; i++) {
        (*memoria_destino)[i] = (*memoria_origen)[i];
    }
}

void game_navegar(unsigned int dir_primera_pag_fisica, unsigned int dir_segunda_pag_fisica, unsigned int cr3) {
    
    unsigned char* codigoPag1 = (unsigned char*) 0x40000000;
    unsigned char* destinoPag1 = (unsigned char*) dir_primera_pag_fisica;
    unsigned char* codigoPag2 = (unsigned char*) 0x40001000;
    unsigned char* destinoPag2 = (unsigned char*) dir_segunda_pag_fisica;
    for (int i = 0; i < 4096; i++){
        destinoPag1[i] = codigoPag1[i];
        destinoPag2[i] = codigoPag2[i];
    }

    mmu_mapear_pagina(0x40000000, cr3, dir_primera_pag_fisica, 1, 1);
    // desregistrar_memoria_tarea(dame_tarea_actual(), 1);
    registar_memoria_tarea(dame_tarea_actual(), 1, dir_primera_pag_fisica);
    mmu_mapear_pagina(0x40001000, cr3, dir_segunda_pag_fisica, 1, 1);
    // desregistrar_memoria_tarea(dame_tarea_actual(), 2);
    registar_memoria_tarea(dame_tarea_actual(), 2, dir_segunda_pag_fisica);
}

void game_service(unsigned int codigo_servico, unsigned int dir_fisica1, unsigned int dir_fisica2, unsigned int cr3) {
    if (codigo_servico == 0x923) {
        game_fondear(dir_fisica1, cr3);
    } else if (codigo_servico == 0x83A) {
        game_canonear(dir_fisica1, dir_fisica2);
    } else {
        game_navegar(dir_fisica1, dir_fisica2, cr3);
    }
}


