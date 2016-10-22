/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

static void* siguinte_pagina_libre_tierra = (void*) 0x30000;

// devuelve la posición de memoria de la siguiente pagina de 4KB libre en memoria "tierra"
void* dame_pagina_libre_tierra() {
    void* temp = siguinte_pagina_libre_tierra;
    siguinte_pagina_libre_tierra += 4096;
    return temp;
}

static void* siguinte_pagina_libre_mar = (void*) 0x100000;

// devuelve la posición de memoria de la siguiente pagina de 4KB libre en memoria "mar"
void* dame_pagina_libre_mar() {
    void* temp = siguinte_pagina_libre_mar;
    siguinte_pagina_libre_mar += 4096;
    return temp;
}

void mmu_inicializar() {
    pd_entry* p0 = (pd_entry *) PAGE_DIRECTORY_ADDR;
    
    p0[0].p = 1;
    p0[0].rw = 1;
    p0[0].us = 0;
    p0[0].pwt = 0;
    p0[0].pcd = 0;
    p0[0].a = 0;
    p0[0].i = 0;
    p0[0].ps = 0;
    p0[0].g = 0;
    p0[0].avl = 0;
    p0[0].table_addr = 0x28;

    void* tabla1 = dame_pagina_libre();

    p0[1].p = 1;
    p0[1].rw = 1;
    p0[1].us = 0;
    p0[1].pwt = 0;
    p0[1].pcd = 0;
    p0[1].a = 0;
    p0[1].i = 0;
    p0[1].ps = 0;
    p0[1].g = 0;
    p0[1].avl = 0;
    p0[1].table_addr = ((int) tabla1) >> 12;
    

    pt_entry* p1 = (pt_entry *) PAGE_TABLE0_ADDR;
    for (int i = 0; i < PAGE_TABLE0_COUNT; i++) {
        p1[i].p = 1;
        p1[i].rw = 1;
        p1[i].us = 0;
        p1[i].pwt = 0;
        p1[i].pcd = 0;
        p1[i].a = 0;
        p1[i].d = 0;
        p1[i].pat = 0;
        p1[i].g = 0;
        p1[i].avl = 0;
        p1[i].page_addr = i;         
    }

    pt_entry* p2 = (pt_entry *) tabla1;
    for (int i = 0; i < PAGE_TABLE1_COUNT; i++) {
        p2[i].p = 1;
        p2[i].rw = 1;
        p2[i].us = 0;
        p2[i].pwt = 0;
        p2[i].pcd = 0;
        p2[i].a = 0;
        p2[i].d = 0;
        p2[i].pat = 0;
        p2[i].g = 0;
        p2[i].avl = 0;
        p2[i].page_addr = i + 1024;         
    }

    /* Para los restantes 0.5 MB = 1024 - PAGE_TABLE1_COUNT = 1024 - 896 = 128 (descriptores de Tablas de Paginas) de la 2da tabla dejamos present en 0 */
    for (int i = PAGE_TABLE1_COUNT; i < 1024; i++) {
        p2[i].p = 0;
        p2[i].rw = 0;
        p2[i].us = 0;
        p2[i].pwt = 0;
        p2[i].pcd = 0;
        p2[i].a = 0;
        p2[i].d = 0;
        p2[i].pat = 0;
        p2[i].g = 0;
        p2[i].avl = 0;
        p2[i].page_addr = 0;         
    }
}

void mmu_inicializar_dir_tarea() {
    // pedimos una pagina para el directorio de paginas de la tarea
    void* nueva_pagina = dame_pagina_libre_tierra();

    // seteamos en 0 todas las entradas para poder distinguir si las entradas ya fueron utilizadas
    pd_entry* directorio_paginas = (pd_entry*) nueva_pagina;
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

    // mapeamos pagina 1 (nivel 3)
    mmu_mapear_pagina(0x40000000, (unsigned int) directorio_paginas, dame_pagina_libre_mar());
    // mapeamos pagina 2 (nivel 3)
    mmu_mapear_pagina(0x40001000, (unsigned int) directorio_paginas, dame_pagina_libre_mar());
    // mapeamos pagina ancla (nivel 3)
    mmu_mapear_pagina(0x40002000, (unsigned int) directorio_paginas, dame_pagina_libre_tierra());

}

void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica) {
    // accedemos a la entrada correspondiente en el directorio de paginas
    int pd_index = PDE_INDEX(virtual);
    pd_entry* directorio_paginas = (pd_entry*) cr3; 

    // verificamos si la entrada ya fue seteada previamente, y en caso de que no, la seteamos
    if (directorio_paginas[pd_index] == 0) {
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
        directorio_paginas[pd_index].table_addr = ((unsigned int) dame_pagina_libre_tierra()) >> 12;
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