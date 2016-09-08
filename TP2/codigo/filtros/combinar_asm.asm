; void combinar_asm (
; 	unsigned char *src,
; 	unsigned char *dst,
; 	int cols,
; 	int filas,
; 	int src_row_size,
; 	int dst_row_size,
; 	float alpha
; );

; Parámetros:
; 	rdi = src
; 	rsi = dst
; 	rdx = cols
; 	rcx = filas
; 	r8 = dst_row_size
; 	r9 = dst_row_size
; 	xmm0 = alpha

global combinar_asm

extern combinar_c

section .text

combinar_asm:
	;; TODO: Implementar

	sub rsp, 8

	call combinar_c

	add rsp, 8

	ret
