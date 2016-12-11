/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "sched.h"
#include "i386.h"
#include "defines.h"
#include "gdt.h"
#include "mmu.h"
// tss de la tarea incial
tss tarea_inicial;
// tss de la tarea idle
tss tarea_idle;
// tss's de las tareas 
tss tss_navios[CANT_TAREAS];
// tss's de las banderas 
tss tss_banderas[CANT_TAREAS];

// hace el calculo del eip de la bandera asociada a la tarea dada
unsigned int calcular_eip_bandera(int tarea) {
    unsigned int* offset_eip = (unsigned int*) (0x11FFC + (tarea - 1) * TASK_SIZE);
    unsigned int puntero_funcion_bandera = *offset_eip;
    unsigned int res = puntero_funcion_bandera + TASK_CODE_PAGE1;
    return res;
}

// resetea el eip de la bandera al inicial para que cada vez que esta se ejecute vuelva a flamear
void resetear_bandera_tss() {
    // solo reseteamos si venimos de ejecutar la bandera
    if (es_tarea() == 0) {
        unsigned short tarea = dame_tarea_no_idle();
        tss_banderas[tarea-1].ss0 = GDT_IDX_KERNEL_DATA << 3;     
        tss_banderas[tarea-1].eflags = 0x202;
        tss_banderas[tarea-1].eip = calcular_eip_bandera(tarea);
        tss_banderas[tarea-1].esp = 0x40001FFC;     
        tss_banderas[tarea-1].ebp = 0x40001FFC;
    }
}

// iniciliza todas las tss y las entradas de gdt de cada una de ellas. Ademas mapea la memoria de todas las tareas
void tss_inicializar() {

    // no tocamos nada de la tss de la tarea incial, ya que simplemente la usamos para pisarla con basura. Dejamos todas las entradas por defecto en 0

    // inicializamos entrada en la gdt de la tss de la tarea incial
    gdt[GDT_IDX_INIT_TASK].limit_0_15 = 0x67;
    gdt[GDT_IDX_INIT_TASK].base_0_15 = (((int) &tarea_inicial) & 0xFFFF);
    gdt[GDT_IDX_INIT_TASK].base_23_16 = (((int) &tarea_inicial) & 0xFF0000) >> 16;
    gdt[GDT_IDX_INIT_TASK].type = 0x9;
    gdt[GDT_IDX_INIT_TASK].p = 0x1;
    gdt[GDT_IDX_INIT_TASK].base_31_24 = (((int) &tarea_inicial) & 0xFF000000) >> 24;

    // inicializamos tss de la tarea idle
    tarea_idle.cr3 = PAGE_DIRECTORY_ADDR;
    tarea_idle.eip = TASK_IDLE_CODE;
    tarea_idle.eflags = 0x202;
    tarea_idle.esp = TASK_IDLE_STACK_BOT;
    tarea_idle.ebp = TASK_IDLE_STACK_BOT;
    tarea_idle.es = GDT_IDX_KERNEL_DATA << 3;
    tarea_idle.cs = GDT_IDX_KERNEL_CODE << 3;
    tarea_idle.ss = GDT_IDX_KERNEL_DATA << 3;
    tarea_idle.ds = GDT_IDX_KERNEL_DATA << 3;
    tarea_idle.fs = GDT_IDX_KERNEL_DATA << 3;
    tarea_idle.gs = GDT_IDX_KERNEL_DATA << 3;

    gdt[GDT_IDX_IDLE].limit_0_15 = 0x67;
    gdt[GDT_IDX_IDLE].base_0_15 = ((int) &tarea_idle) & 0xFFFF;
    gdt[GDT_IDX_IDLE].base_23_16 = (((int) &tarea_idle) & 0xFF0000) >> 16;
    gdt[GDT_IDX_IDLE].type = 0x9;
    gdt[GDT_IDX_IDLE].p = 0x1;
    gdt[GDT_IDX_IDLE].base_31_24 = (((int) &tarea_idle) & 0xFF000000) >> 24;   

    for (int i = 1; i <= CANT_TAREAS; i++) {
        // inicializamos memoria de la tarea i y obtenemos cr3 de la misma 
        unsigned int cr3_tarea_i = mmu_inicializar_dir_tarea(i, (unsigned int) dame_pagina_libre_mar());
        // llamamos a la funcion simplemente porque en mmu_inicializar_dir_tarea utilizamos 2 paginas y solo le pedimos 1. Seria para "avanzar" el puntero
        dame_pagina_libre_mar();
        // inicializamos tss de tarea i
        tss_navios[i-1].esp0 = (unsigned int) dame_pagina_libre_tierra() + TAMANO_PAGINA;
        tss_navios[i-1].ss0 = GDT_IDX_KERNEL_DATA << 3;     
        tss_navios[i-1].cr3 = cr3_tarea_i;
        tss_navios[i-1].eip = TASK_CODE_PAGE1;     
        tss_navios[i-1].eflags = 0x202;
        tss_navios[i-1].esp = TASK_STACK_BOT;     
        tss_navios[i-1].ebp = TASK_STACK_BOT;
        tss_navios[i-1].es = (GDT_IDX_USER_DATA << 3) | 3;
        tss_navios[i-1].cs = (GDT_IDX_USER_CODE << 3) | 3;                   
        tss_navios[i-1].fs = (GDT_IDX_USER_DATA << 3) | 3;
        tss_navios[i-1].ss = (GDT_IDX_USER_DATA << 3) | 3;
        tss_navios[i-1].ds = (GDT_IDX_USER_DATA << 3) | 3;
        tss_navios[i-1].gs = (GDT_IDX_USER_DATA << 3) | 3;
        tss_navios[i-1].dtrap = 0x0;
        tss_navios[i-1].iomap = 0xFFFF;

        // inicializamos entrada en la gdt de la tss de la tarea i
        gdt[i].limit_0_15 = 0x67;
        gdt[i].base_0_15 = ((int) &tss_navios[i-1]) & 0xFFFF;
        gdt[i].base_23_16 = (((int) &tss_navios[i-1]) & 0xFF0000) >> 16;
        gdt[i].type = 0x9;
        gdt[i].p = 0x1;
        gdt[i].base_31_24 = (((int) &tss_navios[i-1]) & 0xFF000000) >> 24;

        // inicializamos tss de bandera i
        tss_banderas[i-1].esp0 = (unsigned int) dame_pagina_libre_tierra() + TAMANO_PAGINA;
        tss_banderas[i-1].ss0 = GDT_IDX_KERNEL_DATA << 3;     
        tss_banderas[i-1].cr3 = cr3_tarea_i;
        tss_banderas[i-1].eip = calcular_eip_bandera(i);
        tss_banderas[i-1].eflags = 0x202;
        tss_banderas[i-1].esp = FLAG_STACK_BOT;     
        tss_banderas[i-1].ebp = FLAG_STACK_BOT;
        tss_banderas[i-1].es = (GDT_IDX_USER_DATA << 3) | 3;
        tss_banderas[i-1].cs = (GDT_IDX_USER_CODE << 3) | 3;                   
        tss_banderas[i-1].fs = (GDT_IDX_USER_DATA << 3) | 3;
        tss_banderas[i-1].ss = (GDT_IDX_USER_DATA << 3) | 3;
        tss_banderas[i-1].ds = (GDT_IDX_USER_DATA << 3) | 3;
        tss_banderas[i-1].gs = (GDT_IDX_USER_DATA << 3) | 3;
        tss_banderas[i-1].dtrap = 0x0;
        tss_banderas[i-1].iomap = 0xFFFF;

        // inicializamos entrada en la gdt de la tss de la bandera i
        gdt[i+8].limit_0_15 = 0x67;
        gdt[i+8].base_0_15 = ((int) &tss_banderas[i-1]) & 0xFFFF;
        gdt[i+8].base_23_16 = (((int) &tss_banderas[i-1]) & 0xFF0000) >> 16;
        gdt[i+8].type = 0x9;
        gdt[i+8].p = 0x1;
        gdt[i+8].base_31_24 = (((int) &tss_banderas[i-1]) & 0xFF000000) >> 24;
    }
}

