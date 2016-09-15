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
	
	shr r9d,3        ; r8d =  dst_row_size/8

	pshufd xmm0,xmm0, 00000000b 	; xmm0 = alpha | alpha | alpha | alpha |  (alineo el alpha para multiplicar cada pixel en el xmm)
	movdqu xmm3,[cte_255]           ; xmm3 = 255.0 | 255.0 | 255.0 | 255.0
	divps xmm0 ,xmm3                ; xmm0 = alpha / 255.0 | alpha / 255.0 | alpha 255.0 | alpha / 255.0

	.cicloexterno:
	cmp r14d,ecx      ; i < filas
	je .fin
	xor rax,rax
	mov eax,r14d      ; eax = i
	mul r12d          ; eax = i * dst_row_size/2
	mov r10,rax
	xor rax,rax
	mov eax,r14d      ; eax = i
	mul r13d          ; eax = i * (dst_row_size/2 + dst_row_size)
	mov r11,rax       ; r11 = i * (dst_row_size/2 + dst_row_size)
	lea rdi,[rdi+r10] ;rdi apunta al principio de la fila actual
	lea rsi,[rsi+r10] ;rsi apunta al principio de la fila actual
	lea rbx,[rbx+r11] ;rbx apunta al final de la fla actual
	sub rbx,16        ;rbx apunta a los ultimos 4 pixeles de la fila
	add r14d,1

		.ciclointerno:
		cmp r15d,r9d          	; j < dst_row_size/8  
		movdqu xmm1,[rdi]       ; xmm1 =  P3 | P2 | P1 | P0 |
		movdqu xmm2,[rbx]       ; xmm2 =  Pn | P(n-1) | P(n-2) | P(n-3) |
		psubusb xmm1,xmm2       ; xmm1 = P3-Pn | P2 - P(n-1) | P1 - P(n-2) | P0 - P(n-3) |
		
		cvtpi2pd xmm4,xmm1      ; xmm4 = |       P'1         |          P'0              |
		psrldq xmm1,8           ; xmm1 = | 0  | 0 | P3-Pn | P2 - P(n-1) |
		cvtpi2pd xmm5,xmm1      ; xmm5 = |       P'3         |          P'2              |
		
		cvtpd2ps xmm6,xmm4      ; xmm6 = | A(P'0) | R(P'0) | G(P'0) | B(P'0) | 
		psrldq xmm4,8           ; xmm4 = |        0          |          P'1              |
		cvtpd2ps xmm7,xmm4      ; xmm7 = | A (P'1)| R(P'1) | G(P'1) | B(P'1) |

		cvtpd2ps xmm8,xmm5      ; xmm8 = | A (P'2)| R(P'2) | G(P'2) | B(P'2) |
		psrldq xmm5,8           ; xmm4 = |        0          |          P'3              |
		cvtpd2ps xmm9,xmm5      ; xmm9 = | A (P'3)| R(P'3) | G(P'3) | B(P'3) |

		mulps xmm6,xmm0         ;xmm6 = | A (P'0) * alpha/255.0 | R(P'0) * alpha/255.0 | G(P'0) * alpha/255.0 | B(P'0) * alpha/255.0
		mulps xmm7,xmm0			;xmm7 = | A (P'1) * alpha/255.0 | R(P'1) * alpha/255.0 | G(P'1) * alpha/255.0 | B(P'1) * alpha/255.0
		mulps xmm8,xmm0			;xmm8 = | A (P'2) * alpha/255.0 | R(P'2) * alpha/255.0 | G(P'2) * alpha/255.0 | B(P'2) * alpha/255.0
		mulps xmm9,xmm0			;xmm9 = | A (P'3) * alpha/255.0 | R(P'3) * alpha/255.0 | G(P'3) * alpha/255.0 | B(P'3) * alpha/255.0






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