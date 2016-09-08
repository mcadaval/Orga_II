
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

	
	for (int i = 0; i<filas; i+=1) {          	//voltear horizontal la imagen dst (sacado de utils.c)
		for (int j = 0; j<cols; j+=1) {
			dst_matrix[i][cols-j-1] = src_matrix[i][j];
		}
	}
	
	for (int f = 0; f < filas; f++) {   		//recorro la imagen dst aplicando la formula con el dst viejo(el volteado) y src  
		for (int c = 0; c < cols; c++) {   
			bgra_t *p_d = (bgra_t*) &dst_matrix[f][c * 4];
            bgra_t *p_s = (bgra_t*) &src_matrix[f][c * 4];
            
			p_d->b = ( (alpha * (p_s->b - p_d->b)) / (float)255.00) + p_d->b;      
			p_d->g = ( (alpha * (p_s->g - p_d->g)) / (float)255.00) + p_d->g;
			p_d->r = ( (alpha * (p_s->r - p_d->r)) / (float)255.00) + p_d->r;
			p_d->a = p_s->a;

		}
	}
}
