/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"
#include "gdt.h"

tss tarea_inicial;
tss tarea_idle;

tss tss_navios[CANT_TAREAS];
tss tss_banderas[CANT_TAREAS];

void tss_inicializar() {
    tarea_inicial = (tss) {
        (unsigned short)  0x0000,                   /*   ptl   */
        (unsigned short)  0x0000,                   /* unused0 */
        (unsigned int)    0x0000,                   /* esp0 */
        (unsigned short)  0x00,                     /* ss0 */
        (unsigned short)  0x00,                     /* unused1 */
        (unsigned int)    0x0000,                   /* esp1 */
        (unsigned short)  0x00,                     /* ss1 */
        (unsigned short)  0x00,                     /* unused2 */
        (unsigned int)    0x0000,                   /* esp2 */
        (unsigned short)  0x00,                     /* ss2 */
        (unsigned short)  0x00,                     /* unused3 */
        (unsigned int)    0x0000,                   /* cr3 */
        (unsigned int)    0x0000,                   /* eip */
        (unsigned int)    0x0000,                   /* eflags */
        (unsigned int)    0x0000,                   /* eax */
        (unsigned int)    0x0000,                   /* ecx */
        (unsigned int)    0x0000,                   /* edx */
        (unsigned int)    0x0000,                   /* ebx */
        (unsigned int)    0x0000,                   /* esp */
        (unsigned int)    0x0000,                   /* ebp */
        (unsigned int)    0x0000,                   /* esi */
        (unsigned int)    0x0000,                   /* edi */
        (unsigned short)  0x00,                     /* es */
        (unsigned short)  0x00,                     /* unused4 */
        (unsigned short)  0x00,                     /* cs */
        (unsigned short)  0x00,                     /* unused5 */
        (unsigned short)  0x00,                     /* ss */
        (unsigned short)  0x00,                     /* unused6 */
        (unsigned short)  0x00,                     /* ds */
        (unsigned short)  0x00,                     /* unused7 */
        (unsigned short)  0x00,                     /* fs */
        (unsigned short)  0x00,                     /* unused8 */
        (unsigned short)  0x00,                     /* gs */
        (unsigned short)  0x00,                     /* unused9 */
        (unsigned short)  0x00,                     /* ldt */
        (unsigned short)  0x00,                     /* unused10 */
        (unsigned short)  0x00,                     /* dtrap */
        (unsigned short)  0x00                      /* iomap */
    };

    tarea_idle = (tss) {
        (unsigned short)  0x00,                   /*   ptl   */
        (unsigned short)  0x00,                   /* unused0 */
        (unsigned int)    0x2A000,                   /* esp0 */
        (unsigned short)  0x00,                     /* ss0 */
        (unsigned short)  0x00,                     /* unused1 */
        (unsigned int)    0x0000,                   /* esp1 */
        (unsigned short)  0x00,                     /* ss1 */
        (unsigned short)  0x00,                     /* unused2 */
        (unsigned int)    0x0000,                   /* esp2 */
        (unsigned short)  0x00,                     /* ss2 */
        (unsigned short)  0x00,                     /* unused3 */
        (unsigned int)    mmu_inicializar_dir_tarea(), /* cr3 */
        (unsigned int)    0x40000000,                   /* eip */
        (unsigned int)    0x202,                   /* eflags */
        (unsigned int)    0x0000,                   /* eax */
        (unsigned int)    0x0000,                   /* ecx */
        (unsigned int)    0x0000,                   /* edx */
        (unsigned int)    0x0000,                   /* ebx */
        (unsigned int)    0x40001C00,                   /* esp */
        (unsigned int)    0x40001C00,                   /* ebp */
        (unsigned int)    0x0000,                   /* esi */
        (unsigned int)    0x0000,                   /* edi */
        (unsigned short)  0xAB,                     /* es */ // selector de datos nivel 3 10101  0  11  AB
        (unsigned short)  0x00,                     /* unused4 */
        (unsigned short)  0x9B,                     /* cs */ // selector de codigo nivel 3 10011  0  11  9B
        (unsigned short)  0x00,                     /* unused5 */
        (unsigned short)  0x00,                     /* ss */
        (unsigned short)  0x00,                     /* unused6 */
        (unsigned short)  0x00,                     /* ds */
        (unsigned short)  0x00,                     /* unused7 */
        (unsigned short)  0x00,                     /* fs */
        (unsigned short)  0x00,                     /* unused8 */
        (unsigned short)  0x00,                     /* gs */
        (unsigned short)  0x00,                     /* unused9 */
        (unsigned short)  0x00,                     /* ldt */
        (unsigned short)  0x00,                     /* unused10 */
        (unsigned short)  0x00,                     /* dtrap */
        (unsigned short)  0x00                      /* iomap */
    };

    for (int i = 0; i < CANT_TAREAS; i++) {
        tss_navios[i] = (tss) {
            (unsigned short)  0x00,                   /*   ptl   */
            (unsigned short)  0x00,                   /* unused0 */
            (unsigned int)    0x2A000,                  /* esp0 */
            (unsigned short)  0x00,                     /* ss0 */
            (unsigned short)  0x00,                     /* unused1 */
            (unsigned int)    0x0000,                   /* esp1 */
            (unsigned short)  0x00,                     /* ss1 */
            (unsigned short)  0x00,                     /* unused2 */
            (unsigned int)    0x0000,                   /* esp2 */
            (unsigned short)  0x00,                     /* ss2 */
            (unsigned short)  0x00,                     /* unused3 */
            (unsigned int)    mmu_inicializar_dir_tarea(), /* cr3 */
            (unsigned int)    0x40000000,                   /* eip */
            (unsigned int)    0x202,                   /* eflags */
            (unsigned int)    0x0000,                   /* eax */
            (unsigned int)    0x0000,                   /* ecx */
            (unsigned int)    0x0000,                   /* edx */
            (unsigned int)    0x0000,                   /* ebx */
            (unsigned int)    0x40001C00,                   /* esp */
            (unsigned int)    0x40001C00,                   /* ebp */
            (unsigned int)    0x0000,                   /* esi */
            (unsigned int)    0x0000,                   /* edi */
            (unsigned short)  0xAB,                     /* es */ // selector de datos nivel 3 10101  0  11  AB
            (unsigned short)  0x00,                     /* unused4 */
            (unsigned short)  0x9B,                     /* cs */ // selector de codigo nivel 3 10011  0  11  9B
            (unsigned short)  0x00,                     /* unused5 */
            (unsigned short)  0x00,                     /* ss */
            (unsigned short)  0x00,                     /* unused6 */
            (unsigned short)  0x00,                     /* ds */
            (unsigned short)  0x00,                     /* unused7 */
            (unsigned short)  0x00,                     /* fs */
            (unsigned short)  0x00,                     /* unused8 */
            (unsigned short)  0x00,                     /* gs */
            (unsigned short)  0x00,                     /* unused9 */
            (unsigned short)  0x00,                     /* ldt */
            (unsigned short)  0x00,                     /* unused10 */
            (unsigned short)  0x00,                     /* dtrap */
            (unsigned short)  0x00                      /* iomap */
    }
}

