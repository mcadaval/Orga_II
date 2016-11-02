/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

static void* siguinte_pagina_libre = (void*) 0x30000;

// devuelve la posición de memoria de la siguiente pagina de 4KB libre en memoria "tierra"
void* dame_pagina_libre() {
    void* temp = siguinte_pagina_libre;
    siguinte_pagina_libre += 4096;
    return temp;
}

// copia una pagina (4KB) a partir de la posicion origen en destino 
// @todo
void copiar_pagina(unsigned int origen, unsigned int destino) {
    // char (*memoria_origen)[4096] = (char (*)[4096]) origen;
    // char (*memoria_destino)[4096] = (char (*)[4096]) destino;
    // for (int i = 0; i < 4096; i++) {
    //     (*memoria_destino)[i] = (*memoria_origen)[i];
    // }
}

void mmu_inicializar_dir_kernel(unsigned int cr3) {
    // pd_entry* directorio_paginas = (pd_entry *) PAGE_DIRECTORY_ADDR;
    pd_entry* directorio_paginas = (pd_entry *) cr3;    
    
    // seteamos en 0 todas las entradas para poder distinguir si las entradas ya fueron utilizadas
    for (int i = 0; i < 1024; i++) {
        directorio_paginas[i].p = 0;
        directorio_paginas[i].rw = 0;
        directorio_paginas[i].us = 0;
        directorio_paginas[i].pwt = 0;
        directorio_paginas[i].pcd = 0;
        directorio_paginas[i].a = 0;
        directorio_paginas[i].i = 0;
        directorio_paginas[i].ps = 0;
        directorio_paginas[i].g = 0;
        directorio_paginas[i].avl = 0;
        directorio_paginas[i].table_addr = 0;   
    }

    // seteamos entrada 0 en el directorio de paginas
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

    void* PAGE_TABLE1_ADDR = dame_pagina_libre();

    // seteamos entrada 1 en el directorio de paginas
    directorio_paginas[1].p = 1;
    directorio_paginas[1].rw = 1;
    directorio_paginas[1].us = 0;
    directorio_paginas[1].pwt = 0;
    directorio_paginas[1].pcd = 0;
    directorio_paginas[1].a = 0;
    directorio_paginas[1].i = 0;
    directorio_paginas[1].ps = 0;
    directorio_paginas[1].g = 0;
    directorio_paginas[1].avl = 0;
    directorio_paginas[1].table_addr = ((unsigned int) PAGE_TABLE1_ADDR) >> 12;
    
    pt_entry* tabla_paginas0 = (pt_entry *) PAGE_TABLE0_ADDR;
    for (int i = 0; i < PAGE_TABLE0_COUNT; i++) {
        tabla_paginas0[i].p = 1;
        tabla_paginas0[i].rw = 1;
        tabla_paginas0[i].us = 0;
        tabla_paginas0[i].pwt = 0;
        tabla_paginas0[i].pcd = 0;
        tabla_paginas0[i].a = 0;
        tabla_paginas0[i].d = 0;
        tabla_paginas0[i].pat = 0;
        tabla_paginas0[i].g = 0;
        tabla_paginas0[i].avl = 0;
        tabla_paginas0[i].page_addr = i;         
    }

    pt_entry* tabla_paginas1 = (pt_entry *) PAGE_TABLE1_ADDR;
    for (int i = 0; i < PAGE_TABLE1_COUNT; i++) {
        tabla_paginas1[i].p = 1;
        tabla_paginas1[i].rw = 1;
        tabla_paginas1[i].us = 0;
        tabla_paginas1[i].pwt = 0;
        tabla_paginas1[i].pcd = 0;
        tabla_paginas1[i].a = 0;
        tabla_paginas1[i].d = 0;
        tabla_paginas1[i].pat = 0;
        tabla_paginas1[i].g = 0;
        tabla_paginas1[i].avl = 0;
        tabla_paginas1[i].page_addr = i + 1024;         
    }

    /* Para los restantes 0.5 MB = 1024 - PAGE_TABLE1_COUNT = 1024 - 896 = 128 (descriptores de Tablas de Paginas) de la 2da tabla dejamos present en 0 */
    for (int i = PAGE_TABLE1_COUNT; i < 1024; i++) {
        tabla_paginas1[i].p = 0;
        tabla_paginas1[i].rw = 0;
        tabla_paginas1[i].us = 0;
        tabla_paginas1[i].pwt = 0;
        tabla_paginas1[i].pcd = 0;
        tabla_paginas1[i].a = 0;
        tabla_paginas1[i].d = 0;
        tabla_paginas1[i].pat = 0;
        tabla_paginas1[i].g = 0;
        tabla_paginas1[i].avl = 0;
        tabla_paginas1[i].page_addr = 0;         
    }
}

unsigned int mmu_inicializar_dir_tarea(unsigned int tarea, unsigned int fisica) {
    // pedimos una pagina para el directorio de paginas de la tarea
    unsigned int directorio_paginas = (unsigned int) dame_pagina_libre();

    mmu_inicializar_dir_kernel(directorio_paginas);

    // mapeamos pagina 1 (nivel 3)
    mmu_mapear_pagina(0x40000000, directorio_paginas, fisica);
    // mapeamos pagina 2 (nivel 3)
    mmu_mapear_pagina(0x40001000, directorio_paginas, fisica + 0x1000);
    // mapeamos pagina ancla (nivel 3)
    mmu_mapear_pagina(0x40002000, directorio_paginas, 0x0);
    
    unsigned int direccion_codigo_tarea = (tarea - 1) * 8192 + 0x10000;
    // copia primer pagina de la tarea a la memoria indicada
    copiar_pagina(direccion_codigo_tarea, fisica);
    // copia segunda pagina de la tarea a la memoria indicada
    copiar_pagina(direccion_codigo_tarea + 0x1000, fisica + 0x1000);

    return directorio_paginas;
}

void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica) {
    // accedemos a la entrada correspondiente en el directorio de paginas
    int pd_index = PDE_INDEX(virtual);
    pd_entry* directorio_paginas = (pd_entry*) cr3; 

    // verificamos si la entrada ya fue seteada previamente, y en caso de que no, la seteamos
    if (directorio_paginas[pd_index].p == 0) {
        directorio_paginas[pd_index].p = 1;
        directorio_paginas[pd_index].rw = 1;
        directorio_paginas[pd_index].us = 0;
        directorio_paginas[pd_index].pwt = 0;
        directorio_paginas[pd_index].pcd = 0;
        directorio_paginas[pd_index].a = 0;
        directorio_paginas[pd_index].i = 0;
        directorio_paginas[pd_index].ps = 0;
        directorio_paginas[pd_index].g = 0;
        directorio_paginas[pd_index].avl = 0;
        directorio_paginas[pd_index].table_addr = ((unsigned int) dame_pagina_libre()) >> 12;

        // obtenemos la direccion de la tabla de paginas en la cual debemos mapear la pagina
        pt_entry* tabla_paginas = (pt_entry*) (directorio_paginas[pd_index].table_addr << 12);
        
        // seteamos en 0 todas las entradas para no mapear memoria que no deseamos
        for (int i = 0; i < 1024; i++) {
            tabla_paginas[i].p = 0;
            tabla_paginas[i].rw = 0;
            tabla_paginas[i].us = 0;
            tabla_paginas[i].pwt = 0;
            tabla_paginas[i].pcd = 0;
            tabla_paginas[i].a = 0;
            tabla_paginas[i].d = 0;
            tabla_paginas[i].pat = 0;
            tabla_paginas[i].g = 0;
            tabla_paginas[i].avl = 0;
            tabla_paginas[i].page_addr = 0;
        }
    }

    // obtenemos la direccion de la tabla de paginas en la cual debemos mapear la pagina
    pt_entry* tabla_paginas = (pt_entry*) (directorio_paginas[pd_index].table_addr << 12);

    // calculamos el indice en la tabla de paginas de acuerdo a virtual
    int pt_index = PTE_INDEX(virtual);

    // mapeamos memoria seteando entrada en la tabla de paginas 
    tabla_paginas[pt_index].p = 1;
    tabla_paginas[pt_index].rw = 1;
    tabla_paginas[pt_index].us = 0;
    tabla_paginas[pt_index].pwt = 0;
    tabla_paginas[pt_index].pcd = 0;
    tabla_paginas[pt_index].a = 0;
    tabla_paginas[pt_index].d = 0;
    tabla_paginas[pt_index].pat = 0;
    tabla_paginas[pt_index].g = 0;
    tabla_paginas[pt_index].avl = 0;
    tabla_paginas[pt_index].page_addr = fisica >> 12;
}
        

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