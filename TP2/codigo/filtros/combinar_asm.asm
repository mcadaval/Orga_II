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
; 	edx = cols
; 	ecx = filas
; 	r8d = src_row_size
; 	r9d = dst_row_size
; 	xmm0 = alpha

global combinar_asm

extern combinar_c

section .rodata
cte_255: dd 255.0,255.0,255.0,255.0

section .text

combinar_asm:
	push rbp
	mov rsp,rbp
	push rbx
	push r12
	push r13
	push r14  ;contador de filas
	push r15  ;contador de columnas

	xor r14,r14
	xor r15,r15

	mov rbx,rsi 	;creo una copia del puntero a dst 
	mov eax,r9d
	mov r12d,2
	div r12d
	mov r12d,eax 	;muevo a r12d = dst_row_size/2 (media fila) lo que aumentara rsi con cada iteracion externa (cuando cambia de fila)
	mov r13d,r9d
	add r13d,r12d 	;muevo a r13d = dst_row_size/2 + dst_row_size (una fila y media) lo que aumentara rbx con cada iteracion externa (cuando cambia de fila)
	

	lea rbx,[rbx+r9] ;voy hasta alfinal de la fila 
	sub rbx,16       ;reapunto rbx a los ultimos 4 pixeles de la fila 
	
	pshufd xmm0,xmm0, 00000000b 	; xmm0 = alpha | alpha | alpha | alpha |  (alineo el alpha para multiplicar cada pixel en el xmm)
	movdqu xmm3,[cte_255]           ; xmm3 = 255.0 | 255.0 | 255.0 | 255.0

	.cicloexterno:
	cmp r14d,ecx      ; i < filas
	je .fin
	mov eax,r14d      ; eax = i
	mul r12d          ; eax = i * dst_row_size/2
	mov r10,rax
	xor rax,rax
	mov eax,r14d      ; eax = i
	mul r13d          ; eax = i * (dst_row_size/2 + dst_row_size)
	mov r11,rax
	lea rsi,[rsi+r10] ;rsi apunta al principio de la fila actual
	lea rbx,[rbx+r11] ;rbx apunta al final de la fla actual
	sub rbx,16        ;rbx apunta a los ultimos 4 pixeles de la fila
	add r14d,1

		.ciclointerno:
		cmp r15d,r12d          	; j < dst_row_size/2
		movdqu xmm1,[rsi]       ; xmm1 =  P3 | P2 | P1 | P0 |
		movdqu xmm2,[rbx]       ; xmm2 =  Pn | P(n-1) | P(n-2) | P(n-3) |
		psubusb xmm1,xmm2       ; xmm1 = P3-Pn | P2 - P(n-1) | P1 - P(n-2) | P0 - P(n-3) |




	.fin:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rbp
	ret

	;sub rsp, 8
	;call combinar_c
	;add rsp, 8