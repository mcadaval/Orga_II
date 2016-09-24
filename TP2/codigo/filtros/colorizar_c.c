
#include "../tp2.h"
#include "stdlib.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

unsigned char maximo (unsigned char *list, short size) {
  unsigned char max = '0';
  for (int i = 0; i < size; i++) {
    if (max < list[i])
      max = list[i];
  }
  return max;
}

void colorizar_c (
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

	// iteramos sobre todos los pixeles excepto los de los extremos
  for (int f = 1; f < filas - 1; f++) {
    for (int c = 1; c < cols - 1; c++) {
      // en estas tres variables vamos a ir guardando los maximos
      unsigned char max_red = 0;
      unsigned char max_green = 0;
      unsigned char max_blue = 0;
      
      // iteramos sobre los 9 pixeles en base a la poscion en la que estamos y vamos verificando si encontramos un nuevo maximo para cada color
      for (int i = f - 1; i < f + 2; i++) {
        for (int j = c - 1; j < c + 2; j++) {
          // puntero al pixel al que vamos a acceder
          bgra_t *p_s = (bgra_t*) &src_matrix[i][j*4];
          max_red = (p_s->r > max_red) ? p_s->r : max_red;
          max_green = (p_s->g > max_green) ? p_s->g : max_green;   
          max_blue = (p_s->b > max_blue) ? p_s->b : max_blue;
        }
      }

      // calculamos los phi en base a los maximos obtenidos
      float phi_red = ((max_red >= max_green) & (max_red >= max_blue)) ? 1.0 + alpha : 1.0 - alpha;
      float phi_green = ((max_red < max_green) & (max_green >= max_blue)) ? 1.0 + alpha : 1.0 - alpha;  
      float phi_blue = ((max_red < max_blue) & (max_green < max_blue)) ? 1.0 + alpha : 1.0 - alpha;  

      // puntero al pixel en el que estamos posicionados en src
      bgra_t *current = (bgra_t*) &src_matrix[f][c*4];
      
      // puntero al pixel en el que estamos posicionados en dst
      bgra_t *p_d = (bgra_t*) &dst_matrix[f][c*4];

      // seteamos los nuevos valores
      p_d->r = min(255, phi_red * current->r);
      p_d->g = min(255, phi_green * current->g);
      p_d->b = min(255, phi_blue * current->b);
    }
  }

}