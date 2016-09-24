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
	push r14  						;contador de filas
	push r15  						;contador de columnas
	sub rsp,8

	xor r12,r12
	xor r13,r13
	xor r14,r14
	xor rbx,rbx

	pshufd xmm0,xmm0, 00000000b 	; xmm0 = alpha | alpha | alpha | alpha |  (alineo el alpha para multiplicar cada pixel en el xmm)
	movups xmm3,[cte_255]           ; xmm3 = 255.0 | 255.0 | 255.0 | 255.0
	divps xmm0 ,xmm3                ; xmm0 = alpha / 255.0 | alpha / 255.0 | alpha 255.0 | alpha / 255.0
	pxor xmm15,xmm15
	
	mov r12d,edx                    ;r12d =  cols
	shr r12d,2                      ;r12d = cols/4   (cantidad de iteraciones en ciclo interno)
	mov r13d,r8d                    ;r13d = src_row_size
	
	.cicloexterno:
	xor r15,r15 										;pongo columnas en 0
	cmp r14d,ecx					; i < filas	
	jge .fin
	mov rbx,rdi                     ;muevo a rbx la direccion de la fila actual
	lea rbx,[rbx+r13]               ;actualizo rbx al final de la fila actual
	sub rbx,16                      ;reapunto rbx a los ultimos 4 pixeles       
	add r14d,1                      ;incremento la fila para la proxima iteracion

		.ciclointerno:
		cmp r15d,r12d          	; j < cols/4
		jge .cicloexterno  
		movdqu xmm1,[rdi]       ; xmm1 =  P3 | P2 | P1 | P0 |
		movdqu xmm2,[rbx]       ; xmm2 =  Pn | P(n-1) | P(n-2) | P(n-3) |

		movdqu xmm4,xmm1
		punpcklbw xmm1,xmm15   ; xmm1 = 0 | A1 | 0 | R1 | 0 | G1 | 0 | B1 | 0 | A0 | 0 | R0 | 0 | G0 | 0 | B0
		punpckhbw xmm4,xmm15   ; xmm4 = 0 | A3 | 0 | R3 | 0 | G3 | 0 | B3 | 0 | A2 | 0 | R2 | 0 | G2 | 0 | B2

		movdqu xmm5,xmm1
		punpcklwd xmm1,xmm15   ;xmm1 = 0 | 0 | 0 | A0 | 0 | 0 | 0 | R0 | 0 | 0 | 0 | G0 | 0 | 0 | 0 | B0
		punpckhwd xmm5,xmm15   ;xmm5 = 0 | 0 | 0 | A1 | 0 | 0 | 0 | R1 | 0 | 0 | 0 | G1 | 0 | 0 | 0 | B1

		movdqu xmm6,xmm4
		punpcklwd xmm4,xmm15   ;xmm4 = 0 | 0 | 0 | A2 | 0 | 0 | 0 | R2 | 0 | 0 | 0 | G2 | 0 | 0 | 0 | B2
		punpckhwd xmm6,xmm15   ;xmm6 = 0 | 0 | 0 | A3 | 0 | 0 | 0 | R3 | 0 | 0 | 0 | G3 | 0 | 0 | 0 | B3

		movdqu xmm7,xmm2       ;preservo xmm2 para luego al final sumarlo en la formula
		movdqu xmm8,xmm7
		punpcklbw xmm7,xmm15   ; xmm7 = 0 | An-2 | 0 | Rn-2 | 0 | Gn-2 | 0 | Bn-2 | 0 | An-3 | 0 | Rn-3 | 0 | Gn-3 | 0 | Bn-3
		punpckhbw xmm8,xmm15   ; xmm8 = 0 | An | 0 | Rn | 0 | Gn | 0 | Bn | 0 | An-1 | 0 | Rn-1 | 0 | Gn-1 | 0 | Bn-1

		movdqu xmm9,xmm7
		punpcklwd xmm7,xmm15   ; xmm7 = 0 | 0 | 0 | An-3 | 0 | 0 | 0 | Rn-3 | 0 | 0 | 0 | Gn-3 | 0 | 0 | 0 | Bn-3
		punpckhwd xmm9,xmm15   ; xmm9 = 0 | 0 | 0 | An-2 | 0 | 0 | 0 | Rn-2 | 0 | 0 | 0 | Gn-2 | 0 | 0 | 0 | Bn-2

		movdqu xmm10,xmm8      
		punpcklwd xmm8,xmm15   ; xmm8 = 0 | 0 | 0 | An-1 | 0 | 0 | 0 | Rn-1 | 0 | 0 | 0 | Gn-1 | 0 | 0 | 0 | Bn-1
		punpckhwd xmm10,xmm15  ;xmm10 = 0 | 0 | 0 | An | 0 | 0 | 0 | Rn | 0 | 0 | 0 | Gn | 0 | 0 | 0 | Bn

		psubd xmm1,xmm10         ; xmm1 = 0 | 0 | 0 | A0 -An-3 | 0 | 0 | 0 | R0 - Rn-3 | 0 | 0 | 0 | G0 - Gn-3 | 0 | 0 | 0 | B0 - Bn-3
		psubd xmm5,xmm8	   		 ; xmm5 = 0 | 0 | 0 | A1 -An-2 | 0 | 0 | 0 | R1 - Rn-2 | 0 | 0 | 0 | G1 - Gn-2 | 0 | 0 | 0 | B1 - Bn-2
		psubd xmm4,xmm9          ; xmm4 = 0 | 0 | 0 | A2 -An-1 | 0 | 0 | 0 | R2 - Rn-1 | 0 | 0 | 0 | G2 - Gn-1 | 0 | 0 | 0 | B2 - Bn-1
		psubd xmm6,xmm7       	 ; xmm6 = 0 | 0 | 0 | A3 -An   | 0 | 0 | 0 | R3 - Rn   | 0 | 0 | 0 | G3 - Gn   | 0 | 0 | 0 | B3 - Bn 

		cvtdq2ps xmm1,xmm1     ; (float) xmm1 = 0 | 0 | 0 | A0 -An-3 | 0 | 0 | 0 | R0 - Rn-3 | 0 | 0 | 0 | G0 - Gn-3 | 0 | 0 | 0 | B0 - Bn-3
		cvtdq2ps xmm5,xmm5     ; (float) xmm5 = 0 | 0 | 0 | A1 -An-2 | 0 | 0 | 0 | R1 - Rn-2 | 0 | 0 | 0 | G1 - Gn-2 | 0 | 0 | 0 | B1 - Bn-2
		cvtdq2ps xmm4,xmm4     ; (float) xmm4 = 0 | 0 | 0 | A2 -An-1 | 0 | 0 | 0 | R2 - Rn-1 | 0 | 0 | 0 | G2 - Gn-1 | 0 | 0 | 0 | B2 - Bn-1
		cvtdq2ps xmm6,xmm6	   ; (float) xmm6 = 0 | 0 | 0 | A3 -An   | 0 | 0 | 0 | R3 - Rn   | 0 | 0 | 0 | G3 - Gn   | 0 | 0 | 0 | B3 - Bn 


		mulps xmm1,xmm0        ; (float) xmm1 = 0 | 0 | 0 | (A0 -An-3) * alpha/255.0 | 0 | 0 | 0 | (R0 - Rn-3) * alpha/255.0  | 0 | 0 | 0 | (G0 - Gn-3) * alpha/255.0  | 0 | 0 | 0 | (B0 - Bn-3) *  alpha/255.0 
		mulps xmm5,xmm0
		mulps xmm4,xmm0
		mulps xmm6,xmm0

		cvtps2dq xmm1,xmm1    ; (int) xmm1 = 0 | 0 | 0 | (A0 -An-3) * alpha/255.0 | 0 | 0 | 0 | (R0 - Rn-3) * alpha/255.0  | 0 | 0 | 0 | (G0 - Gn-3) * alpha/255.0  | 0 | 0 | 0 | (B0 - Bn-3) *  alpha/255.0
		cvtps2dq xmm5,xmm5
		cvtps2dq xmm4,xmm4
		cvtps2dq xmm6,xmm6

		paddd xmm1,xmm10     ; (int) xmm1 = 0 | 0 | 0 | (A0 -An-3) * alpha/255.0 + An-3 = (A'0) | 0 | 0 | 0 | (R0 - Rn-3) * alpha/255.0 + Rn-3 = (R'0)| 0 | 0 | 0 | (G0 - Gn-3) * alpha/255.0 + Gn-3 = (G'0)| 0 | 0 | 0 | (B0 - Bn-3) *  alpha/255.0 + Bn-3  =(B'0)
		paddd xmm5,xmm8
		paddd xmm4,xmm9
		paddd xmm6,xmm7
	
		
		;xmm1 = P'0
		;xmm5 = P'1
		;xmm4 = P'2
		;xmm6 = P'3

		packusdw xmm1,xmm5   ; xmm1 = 0 | A'1 | 0 | R'1 | 0 | G'1 | 0 | B'1 | 0 | A'0 | 0 | R'0 | 0 | G'0 | 0 | B'0  = P'1 | P'0
		packusdw xmm4,xmm6	 ; xmm4 = 0 | A'3 | 0 | R'3 | 0 | G'3 | 0 | B'3 | 0 | A'2 | 0 | R'2 | 0 | G'2 | 0 | B'2  = P'3 | P'2

		packuswb xmm1,xmm4   ; xmm5 = P'3 | P'2 | P'1 | P'0

		movdqu [rsi],xmm1    ; rsi = P'0,P'1,P'2,P'3...

		add rsi,16             ; rsi suma 16 B
		add rdi,16             ; rdi suma 16 B para avanzar a los siguientes 4 pixeles
		sub rbx,16             ; rbx resta 16B para retroceder a los siguientes 4 pixeles
		add r15d,1             ;aumento el contador
		jmp .ciclointerno

	.fin:
	add rsp,8
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



;		movdqu xmm1,[rdi]       ; xmm1 =  P3 | P2 | P1 | P0 |
;		movdqu xmm2,[rbx]       ; xmm2 =  Pn | P(n-1) | P(n-2) | P(n-3) |
;		psubusb xmm1,xmm2       ; xmm1 = P3-Pn | P2 - P(n-1) | P1 - P(n-2) | P0 - P(n-3) |
		
;		pxor xmm4,xmm4          ; limpio xmm4
;		cvtdq2pd xmm4,xmm1      ; xmm4 = |       P'1         |          P'0              |
;		psrldq xmm1,8           ; xmm1 = | 0  | 0 | P3-Pn | P2 - P(n-1) |
;		pxor xmm5,xmm5          ; limpio xmm5
;		cvtdq2pd xmm5,xmm1      ; xmm5 = |       P'3         |          P'2              |
		
;		cvtpd2ps xmm6,xmm4      ; xmm6 = | A(P'0) | R(P'0) | G(P'0) | B(P'0) | 
;		psrldq xmm4,8           ; xmm4 = |        0          |          P'1              |
;		cvtpd2ps xmm7,xmm4      ; xmm7 = | A (P'1)| R(P'1) | G(P'1) | B(P'1) |

;		cvtpd2ps xmm8,xmm5      ; xmm8 = | A (P'2)| R(P'2) | G(P'2) | B(P'2) |
;		psrldq xmm5,8           ; xmm4 = |        0          |          P'3              |
;		cvtpd2ps xmm9,xmm5      ; xmm9 = | A (P'3)| R(P'3) | G(P'3) | B(P'3) |

;		mulps xmm6,xmm0         ;xmm6 = | A (P'0) * alpha/255.0 | R(P'0) * alpha/255.0 | G(P'0) * alpha/255.0 | B(P'0) * alpha/255.0
;		mulps xmm7,xmm0			;xmm7 = | A (P'1) * alpha/255.0 | R(P'1) * alpha/255.0 | G(P'1) * alpha/255.0 | B(P'1) * alpha/255.0
;		mulps xmm8,xmm0			;xmm8 = | A (P'2) * alpha/255.0 | R(P'2) * alpha/255.0 | G(P'2) * alpha/255.0 | B(P'2) * alpha/255.0
;		mulps xmm9,xmm0			;xmm9 = | A (P'3) * alpha/255.0 (float) | R(P'3) * alpha/255.0 (float) | G(P'3) * alpha/255.0 | B(P'3) * alpha/255.0 (float)
;;
;		cvtps2dq xmm6,xmm6      ;xmm6 = | A (P'0) * alpha/255.0 (int) | R(P'0) * alpha/255.0 (int)| G(P'0) * alpha/255.0 (int)| B(P'0) * alpha/255.0 (int)
;		cvtps2dq xmm7,xmm7		;xmm7 = | A (P'0) * alpha/255.0 | R(P'0) * alpha/255.0 | G(P'0) * alpha/255.0 | B(P'0) * alpha/255.0
;		cvtps2dq xmm8,xmm8		;xmm8 = | A (P'0) * alpha/255.0 | R(P'0) * alpha/255.0 | G(P'0) * alpha/255.0 | B(P'0) * alpha/255.0
;		cvtps2dq xmm9,xmm9		;xmm9 = | A (P'0) * alpha/255.0 | R(P'0) * alpha/255.0 | G(P'0) * alpha/255.0 | B(P'0) * alpha/255.0
;
;		packusdw xmm6,xmm7     ; xmm6 = P'1 | P'0
;		packusdw xmm8,xmm9     ; xmm8 = P'3 | P'2
;
;		packuswb xmm6,xmm8     ; xmm6 = P'3 | P'2 | P'1 | P'0
;
;		movdqu [rsi],xmm6          