
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../tp2.h"
void rotar_asm    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

void rotar_c    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

typedef void (rotar_fn_t) (unsigned char*, unsigned char*, int, int, int, int);



void leer_params_rotar(configuracion_t *config, int argc, char *argv[]) {
}

void aplicar_rotar(configuracion_t *config)
{
	rotar_fn_t *rotar = SWITCH_C_ASM ( config, rotar_c, rotar_asm ) ;
	buffer_info_t info = config->src;
	rotar(info.bytes, config->dst.bytes, info.width, info.height, info.width_with_padding,
	         config->dst.width_with_padding);

}

void ayuda_rotar()
{
	printf ( "       * rotar\n" );
	printf ( "           Parámetros     : ninguno\n");
	printf ( "           Ejemplo de uso : \n"
	         "                         rotar -i c facil.bmp\n");
}


