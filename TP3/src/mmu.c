/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"



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
    p0[1].table_addr = 0x30;
    

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

    pt_entry* p2 = (pt_entry *) PAGE_TABLE1_ADDR;
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
    for (int i = PAGE_TABLE1_COUNT; i < 1024 - PAGE_TABLE1_COUNT; i++) {
        p2[i].p = 0;
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
}

