/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 25
#define VIDEO_COLS 80
#define VIDEO_SCREEN 0xB8000
#define BANDERA_FILS 5
#define BANDERA_COLS 10

#include "colors.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c; // caracter
    unsigned char a; // modo
} ca;

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);

void screen_pintar_pantalla();

void print_exception_message(unsigned int e);

void print_modo_estado();

void print_modo_mapa();

#endif  /* !__SCREEN_H__ */
