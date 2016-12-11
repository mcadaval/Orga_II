/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "screen.h"
#include "i386.h"
#include "defines.h"

static void* siguinte_pagina_libre_tierra = (void*) 0x30000;
static void* siguinte_pagina_libre_mar = (void*) 0x100000;

// devuelve la posición de memoria de la siguiente pagina de 4KB libre en memoria "tierra"
void* dame_pagina_libre_tierra() {
    void* temp = siguinte_pagina_libre_tierra;
    siguinte_pagina_libre_tierra += TAMANO_PAGINA;
    return temp;
}

// devuelve la posición de memoria de la siguiente pagina de 4KB libre en memoria "mar"
void* dame_pagina_libre_mar() {
    void* temp = siguinte_pagina_libre_mar;
    siguinte_pagina_libre_mar += TAMANO_PAGINA;
    return temp;
}

// copia una pagina (4KB) a partir de la posicion origen en destino 
void copiar_pagina(unsigned int origen, unsigned int destino) {
    char (*memoria_origen)[TAMANO_PAGINA] = (char (*)[TAMANO_PAGINA]) origen;
    char (*memoria_destino)[TAMANO_PAGINA] = (char (*)[TAMANO_PAGINA]) destino;
    for (int i = 0; i < TAMANO_PAGINA; i++) {
        (*memoria_destino)[i] = (*memoria_origen)[i];
    }
}

// mapea todas las paginas de kernel con identity mapping utilizando como tabla de paginas la pagina 0x27000 
void mmu_inicializar_dir_kernel(unsigned int cr3) {

    pd_entry* directorio_paginas = (pd_entry *) cr3;    
    
    // seteamos el present de todas las entradas en 0 para poder distinguir si ya fueron utilizadas
    for (int i = 0; i < 1024; i++)
        directorio_paginas[i].p = 0;

    // seteamos manualmente la entrada 0 en el directorio de paginas porque tiene que usar una direccion de memoria determinada para la tabla
    // para el resto podemos usar mmu_mapear_pagina que pide pagina de tierra automaticamente de ser necesario
    directorio_paginas[0].p = 1;
    directorio_paginas[0].rw = 1;
    directorio_paginas[0].us = 0;
    directorio_paginas[0].pwt = 0;
    directorio_paginas[0].pcd = 0;
    directorio_paginas[0].a = 0;
    directorio_paginas[0].i = 0;
    directorio_paginas[0].ps = 0;
    directorio_paginas[0].g = 0;
    directorio_paginas[0].avl = 0;
    directorio_paginas[0].table_addr = PAGE_TABLE0_ADDR >> 12;

    unsigned int dir_memoria = 0x0;
    unsigned char rw = 1;
    unsigned char us = 0;
    // identity mapping de los primeros 1,75GB
    for (int i = 0; i < PAGS_TOTALES; i++) {
        mmu_mapear_pagina(dir_memoria, cr3, dir_memoria, us, rw);
        dir_memoria += TAMANO_PAGINA;
    }
}

// mapea todas las paginas de kernel con identity mapping. Utilizada para las el mapeo de kernel de las tareas
void mmu_inicializar_dir_kernel_tarea(unsigned int cr3) {
    
    pd_entry* directorio_paginas = (pd_entry *) cr3;    
   
    // seteamos el present de todas las entradas en 0 para poder distinguir si ya fueron utilizadas
    for (int i = 0; i < 1024; i++)
        directorio_paginas[i].p = 0;

    unsigned int dir_memoria = 0x0;
    unsigned char rw = 1;
    unsigned char us = 0;
    // identity mapping de los primeros 1,75GB
    for (int i = 0; i < PAGS_TOTALES; i++) {
        mmu_mapear_pagina(dir_memoria, cr3, dir_memoria, us, rw);
        dir_memoria += TAMANO_PAGINA;
    }
}

// mapea todas las 2 paginas que utiliza la tarea idle
void mmu_inicializar_dir_idle() {
    // mapeamos pagina 1 
    mmu_mapear_pagina(TASK_CODE_PAGE1, PAGE_DIRECTORY_ADDR, TASK_IDLE_CODE_PAGE1, 0, 1);
    // mapeamos pagina 2 
    mmu_mapear_pagina(TASK_CODE_PAGE2, PAGE_DIRECTORY_ADDR, TASK_IDLE_CODE_PAGE2, 0, 1);
}

// inicializa todo el mapa de memoria de una tarea a partir de una direccion fisica donde mapear la primera pagina (en la siguiente mapea la segunda). El ancla siempre se mapea en 0x0
unsigned int mmu_inicializar_dir_tarea(unsigned int tarea, unsigned int fisica) {
    // pedimos una pagina para el directorio de paginas de la tarea
    unsigned int directorio_paginas = (unsigned int) dame_pagina_libre_tierra();

    mmu_inicializar_dir_kernel_tarea(directorio_paginas);

    // mapeamos pagina 1 (nivel 3) y guardamos direccion fisica en estructura para poder imprimir en pantalla
    mmu_mapear_pagina(TASK_CODE_PAGE1, directorio_paginas, fisica, 1, 1);
    registar_memoria_tarea(tarea, 1, fisica);

    // mapeamos pagina 2 (nivel 3) y guardamos direccion fisica en estructura para poder imprimir en pantalla
    mmu_mapear_pagina(TASK_CODE_PAGE2, directorio_paginas, fisica + TAMANO_PAGINA, 1, 1);
    registar_memoria_tarea(tarea, 2, fisica + TAMANO_PAGINA);
    
    // mapeamos pagina ancla (nivel 3) y guardamos direccion fisica en estructura para poder imprimir en pantalla
    mmu_mapear_pagina(TASK_ANCLA, directorio_paginas, TASK_ANCLA_FIS, 1, 0);
    registar_memoria_tarea(tarea, 3, TASK_ANCLA_FIS);
    
    unsigned int direccion_codigo_tarea = (tarea - 1) * TASK_SIZE + TASK_1_CODE_SRC_ADDR;

    // copia primer pagina de la tarea a la memoria indicada
    copiar_pagina(direccion_codigo_tarea, fisica);
    // copia segunda pagina de la tarea a la memoria indicada
    copiar_pagina(direccion_codigo_tarea + TAMANO_PAGINA, fisica + TAMANO_PAGINA);

    return directorio_paginas;
}

// mapea la pagina fisica a la virtual dada utilizando como directorio de paginas el referenciado por el cr3 dado. us y rw son los niveles de user/supervisor y read/write respectivamente
void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, unsigned char us, unsigned char rw) {
    // accedemos a la entrada correspondiente en el directorio de paginas
    int pd_index = PDE_INDEX(virtual);
    pd_entry* directorio_paginas = (pd_entry*) cr3; 

    // verificamos si la entrada ya fue seteada previamente, y en caso de que no, la seteamos
    if (directorio_paginas[pd_index].p == 0) {
        directorio_paginas[pd_index].p = 1;
        directorio_paginas[pd_index].rw = rw;
        directorio_paginas[pd_index].us = us;
        directorio_paginas[pd_index].pwt = 0;
        directorio_paginas[pd_index].pcd = 0;
        directorio_paginas[pd_index].a = 0;
        directorio_paginas[pd_index].i = 0;
        directorio_paginas[pd_index].ps = 0;
        directorio_paginas[pd_index].g = 0;
        directorio_paginas[pd_index].avl = 0;
        directorio_paginas[pd_index].table_addr = ((unsigned int) dame_pagina_libre_tierra()) >> 12;

        // obtenemos la direccion de la tabla de paginas en la cual debemos mapear la pagina
        pt_entry* tabla_paginas = (pt_entry*) (directorio_paginas[pd_index].table_addr << 12);
        
        // seteamos en 0 el present de todas las entradas para no mapear memoria que no deseamos
        for (int i = 0; i < 1024; i++)
            tabla_paginas[i].p = 0;
    }

    // obtenemos la direccion de la tabla de paginas en la cual debemos mapear la pagina
    pt_entry* tabla_paginas = (pt_entry*) (directorio_paginas[pd_index].table_addr << 12);

    // calculamos el indice en la tabla de paginas de acuerdo a virtual
    int pt_index = PTE_INDEX(virtual);

    // mapeamos memoria seteando entrada en la tabla de paginas 
    tabla_paginas[pt_index].p = 1;
    tabla_paginas[pt_index].rw = rw;
    tabla_paginas[pt_index].us = us;
    tabla_paginas[pt_index].pwt = 0;
    tabla_paginas[pt_index].pcd = 0;
    tabla_paginas[pt_index].a = 0;
    tabla_paginas[pt_index].d = 0;
    tabla_paginas[pt_index].pat = 0;
    tabla_paginas[pt_index].g = 0;
    tabla_paginas[pt_index].avl = 0;
    tabla_paginas[pt_index].page_addr = fisica >> 12;

    tlbflush();
}
        
// desmapea la pagina virtual dada utilizando del directorio de paginas referenciado por el cr3 dado
void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3) {  
    // accedemos a la entrada correspondiente en el directorio de paginas
    int pd_index = PDE_INDEX(virtual);
    pd_entry* p = (pd_entry*) cr3; 

    // obtenemos la direccion de la tabla de paginas en la cual esta mapeada la pagina buscada
    pt_entry* tabla_paginas = (pt_entry*) (p[pd_index].table_addr << 12);

    // calculamos el indice en la tabla de paginas de acuerdo a virtual
    int pt_index = PTE_INDEX(virtual);

    // unmapeamos memoria poniendo todo en 0
    tabla_paginas[pt_index].p = 0;
    tabla_paginas[pt_index].rw = 0;
    tabla_paginas[pt_index].us = 0;
    tabla_paginas[pt_index].pwt = 0;
    tabla_paginas[pt_index].pcd = 0;
    tabla_paginas[pt_index].a = 0;
    tabla_paginas[pt_index].d = 0;
    tabla_paginas[pt_index].pat = 0;
    tabla_paginas[pt_index].g = 0;
    tabla_paginas[pt_index].avl = 0;
    tabla_paginas[pt_index].page_addr = 0;    
}