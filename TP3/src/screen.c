/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
// #include "i386.h"  //Para magic breakpoint

static screen_char(*screen)[VIDEO_COLS] = (screen_char (*)[VIDEO_COLS]) VIDEO_ADDR;


void escribir_caracter(screen_char* screen)
{
  screen_char r = {.character = ' ', .mode = C_BG_BLACK | C_FG_WHITE};
  *screen = r;
}

void screen_pintar_pantalla()
{
  for (int i = 0; i < VIDEO_FILS; i++) {
    for (int j = 0; j < VIDEO_COLS; j++) {
      escribir_caracter(&screen[i][j]);
    }
  } 
}




