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
#define TASK_SIZE               0x00002000
#define PAGS_TOTALES            1920

/* Indices GDT */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT               41  
#define GDT_IDX_NULL_DESC       0
#define GDT_IDX_IDLE            17
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
#define TASK_CODE_PAGE1         0x40000000 /* direccion virtual de la pagina 1 de codigo */
#define TASK_CODE_PAGE2         0x40001000 /* direccion virtual de la pagina 2 de codigo */
#define TASK_IDLE_CODE          0x40000000 /* direccion virtual del codigo de la tarea idle */
#define TASK_ANCLA              0x40002000

#define TASK_STACK_BOT          0x40001C00
#define FLAG_STACK_BOT          0x40001FFC
#define BANDERA_BUFFER          0x40001000

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
#define TASK_IDLE_CODE_PAGE1    0x00020000
#define TASK_IDLE_CODE_PAGE2    0x00021000

#define PAGE_DIRECTORY_ADDR     0x00027000
#define PAGE_TABLE0_ADDR        0x00028000
#define TASK_IDLE_STACK_BOT     0x0002A000 /* direccion fisica de la base de la pila de la tarea idle */
#define TASK_ANCLA_FIS          0x00000000

// define indice para la entrada en el directorio de paginas (pde)
#define PDE_INDEX(virtual) virtual >> 22

// define indice para la entrada en la tabla de paginas (pte)
#define PTE_INDEX(virtual) (virtual << 10) >> 22

/* Definicion de la pantalla */
#define VIDEO_FILS 25
#define VIDEO_COLS 80
#define VIDEO_SCREEN 0xB8000
#define BANDERA_FILS 5
#define BANDERA_COLS 10

#endif  /* !__DEFINES_H__ */
