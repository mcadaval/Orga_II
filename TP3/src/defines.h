/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_TAREAS             8

#define TAMANO_PAGINA           0x00001000

#define TASK_SIZE               2 * 4096


/* Indices GDT */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT               41  
#define GDT_IDX_NULL_DESC       0
#define GDT_IDX_IDLE            1
#define GDT_IDX_TASK1_CODE      2
#define GDT_IDX_TASK1_FLAG      3
#define GDT_IDX_TASK2_CODE      4
#define GDT_IDX_TASK2_FLAG      5
#define GDT_IDX_TASK3_CODE      6
#define GDT_IDX_TASK3_FLAG      7
#define GDT_IDX_TASK4_CODE      8
#define GDT_IDX_TASK4_FLAG      9
#define GDT_IDX_TASK5_CODE      10
#define GDT_IDX_TASK5_FLAG      11
#define GDT_IDX_TASK6_CODE      12
#define GDT_IDX_TASK6_FLAG      13
#define GDT_IDX_TASK7_CODE      14
#define GDT_IDX_TASK7_FLAG      15
#define GDT_IDX_TASK8_CODE      16
#define GDT_IDX_TASK8_FLAG      17
#define GDT_IDX_KERNEL_CODE     18
#define GDT_IDX_USER_CODE       19    
#define GDT_IDX_KERNEL_DATA     20    
#define GDT_IDX_USER_DATA       21 
#define GDT_IDX_KERNEL_SCREEN   22       
#define GDT_IDX_INIT_TASK       23

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE               0x40000000 /* direccion virtual del codigo */

#define TASK_IDLE_CODE          0x40000000 /* direccion virtual del codigo de la tarea idle */
#define TASK_IDLE_STACK         0x003D0000 /* direccion virtual de la pila de la tarea idle */
#define TASK_IDLE_STACK_RING_0  0x0002B000 /* direccion fisica de la pila de la tarea idle */

#define TASK_ANCLA              0x40002000
#define TASK_ANCLA_FIS          0x00000000

#define AREA_TIERRA_INICIO      0x00000000  /* 0.0 MB     */
#define AREA_TIERRA_FIN         0x000FFFFF  /* 1.0 MB - 1 */
#define AREA_MAR_INICIO         0x00100000  /* 1.0 MB     */
#define AREA_MAR_FIN            0x0077FFFF  /* 7.5 MB - 1 */

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */
#define TASK_1_CODE_SRC_ADDR    0x00010000
#define TASK_2_CODE_SRC_ADDR    0x00012000
#define TAKS_3_CODE_SRC_ADDR    0x00014000
#define TASK_4_CODE_SRC_ADDR    0x00016000
#define TASK_5_CODE_SRC_ADDR    0x00018000
#define TASK_6_CODE_SRC_ADDR    0x0001A000
#define TASK_7_CODE_SRC_ADDR    0x0001C000
#define TASK_8_CODE_SRC_ADDR    0x0001E000

#define TASK_IDLE_CODE_SRC_ADDR 0x00020000

/* Indices TSS */
/* -------------------------------------------------------------------------- */
#define TSS_IDX_TASK1_CODE      0 
#define TSS_IDX_TASK1_FLAG      0
#define TSS_IDX_TASK2_CODE      1 
#define TSS_IDX_TASK2_FLAG      1
#define TSS_IDX_TASK3_CODE      2 
#define TSS_IDX_TASK3_FLAG      2
#define TSS_IDX_TASK4_CODE      3 
#define TSS_IDX_TASK4_FLAG      3
#define TSS_IDX_TASK5_CODE      4 
#define TSS_IDX_TASK5_FLAG      4
#define TSS_IDX_TASK6_CODE      5 
#define TSS_IDX_TASK6_FLAG      5
#define TSS_IDX_TASK7_CODE      6 
#define TSS_IDX_TASK7_FLAG      6
#define TSS_IDX_TASK8_CODE      7 
#define TSS_IDX_TASK8_FLAG      7

#endif  /* !__DEFINES_H__ */
