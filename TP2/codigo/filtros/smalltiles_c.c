
#include "../tp2.h"


// void smalltiles_c (unsigned char *src, unsigned char *dst, int cols, int filas, int src_row_size, int dst_row_size) {
	
// 	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
// 	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;
	
// 	// iteramos sobre los pixeles de indices parese por filas
// 	for (int f = 0; f < filas / 2; f++) {
// 		for (int c = 0; c < cols / 2; c++) {
// 			// puntero a pixel en src 
//       bgra_t *p_s = (bgra_t*) &src_matrix[f * 2][c * 8];
// 			// punteros a pixel en dest
// 			bgra_t *p_d1 = (bgra_t*) &dst_matrix[filas / 2 + f][c * 4];
// 			bgra_t *p_d2 = (bgra_t*) &dst_matrix[filas / 2 + f][src_row_size / 2 + c * 4];
// 			bgra_t *p_d3 = (bgra_t*) &dst_matrix[f][c * 4];
// 			bgra_t *p_d4 = (bgra_t*) &dst_matrix[f][src_row_size / 2 + c * 4];
  
//   		// pegamos el mismo pixel en cada uno de los cuatro cuadrantes
// 			*p_d1 = *p_s;
// 			*p_d2 = *p_s;
// 			*p_d3 = *p_s;
// 			*p_d4 = *p_s;
// 		}
// 	}
// }

void smalltiles_c (unsigned char *src, unsigned char *dst, int cols, int filas, int src_row_size, int dst_row_size) {
	
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;
	
	// iteramos sobre los pixeles de indices pares por columnas
	for (int c = 0; c < cols / 2; c++) {
		for (int f = 0; f < filas / 2; f++) {
			// puntero a pixel en src 
      bgra_t *p_s = (bgra_t*) &src_matrix[f * 2][c * 8];
			// punteros a pixel en dest
			bgra_t *p_d1 = (bgra_t*) &dst_matrix[filas / 2 + f][c * 4];
			bgra_t *p_d2 = (bgra_t*) &dst_matrix[filas / 2 + f][src_row_size / 2 + c * 4];
			bgra_t *p_d3 = (bgra_t*) &dst_matrix[f][c * 4];
			bgra_t *p_d4 = (bgra_t*) &dst_matrix[f][src_row_size / 2 + c * 4];
  
  		// pegamos el mismo pixel en cada uno de los cuatro cuadrantes
			*p_d1 = *p_s;
			*p_d2 = *p_s;
			*p_d3 = *p_s;
			*p_d4 = *p_s;
		}
	}
}

// void smalltiles_c (unsigned char *src, unsigned char *dst, int cols, int filas, int src_row_size, int dst_row_size) {
	
// 	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
// 	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;
	


// 	// iteramos sobre cada cuadrante
// 	for (int i = 0; i < 4; i++) {
// 		// dependiendo del i determinamos en que cuadrante estamos
// 		// | 2 | 3 |
// 		// | 0 | 1 |
// 		int offset_filas = 0;
// 		int offset_cols = 0;
// 		switch (i) {
// 			case 1:
// 				offset_cols = src_row_size / 2;
// 				break;
// 			case 2:
// 				offset_filas = filas / 2;
// 				break;
// 			case 3:
// 				offset_filas = filas / 2;
// 				offset_cols = src_row_size / 2;
// 				break;
// 			default:
// 				break;
// 		}
// 		// iteramos sobre el cuadrante en que estamos y pegamos el pixel correspondiente
// 		for (int f = 0; f < filas / 2; f++) {
// 			for (int c = 0; c < cols / 2; c++) {
// 				bgra_t *p_s = (bgra_t*) &src_matrix[f * 2][c * 8];
// 				bgra_t *p_d = (bgra_t*) &dst_matrix[offset_filas + f][offset_cols + c * 4];	
// 			 	*p_d = *p_s;
// 			}
// 		}			
// 	}
// }


