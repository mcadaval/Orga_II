
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tp2.h"
#include "helper/tiempo.h"
#include "helper/libbmp.h"
#include "helper/utils.h"
#include "helper/imagenes.h"

// ~~~ seteo de los filtros ~~~

#define N_ENTRADAS_colorizar 1
#define N_ENTRADAS_combinar 1
#define N_ENTRADAS_pixelar 1
#define N_ENTRADAS_rotar 1
#define N_ENTRADAS_smalltiles 1

DECLARAR_FILTRO(colorizar)
DECLARAR_FILTRO(combinar)
DECLARAR_FILTRO(pixelar)
DECLARAR_FILTRO(rotar)
DECLARAR_FILTRO(smalltiles)

filtro_t filtros[] = {
	DEFINIR_FILTRO(colorizar) ,
	DEFINIR_FILTRO(combinar) ,
	DEFINIR_FILTRO(pixelar) ,
	DEFINIR_FILTRO(rotar) ,
	DEFINIR_FILTRO(smalltiles) ,
	{0,0,0,0,0}
};

// ~~~ fin de seteo de filtros. Para agregar otro debe agregarse ~~~
//    ~~~ una linea en cada una de las tres partes anteriores ~~~

int main( int argc, char** argv ) {

	configuracion_t config;
	config.dst.width = 0;

	procesar_opciones(argc, argv, &config);
	// Imprimo info
	if (!config.nombre)
	{
		printf ( "Procesando...\n");
		printf ( "  Filtro             : %s\n", config.nombre_filtro);
		printf ( "  Implementación     : %s\n", C_ASM( (&config) ) );
		printf ( "  Archivo de entrada : %s\n", config.archivo_entrada);
	}

	filtro_t *filtro = detectar_filtro(&config);

	if (filtro != NULL) {
		filtro->leer_params(&config, argc, argv);
		correr_filtro_imagen(&config, filtro->aplicador);
	}

	return 0;
}

filtro_t* detectar_filtro(configuracion_t *config)
{
	for (int i = 0; filtros[i].nombre != 0; i++)
	{
		if (strcmp(config->nombre_filtro, filtros[i].nombre) == 0)
			return &filtros[i];
	}

	fprintf(stderr, "Filtro desconocido\n");
	return NULL; // avoid C warning
}


void imprimir_tiempos_ejecucion(unsigned long long int start, unsigned long long int end, int cant_iteraciones) {
	unsigned long long int cant_ciclos = end-start;

	printf("Tiempo de ejecución:\n");
	printf("  Comienzo                          : %llu\n", start);
	printf("  Fin                               : %llu\n", end);
	printf("  # iteraciones                     : %d\n", cant_iteraciones);
	printf("  # de ciclos insumidos totales     : %llu\n", cant_ciclos);
	printf("  # de ciclos insumidos por llamada : %.3f\n", (float)cant_ciclos/(float)cant_iteraciones);
}

void correr_filtro_imagen(configuracion_t *config, aplicador_fn_t aplicador)
{
	snprintf(config->archivo_salida, sizeof  (config->archivo_salida), "%s/%s.%s.%s%s.bmp",
             config->carpeta_salida, basename(config->archivo_entrada),
             config->nombre_filtro,  C_ASM(config), config->extra_archivo_salida );

	if (config->nombre)
	{
		printf("%s\n", basename(config->archivo_salida));
	}
	else
	{
		imagenes_abrir(config);
		unsigned long long start, end;
		MEDIR_TIEMPO_START(start)
		for (int i = 0; i < config->cant_iteraciones; i++) {
				aplicador(config);
		}
		MEDIR_TIEMPO_STOP(end)
		imagenes_guardar(config);
		imagenes_liberar(config);
		imprimir_tiempos_ejecucion(start, end, config->cant_iteraciones);
	}
}
