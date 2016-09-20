
#include "../tp2.h"

float clamp(float pixel)
{
	float res = pixel < 0.0 ? 0.0 : pixel;
	return res > 255.0 ? 255 : res;
}

void combinar_c (
	unsigned char *src, 
	unsigned char *dst, 
	int cols, 
	int filas, 
	int src_row_size,
	int dst_row_size,
	float alpha
) {

	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;

	
	for (int i = 0; i<filas; i+=1) {          	//en estos ciclos voy hacer la imagen espejo
		for (int j = 0; j<src_row_size/2; j+=4) {  //sumo de a 4 bytes (tam de 1 pixel)
			bgra_t *p_d = (bgra_t*) &dst_matrix[i][j];	//creo un puntero al primer pixel
			bgra_t *p_d2 = (bgra_t*) &dst_matrix[i][src_row_size - j - 4];	//creo un puntero al ultimo pixel	
			bgra_t copia;
			copia = *p_d2;  
			*p_d2 = *p_d;  // muevo los pixels del final al principio de cada fila
			*p_d = copia;  // muevo los pixeles del principio al final de cada fila	

		}
	}

	for (int f = 0; f < filas; f++) {
		for (int c = 0; c < cols; c++) {
			bgra_t *p_d = (bgra_t*) &dst_matrix[f][c * 4];
            bgra_t *p_s = (bgra_t*) &src_matrix[f][c * 4];

			p_d->b = ( ( alpha * (p_s->b - p_d->b)) / 255.00) + p_d->b;
			p_d->g = ( ( alpha * (p_s->g - p_d->g)) / 255.00) + p_d->g;
			p_d->r = ( ( alpha * (p_s->r - p_d->r)) / 255.00) + p_d->r;
			p_d->a = ( ( alpha * (p_s->a - p_d->a)) / 255.00) + p_d->a;

		}
	}

}
