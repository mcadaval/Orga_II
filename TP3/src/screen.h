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
#include "defines.h"

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

void flamear_bandera(unsigned char n);

void imprimir_bandera(unsigned char n);

void inicializar_pantalla();

void guardar_estado_registros(unsigned int cr0, unsigned int cr2, unsigned int cr3, unsigned int cr4, unsigned int eax, unsigned int ebx, unsigned int ecx, unsigned int edx, unsigned int esi, unsigned int edi, unsigned int ebp, unsigned int ds, unsigned int es, unsigned int fs, unsigned int gs, unsigned int tarea, unsigned int errCode, unsigned int eip, unsigned int cs, unsigned int eflags, unsigned int esp, unsigned int ss);

void imprimir_registros_y_excepcion();

void registar_memoria_tarea(unsigned int tarea, unsigned char numero_pag, unsigned int fisica);

void imprimir_excepciones_por_tarea();

void imprimir_relojes();

char siguiente_reloj(char reloj);

void matar_en_screen();



#endif  /* !__SCREEN_H__ */
