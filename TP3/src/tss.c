/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tarea_inicial;
tss tarea_idle;

tss tss_navios[CANT_TAREAS];
tss tss_banderas[CANT_TAREAS];

void tss_inicializar() {
    // inicializamos tss de tarea inicial
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
        (unsigned short)  0xFFFF                    /* iomap */
    };

    // inicializamos entrada en la gdt de la tss de la tarea incial
    // gdt[GDT_IDX_INIT_TASK] = (gdt_entry) {
    //     (unsigned short)    0x0067,                                                /* limit[0:15]  */
    //     (unsigned short)    (((int) &tarea_inicial) & 0xFFFF),                     /* base[0:15]   */
    //     (unsigned char)     (((int) &tarea_inicial) & 0xFF0000) >> 16,             /* base[23:16]  */
    //     (unsigned char)     0x09,                                                  /* type         */ 
    //     (unsigned char)     0x00,                                                  /* s            */
    //     (unsigned char)     0x00,                                                  /* dpl          */
    //     (unsigned char)     0x01,                                                  /* p            */
    //     (unsigned char)     0x00,                                                  /* limit[16:19] */
    //     (unsigned char)     0x00,                                                  /* avl          */
    //     (unsigned char)     0x00,                                                  /* l            */
    //     (unsigned char)     0x00,                                                  /* db           */
    //     (unsigned char)     0x00,                                                  /* g            */
    //     (unsigned char)     (((int) &tarea_inicial) & 0xFF000000) >> 24,                   /* base[31:24]  */
    // };

    // inicializamos entrada en la gdt de la tss de la tarea incial
    gdt[GDT_IDX_INIT_TASK].limit_0_15 = 0x67;
    gdt[GDT_IDX_INIT_TASK].base_0_15 = (((int) &tarea_inicial) & 0xFFFF);
    gdt[GDT_IDX_INIT_TASK].base_23_16 = (((int) &tarea_inicial) & 0xFF0000) >> 16;
    gdt[GDT_IDX_INIT_TASK].type = 0x9;
    gdt[GDT_IDX_INIT_TASK].p = 0x1;
    gdt[GDT_IDX_INIT_TASK].base_31_24 = (((int) &tarea_inicial) & 0xFF000000) >> 24;









    // inicializamos tss de idle

    // tarea_idle = (tss) {
    //     (unsigned short)  0x00,                     /*   ptl   */
    //     (unsigned short)  0x00,                     /* unused0 */
    //     (unsigned int)    0x00,                     /* esp0 */
    //     (unsigned short)  0x00,                     /* ss0 */
    //     (unsigned short)  0x00,                     /* unused1 */
    //     (unsigned int)    0x0000,                   /* esp1 */
    //     (unsigned short)  0x00,                     /* ss1 */
    //     (unsigned short)  0x00,                     /* unused2 */
    //     (unsigned int)    0x0000,                   /* esp2 */
    //     (unsigned short)  0x00,                     /* ss2 */
    //     (unsigned short)  0x00,                     /* unused3 */
    //     (unsigned int)    0x27000,                  /* cr3 */
    //     (unsigned int)    0x40000000,               /* eip */
    //     (unsigned int)    0x202,                    /* eflags */
    //     (unsigned int)    0x0000,                   /* eax */
    //     (unsigned int)    0x0000,                   /* ecx */
    //     (unsigned int)    0x0000,                   /* edx */
    //     (unsigned int)    0x0000,                   /* ebx */
    //     (unsigned int)    0x2A000,                  /* esp */
    //     (unsigned int)    0x2A000,                  /* ebp */
    //     (unsigned int)    0x0000,                   /* esi */
    //     (unsigned int)    0x0000,                   /* edi */
    //     (unsigned short)  GDT_IDX_KERNEL_DATA << 3, /* es */ 
    //     (unsigned short)  0x00,                     /* unused4 */
    //     (unsigned short)  GDT_IDX_KERNEL_CODE << 3,   /* cs */ 
    //     (unsigned short)  0x00,                     /* unused5 */
    //     (unsigned short)  GDT_IDX_KERNEL_DATA << 3,  /* ss */ 
    //     (unsigned short)  0x00,                     /* unused6 */
    //     (unsigned short)  GDT_IDX_KERNEL_DATA << 3,  /* ds */
    //     (unsigned short)  0x00,                     /* unused7 */
    //     (unsigned short)  GDT_IDX_KERNEL_DATA << 3,    /* fs */
    //     (unsigned short)  0x00,                     /* unused8 */
    //     (unsigned short)  GDT_IDX_KERNEL_DATA << 3,   /* gs */
    //     (unsigned short)  0x00,                     /* unused9 */
    //     (unsigned short)  0x00,                     /* ldt */
    //     (unsigned short)  0x00,                     /* unused10 */
    //     (unsigned short)  0x00,                     /* dtrap */
    //     (unsigned short)  0x00                      /* iomap */
    // };

    tarea_idle.cr3 = 0x27000;
    tarea_idle.eip = 0x40000000;
    tarea_idle.eflags = 0x202;
    tarea_idle.esp = 0x2A000;
    tarea_idle.ebp = 0x2A000;
    tarea_idle.es = GDT_IDX_KERNEL_DATA << 3;
    tarea_idle.cs = GDT_IDX_KERNEL_CODE << 3;
    tarea_idle.ss = GDT_IDX_KERNEL_DATA << 3;
    tarea_idle.ds = GDT_IDX_KERNEL_DATA << 3;
    tarea_idle.fs = GDT_IDX_KERNEL_DATA << 3;
    tarea_idle.gs = GDT_IDX_KERNEL_DATA << 3;


    // inicializamos entrada en la gdt de la tss de idle
    // gdt[GDT_IDX_IDLE] = (gdt_entry) {
    //     (unsigned short)    0x0067,                                                /* limit[0:15]  */
    //     (unsigned short)    ((int) &tarea_idle) & 0xFFFF,                          /* base[0:15]   */
    //     (unsigned char)     (((int) &tarea_idle) & 0xFF0000) >> 16,                /* base[23:16]  */
    //     (unsigned char)     0x09,                                                  /* type         */ 
    //     (unsigned char)     0x00,                                                  /* s            */
    //     (unsigned char)     0x00,                                                  /* dpl          */
    //     (unsigned char)     0x01,                                                  /* p            */
    //     (unsigned char)     0x00,                                                  /* limit[16:19] */
    //     (unsigned char)     0x00,                                                  /* avl          */
    //     (unsigned char)     0x00,                                                  /* l            */
    //     (unsigned char)     0x00,                                                  /* db           */
    //     (unsigned char)     0x00,                                                  /* g            */
    //     (unsigned char)     (((int) &tarea_idle) & 0xFF000000) >> 24               /* base[31:24]  */
    // };

    gdt[GDT_IDX_IDLE].limit_0_15 = 0x67;
    gdt[GDT_IDX_IDLE].base_0_15 = ((int) &tarea_idle) & 0xFFFF;
    gdt[GDT_IDX_IDLE].base_23_16 = (((int) &tarea_idle) & 0xFF0000) >> 16;
    gdt[GDT_IDX_IDLE].type = 0x9;
    gdt[GDT_IDX_IDLE].p = 0x1;
    gdt[GDT_IDX_IDLE].base_31_24 = (((int) &tarea_idle) & 0xFF000000) >> 24;   



    for (int i = 1; i <= CANT_TAREAS; i++) {
        // inicializamos tarea i y obtenemos cr3 de la misma 
        unsigned int cr3_tarea_i = mmu_inicializar_dir_tarea(i, (unsigned int) dame_pagina_libre_mar());

        // inicializamos tss de tarea i
        tss_navios[i-1].esp0 = (unsigned int) dame_pagina_libre_tierra() + 0x1000;
        tss_navios[i-1].ss0 = GDT_IDX_KERNEL_DATA << 3;     
        tss_navios[i-1].cr3 = cr3_tarea_i;
        tss_navios[i-1].eip = 0x40000000;     
        tss_navios[i-1].eflags = 0x202;
        tss_navios[i-1].esp = 0x40001C00;     
        tss_navios[i-1].ebp = 0x40001C00;
        tss_navios[i-1].es = (GDT_IDX_USER_DATA << 3) | 3;
        tss_navios[i-1].cs = (GDT_IDX_USER_CODE << 3) | 3;                   
        tss_navios[i-1].fs = (GDT_IDX_USER_DATA << 3) | 3;
        tss_navios[i-1].ss = (GDT_IDX_USER_DATA << 3) | 3;
        tss_navios[i-1].ds = (GDT_IDX_USER_DATA << 3) | 3;
        // tss_navios[i-1].dtrap = 0x0;
        tss_navios[i-1].iomap = 0xFFFF;

        // inicializamos entrada en la gdt de la tss de tarea i
        // gdt[i] = (gdt_entry) {
        //     (unsigned short)    0x0067,                                                /* limit[0:15]  */
        //     (unsigned short)    ((int) &tss_navios[i-1]) & 0xFFFF,       /* base[0:15]   */
        //     (unsigned char)     (((int) &tss_navios[i-1]) & 0xFF0000) >> 16, /* base[23:16]  */
        //     (unsigned char)     0x09,                                                  /* type         */ 
        //     (unsigned char)     0x00,                                                  /* s            */
        //     (unsigned char)     0x00,                                                  /* dpl          */
        //     (unsigned char)     0x01,                                                  /* p            */
        //     (unsigned char)     0x00,                                                  /* limit[16:19] */
        //     (unsigned char)     0x00,                                                  /* avl          */
        //     (unsigned char)     0x00,                                                  /* l            */
        //     (unsigned char)     0x00,                                                  /* db           */
        //     (unsigned char)     0x00,                                                  /* g            */
        //     (unsigned char)     (((int) &tss_navios[i-1]) & 0xFF000000) >> 24,/* base[31:24]  */
        // };

        gdt[i].limit_0_15 = 0x67;
        gdt[i].base_0_15 = ((int) &tss_navios[i-1]) & 0xFFFF;
        gdt[i].base_23_16 = (((int) &tss_navios[i-1]) & 0xFF0000) >> 16;
        gdt[i].type = 0x9;
        gdt[i].p = 0x1;
        gdt[i].base_31_24 = (((int) &tss_navios[i-1]) & 0xFF000000) >> 24;

        // inicializamos tss de bandera i
        // tss_banderas[i-1] = (tss) {
        //     (unsigned short)  0x00,                   /*   ptl   */
        //     (unsigned short)  0x00,                   /* unused0 */
        //     (unsigned int)    dame_pagina_libre_tierra() + 0x1000,                  /* esp0 */ // @todo
        //     (unsigned short)  GDT_IDX_KERNEL_DATA << 3,                     /* ss0 */ // selector de datos nivel 0 10100  0  11  A3
        //     (unsigned short)  0x00,                     /* unused1 */
        //     (unsigned int)    0x0000,                   /* esp1 */
        //     (unsigned short)  0x00,                     /* ss1 */
        //     (unsigned short)  0x00,                     /* unused2 */
        //     (unsigned int)    0x0000,                   /* esp2 */
        //     (unsigned short)  0x00,                     /* ss2 */
        //     (unsigned short)  0x00,                     /* unused3 */
        //     (unsigned int)    cr3_tarea_i,              /* cr3 */
        //     (unsigned int)    0x40002FFB,              /* eip */ // ultimos 4 bytes de la tarea
        //     (unsigned int)    0x202,                   /* eflags */
        //     (unsigned int)    0x0000,                   /* eax */
        //     (unsigned int)    0x0000,                   /* ecx */
        //     (unsigned int)    0x0000,                   /* edx */
        //     (unsigned int)    0x0000,                   /* ebx */
        //     (unsigned int)    0x40001FFC,                   /* esp */
        //     (unsigned int)    0x40001FFC,                   /* ebp */
        //     (unsigned int)    0x0000,                   /* esi */
        //     (unsigned int)    0x0000,                   /* edi */
        //     (unsigned short)  (GDT_IDX_USER_DATA << 3) | 3,                     /* es */ // selector de datos nivel 3 10101  0  11  AB
        //     (unsigned short)  0x00,                     /* unused4 */
        //     (unsigned short)  (GDT_IDX_USER_CODE << 3) | 3,                      /* cs */ // selector de codigo nivel 3 10011  0  11  9B
        //     (unsigned short)  0x00,                     /* unused5 */
        //     (unsigned short)  (GDT_IDX_USER_DATA << 3) | 3,                     /* ss */
        //     (unsigned short)  0x00,                     /* unused6 */
        //     (unsigned short)  (GDT_IDX_USER_DATA << 3) | 3,                     /* ds */
        //     (unsigned short)  0x00,                     /* unused7 */
        //     (unsigned short)  (GDT_IDX_USER_DATA << 3) | 3,                     /* fs */
        //     (unsigned short)  0x00,                     /* unused8 */
        //     (unsigned short)  (GDT_IDX_USER_DATA << 3) | 3,                     /* gs */
        //     (unsigned short)  0x00,                     /* unused9 */
        //     (unsigned short)  0x00,                     /* ldt */
        //     (unsigned short)  0x00,                     /* unused10 */
        //     (unsigned short)  0x00,                     /* dtrap */
        //     (unsigned short)  0x00                      /* iomap */
        // };

        // inicializamos tss de tarea i
        tss_banderas[i-1].esp0 = (unsigned int) dame_pagina_libre_tierra() + 0x1000;
        tss_banderas[i-1].ss0 = GDT_IDX_KERNEL_DATA << 3;     
        tss_banderas[i-1].cr3 = cr3_tarea_i;
        tss_banderas[i-1].eip = 0x40000000;     
        tss_banderas[i-1].eflags = 0x202;
        tss_banderas[i-1].esp = 0x40001FFC;     
        tss_banderas[i-1].ebp = 0x40001FFC;
        tss_banderas[i-1].es = (GDT_IDX_USER_DATA << 3) | 3;
        tss_banderas[i-1].cs = (GDT_IDX_USER_CODE << 3) | 3;                   
        tss_banderas[i-1].fs = (GDT_IDX_USER_DATA << 3) | 3;
        tss_banderas[i-1].ss = (GDT_IDX_USER_DATA << 3) | 3;
        tss_banderas[i-1].ds = (GDT_IDX_USER_DATA << 3) | 3;
        // tss_banderas[i-1].dtrap = 0x0;
        tss_banderas[i-1].iomap = 0xFFFF;

        // inicializamos entrada en la gdt de la tss de bandera i
        // gdt[i+8] = (gdt_entry) {
        //     (unsigned short)    0x0067,                                                /* limit[0:15]  */
        //     (unsigned short)    ((int) &tss_banderas[i-1]) & 0xFFFF,              /* base[0:15]   */
        //     (unsigned char)     (((int) &tss_banderas[i-1]) & 0xFF0000) >> 16,    /* base[23:16]  */
        //     (unsigned char)     0x09,                                                  /* type         */ 
        //     (unsigned char)     0x00,                                                  /* s            */
        //     (unsigned char)     0x00,                                                  /* dpl          */
        //     (unsigned char)     0x01,                                                  /* p            */
        //     (unsigned char)     0x00,                                                  /* limit[16:19] */
        //     (unsigned char)     0x00,                                                  /* avl          */
        //     (unsigned char)     0x00,                                                  /* l            */
        //     (unsigned char)     0x00,                                                  /* db           */
        //     (unsigned char)     0x00,                                                  /* g            */
        //     (unsigned char)     (((int) &tss_banderas[i-1]) & 0xFF000000) >> 24,  /* base[31:24]  */
        // };

        gdt[i+8].limit_0_15 = 0x67;
        gdt[i+8].base_0_15 = ((int) &tss_banderas[i-1]) & 0xFFFF;
        gdt[i+8].base_23_16 = (((int) &tss_banderas[i-1]) & 0xFF0000) >> 16;
        gdt[i+8].type = 0x9;
        gdt[i+8].p = 0x1;
        gdt[i+8].base_31_24 = (((int) &tss_banderas[i-1]) & 0xFF000000) >> 24;
    }
}

