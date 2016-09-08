
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../tp2.h"
void pixelar_asm    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

void pixelar_c    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

typedef void (pixelar_fn_t) (unsigned char*, unsigned char*, int, int, int, int);



void leer_params_pixelar(configuracion_t *config, int argc, char *argv[]) {
}

void aplicar_pixelar(configuracion_t *config)
{
	pixelar_fn_t *pixelar = SWITCH_C_ASM ( config, pixelar_c, pixelar_asm ) ;
	buffer_info_t info = config->src;
	pixelar(info.bytes, config->dst.bytes, info.width, info.height, info.width_with_padding,
	         config->dst.width_with_padding);

}

void ayuda_pixelar()
{
	printf ( "       * pixelar\n" );
	printf ( "           Parámetros     : ninguno\n");
	printf ( "           Ejemplo de uso : \n"
	         "                         pixelar -i c facil.bmp\n");
}


