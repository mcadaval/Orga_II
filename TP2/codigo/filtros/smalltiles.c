
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../tp2.h"
void smalltiles_asm    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

void smalltiles_c    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

typedef void (smalltiles_fn_t) (unsigned char*, unsigned char*, int, int, int, int);



void leer_params_smalltiles(configuracion_t *config, int argc, char *argv[]) {
}

void aplicar_smalltiles(configuracion_t *config)
{
	smalltiles_fn_t *smalltiles = SWITCH_C_ASM ( config, smalltiles_c, smalltiles_asm ) ;
	buffer_info_t info = config->src;
	smalltiles(info.bytes, config->dst.bytes, info.width, info.height, info.width_with_padding,
	         config->dst.width_with_padding);

}

void ayuda_smalltiles()
{
	printf ( "       * smalltiles\n" );
	printf ( "           Parámetros     : ninguno\n");
	printf ( "           Ejemplo de uso : \n"
	         "                         smalltiles -i c facil.bmp\n");
}


