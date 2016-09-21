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


section .data
cte_4 : dw 4, 4, 4, 4, 4, 4, 4, 4,


section .text

pixelar_asm:
	push rbp
	mov rbp,rsp
	push rbx  ;rbx y r12 se mueven por las columnas
	push r12
	push r13
	push r14
		
	xor r10,r10  	;contador de filas
	shr edx,1       ;cols = cols/2
	mov r14d,edx
	
	movdqu xmm13,[cte_4]

	pxor xmm15,xmm15
	sub rcx,2
	.cicloexterno:
	xor r11,r11     ;contador de columnas que se resetea x cada ciclo externo
	cmp r10,rcx     ; i < filas
	je .fin

	xor rax,rax
	mov rax,r10    		; rax = i
	mul r8         		; rax = i * src_row_size
	mov r13,rax    		; r13 = i * src_row_size
	lea rdi,[rdi+r13]	; rdi se actualiza cada dos filas
	mov rbx,rdi         ; copio a rbx la dir de rdi
	mov r12,rbx
	lea r12,[r12+r8] 	; accedo al pixel de arriba

	add r10,2           ;ya que avanza de a dos filas . i = i + 2
		
		.ciclointerno:
		cmp r11d,r14d    ; j < cols/2
		je .cicloexterno
		movq xmm0,[r12]  ; xmm0 = 0  | 0   | P3  | P2 |
		pslldq xmm0,8    ; xmm0 = P3 | P2  | 0   | 0  |
		movq xmm1,[rbx]  ; xmm1 = 0  |  0  |  P1 | P0 |
		por xmm0,xmm1    ; xmm0 = P3 | P2  | P1  | P0 |

		movdqu xmm2,xmm1
		punpcklbw xmm1,xmm15   ; xmm1 = 0 | A1 | 0 | R1 | 0 | G1 | 0 | B1 | 0 | A0 | 0 | R0 | 0 | G0 | 0 | B0
		punpckhbw xmm2,xmm15   ; xmm2 = 0 | A3 | 0 | R3 | 0 | G3 | 0 | B3 | 0 | A2 | 0 | R2 | 0 | G2 | 0 | B2

		paddb xmm1,xmm2  ; xmm1 = 0 | A1 + A3 | 0 | R1 + R3 | 0 | G1 + G3 | 0 | B1 + B3 | 0 | A0 + A2 | 0 | R0 + R2 | 0 | G0 + G2 | 0 | B0 + B2        
		movdqu xmm3,xmm1  
		psrldq xmm3,8    ; xmm3 = 0 |    0   | 0  |   0    | 0  |   0    |  0 |   0     | 0  | A1 + A3 | 0 | R1 + R3 | 0 | G1 + G3 | 0 | B1 + B3 

		paddw xmm1,xmm3  ; xmm1 = 0 |    0   | 0  |   0    | 0  |   0    |  0 |   0     | 0  | A0 + A2 + A1 + A3 | 0 | R0 + R2 + R1 + R3 | 0 | G0 + G2 + G1 + G3 | 0 | B0 + B2 + B1 + B3 
		
		psrlw xmm1,xmm13 	; xmm1 = 0 |  0 | 0 | 0  | A0 + A2 + A1 + A3  / 4 | R0 + R2 + R1 + R3 / 4 | G0 + G2 + G1 + G3  / 4| B0 + B2 + B1 + B3 /4
		packuswb xmm1,xmm15 ; xmm1 = 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | SA | SR | SG | SB |

		mov rsi,rbx         ;copio la dir de rbx en rsi (actualmente en el pixel de la esquina de abajo a la izq)
		movd [rsi],xmm1     ;muevo los nuevos valores de los pixeles a rsi
		add rsi,4           ;muevo rsi 4 mas adelante (pixel de abajo a la der)
		movd [rsi],xmm1     ;muevo los nuevos valores de los pixeles a rsi
		mov rsi,r12         ;muevo a rsi el valor de r12 (los pixels ela fila de arriba )
		movd [rsi],xmm1      ;muevo los nuevos valores de los pixeles a rsi
		add rsi,4             ;muevo rsi 4 mas adelante (pixel de arriba a la der)
		movd [rsi],xmm1     ;muevo los nuevos valores de los pixeles a rsi

		add rbx,8
		add r12,8
		add r11d,1
		
		jmp .ciclointerno

	.fin:

	pop r14
	pop r13
	pop r12
	pop rbx
	pop rbp
	ret



;	sub rsp, 8

;	call pixelar_c

;	add rsp, 8