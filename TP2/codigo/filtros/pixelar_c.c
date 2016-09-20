
#include "../tp2.h"

void pixelar_c (
	unsigned char *src, 
	unsigned char *dst, 
	int cols, 
	int filas, 
	int src_row_size, 
	int dst_row_size
) {
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;

	for (int f = 0; f < filas; f = f + 2) {
		for (int c = 0; c < cols/2; c++) {
			bgra_t *p_d = (bgra_t*)  &dst_matrix[f][c * 8];
           	bgra_t *p_d2 = (bgra_t*) &dst_matrix[f][c * 8 + 4];
            bgra_t *p_d3 = (bgra_t*) &dst_matrix[f+1][c * 8];
            bgra_t *p_d4 = (bgra_t*) &dst_matrix[f+1][c * 8 + 4];

            bgra_t *p_s = (bgra_t*)  &src_matrix[f][c * 8];
            bgra_t *p_s2 = (bgra_t*) &src_matrix[f][c * 8 + 4];
            bgra_t *p_s3 = (bgra_t*) &src_matrix[f+1][c * 8];
            bgra_t *p_s4 = (bgra_t*) &src_matrix[f+1][c * 8 + 4];
     

            char promedioR;
            char promedioG;
            char promedioB;
            char promedioA;
            promedioR = (p_s->r + p_s2->r + p_s3->r + p_s4->r) / 4;
            promedioG = (p_s->g + p_s2->g + p_s3->g + p_s4->g) / 4;
            promedioB = (p_s->b + p_s2->b + p_s3->b + p_s4->b) / 4;
            promedioA = (p_s->a + p_s2->a + p_s3->a + p_s4->a) / 4;
           

            p_d->r = promedioR;
			p_d->g = promedioG;
			p_d->b = promedioB;
			p_d->a = promedioA;

			p_d2->r = promedioR;
			p_d2->g = promedioG;
			p_d2->b = promedioB;
			p_d2->a = promedioA;

			p_d3->r = promedioR;
			p_d3->g = promedioG;
			p_d3->b = promedioB;
			p_d3->a = promedioA;

			p_d4->r = promedioR;
			p_d4->g = promedioG;
			p_d4->b = promedioB;
			p_d4->a = promedioA;

		}
	}
}
