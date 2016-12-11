/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "screen.h"
#include "sched.h"

// servicio de sistema fondear
void game_fondear(unsigned int ancla_dir_fisica, unsigned int cr3) {
    // mapeamos nueva pagina ancla
    mmu_mapear_pagina(TASK_ANCLA, cr3, ancla_dir_fisica, 0, 0);
    // desregistramos el ancla que acabamos de cambiar (para actualizar la pantalla)
    desregistrar_memoria_tarea(dame_tarea_actual(), 3);
    // registramos la nueva ancla (para actualizar la pantalla)
    registar_memoria_tarea(dame_tarea_actual(), 3, ancla_dir_fisica);
    // imprimimos si estamos en modo mapa
    if (es_modo_mapa() == 1)
        print_modo_mapa();
}

// servicio de sistema cañonear
void game_canonear(unsigned int dir_misil_fisica, unsigned int dir_buffer_absoluta) {
    // pegamos el buffer de 97 bytes en la direccion indicada
    char (*memoria_origen)[97] = (char (*)[97]) dir_buffer_absoluta;
    char (*memoria_destino)[97] = (char (*)[97]) dir_misil_fisica;
    for (int i = 0; i < 97; i++) {
        (*memoria_destino)[i] = (*memoria_origen)[i];
    }
    // registramos nuevo ultimo misil (para actualizar la pantalla)
    nuevo_ultimo_misil(dir_misil_fisica);
}

// servicio de sistema navegar
void game_navegar(unsigned int dir_primera_pag_fisica, unsigned int dir_segunda_pag_fisica, unsigned int cr3) {
    // copiamos las 2 paginas en la direcciones destino indicadas
    unsigned char* codigoPag1 = (unsigned char*) TASK_CODE_PAGE1;
    unsigned char* destinoPag1 = (unsigned char*) dir_primera_pag_fisica;
    unsigned char* codigoPag2 = (unsigned char*) TASK_CODE_PAGE2;
    unsigned char* destinoPag2 = (unsigned char*) dir_segunda_pag_fisica;
    for (int i = 0; i < TASK_SIZE; i++){
        destinoPag1[i] = codigoPag1[i];
        destinoPag2[i] = codigoPag2[i];
    }
    // mapeamos nueva pagina 1
    mmu_mapear_pagina(TASK_CODE_PAGE1, cr3, dir_primera_pag_fisica, 1, 1);
    // desregistramos pagina 1 que acabamos de cambiar (para actualizar la pantalla)
    desregistrar_memoria_tarea(dame_tarea_actual(), 1);
    // registramos la nueva pagina 1 (para actualizar la pantalla)
    registar_memoria_tarea(dame_tarea_actual(), 1, dir_primera_pag_fisica);
    // mapeamos nueva pagina 2
    mmu_mapear_pagina(TASK_CODE_PAGE2, cr3, dir_segunda_pag_fisica, 1, 1);
    // desregistramos pagina 2 que acabamos de cambiar (para actualizar la pantalla)
    desregistrar_memoria_tarea(dame_tarea_actual(), 2);
    // registramos la nueva pagina 2 (para actualizar la pantalla)
    registar_memoria_tarea(dame_tarea_actual(), 2, dir_segunda_pag_fisica);
    // imprimimos si estamos en modo mapa
    if (es_modo_mapa() == 1)
        print_modo_mapa();
}

// servicio de sistema que dependiendo del codigo dado llama a la funcion correspondiente
void game_service(unsigned int codigo_servico, unsigned int dir_fisica1, unsigned int dir_fisica2, unsigned int cr3) {
    if (codigo_servico == 0x923) {
        game_fondear(dir_fisica1, cr3);
    } else if (codigo_servico == 0x83A) {
        game_canonear(dir_fisica1, dir_fisica2);
    } else {
        game_navegar(dir_fisica1, dir_fisica2, cr3);
    }
}


