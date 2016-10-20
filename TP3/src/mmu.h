/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#define PAGE_DIRECTORY_COUNT 2
#define PAGE_DIRECTORY_ADDR 0x27000
#define PAGE_TABLE0_COUNT 1024
#define PAGE_TABLE1_COUNT 896
#define PAGE_TABLE0_ADDR 0x28000
#define PAGE_TABLE1_ADDR 0x30000

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
void mmu_inicializar();

#endif	/* !__MMU_H__ */
