/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "i386.h"

/*
 * Estructura de la tarea :
 *
 *  0x40000000 - 0x40001000 : codigo tarea          (   5 KB)
 *  0x40001000 - 0x40001400 : area de banderas      (   1 KB)
 *  0x40001400 - 0x40001c00 : pila tarea            (   2 KB)
 *  0x40001c00 - 0x40001ffc : pila bandera          (1020 B )
 *  0x40001ffc - 0x40002000 : dir. funcion bandera  (   4 B )
 */

/* Estructura de para acceder a memoria de video */
typedef struct {
    unsigned char c;
    unsigned char a;
} ca;

unsigned char *bandera();

// global var
#define GLOBAL_START 0x40000000
#define var_B GLOBAL_START+0x2000-0x200

void task()
{    
    /* Tarea 7 */
    // MUERE, division por cero
    int a, b, c;

    while (1) {
        a = 0;
        b = 0;
        c =+ a / b;
    };
    *((int*)c) = 1;
}

unsigned char *bandera()
{
    ca(*buffer)[10] = (ca(*)[10]) (BANDERA_BUFFER);
    int *b = (int *) (var_B);
    unsigned int fil;
    unsigned int col;

    (*b)++;

    if (*b == 6)
        *b = 0;

    for (fil = 0; fil < 5; fil++) {
        for (col = 0; col < 10; col++) {
            buffer[fil][col].c = 206;

            if (fil > *b)
              buffer[fil][col].a = C_BG_BLACK | C_FG_DARK_GREY;
            else
              buffer[fil][col].a = C_BG_GREEN | C_FG_RED;
        }
    }

    syscall_bandera_fin((unsigned int) buffer);

    /* Para que el compilador no tire warning... */
    return 0;
}
