
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../tp2.h"

void colorizar_asm    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size, float alpha);

void colorizar_c    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size, float alpha);

typedef void (colorizar_fn_t) (unsigned char*, unsigned char*, int, int, int, int, float);

typedef struct colorizar_params_t {
	float alpha;
} colorizar_params_t;


colorizar_params_t extra;
void leer_params_colorizar(configuracion_t *config, int argc, char *argv[]) {
	config->extra_config = &extra;
	extra.alpha = atof(argv[argc - 1]);
}

void aplicar_colorizar(configuracion_t *config)
{
	colorizar_fn_t *colorizar = SWITCH_C_ASM ( config, colorizar_c, colorizar_asm ) ;
	colorizar_params_t *extra = (colorizar_params_t*)config->extra_config;

	

	buffer_info_t info = config->src;
	colorizar(info.bytes, config->dst.bytes, info.width, info.height, info.width_with_padding,
	         config->dst.width_with_padding, extra->alpha);

}

void ayuda_colorizar()
{
	printf ( "       * colorizar\n" );
	printf ( "           Parámetros     : \n"
	         "                         alpha valor entre 0 y 1\n");
	printf ( "           Ejemplo de uso : \n"
	         "                         colorizar -i c facil.bmp 0.4\n" );
}


