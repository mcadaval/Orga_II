/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "i386.h"

#define PAGE_DIRECTORY_ADDR 0x27000
#define PAGE_TABLE0_COUNT 1024
#define PAGE_TABLE1_COUNT 896
#define PAGE_TABLE0_ADDR 0x28000
#define VIRTUAL_ADDR 0x40000000

// define indice para la entrada en el directorio de paginas (pde)
#define PDE_INDEX(virtual) virtual >> 22

// define indice para la entrada en la tabla de paginas (pte)
#define PTE_INDEX(virtual) (virtual << 10) >> 22


typedef struct page_directory_entry {
    unsigned char p:1;
    unsigned char rw:1;
    unsigned char us:1;
    unsigned char pwt:1;
    unsigned char pcd:1;
    unsigned char a:1;
    unsigned char i:1;
    unsigned char ps:1;
    unsigned char g:1;
    unsigned char avl:3;
    unsigned int table_addr:20;
} __attribute__((__packed__)) pd_entry;

typedef struct page_table_entry {
    unsigned char p:1;
    unsigned char rw:1;
    unsigned char us:1;
    unsigned char pwt:1;
    unsigned char pcd:1;
    unsigned char a:1;
    unsigned char d:1;
    unsigned char pat:1;
    unsigned char g:1;
    unsigned char avl:3;
    unsigned int page_addr:20;
} __attribute__((__packed__)) pt_entry;

/* PAGE DIRECTORY */
extern pd_entry page_directory[];
void* dame_pagina_libre_tierra();
void* dame_pagina_libre_mar();
void copiar_pagina(unsigned int origen, unsigned int destino);
void mmu_inicializar_dir_idle();
void mmu_inicializar_dir_kernel();
void mmu_inicializar_dir_kernel_tarea(unsigned int cr3);
unsigned int mmu_inicializar_dir_tarea(unsigned int tarea, unsigned int fisica);
void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, unsigned char us, unsigned char rw);
void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3);


#endif	/* !__MMU_H__ */
