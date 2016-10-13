/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "colors.h"

#ifndef __SCREEN_H__
#define __SCREEN_H__


/* Definicion de la pantalla */
#define VIDEO_FILS 25
#define VIDEO_COLS 80
#define VIDEO_ADDR 0xB8000

typedef struct screen_char_t {
  unsigned char character; // caracter ascii
  unsigned char mode;      // modo
}screen_char;

void screen_pintar_pantalla();


#endif  /* !__SCREEN_H__ */
