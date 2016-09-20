
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../tp2.h"

void combinar_asm    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size, float alpha);

void combinar_c    (unsigned char *src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size, float alpha);

typedef void (combinar_fn_t) (unsigned char*, unsigned char*, int, int, int, int, float);

typedef struct combinar_params_t {
	float alpha;
} combinar_params_t;


combinar_params_t extra;
void leer_params_combinar(configuracion_t *config, int argc, char *argv[]) {
	config->extra_config = &extra;
	extra.alpha = atof(argv[argc - 1]);
}

void aplicar_combinar(configuracion_t *config)
{
	combinar_fn_t *combinar = SWITCH_C_ASM ( config, combinar_c, combinar_asm ) ;
	combinar_params_t *extra = (combinar_params_t*)config->extra_config;

	

	buffer_info_t info = config->src;
	combinar(info.bytes, config->dst.bytes, info.width, info.height, info.width_with_padding,
	         config->dst.width_with_padding, extra->alpha);

}

void ayuda_combinar()
{
	printf ( "       * combinar\n" );
	printf ( "           Parámetros     : \n"
	         "                         alpha valor entre 0.0 y 255.0\n");
	printf ( "           Ejemplo de uso : \n"
	         "                         combinar -i c facil.bmp 128.0\n" );
}


