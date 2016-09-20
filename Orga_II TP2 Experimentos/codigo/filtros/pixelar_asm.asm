; void pixelar_asm (
; 	unsigned char *src,
; 	unsigned char *dst,
; 	int cols,
; 	int filas,
; 	int src_row_size,
; 	int dst_row_size
; );

; Parámetros:
; 	rdi = src
; 	rsi = dst
; 	rdx = cols
; 	rcx = filas
; 	r8 = src_row_size
; 	r9 = dst_row_size

extern pixelar_c

global pixelar_asm

section .text

pixelar_asm:
	;; TODO: Implementar

	sub rsp, 8

	call pixelar_c

	add rsp, 8

	ret
