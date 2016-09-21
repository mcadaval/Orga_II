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
cte_4 : dd 4.0, 4.0, 4.0, 4.0, 


section .text

pixelar_asm:
	push rbp
	mov rbp,rsp
	push rbx  ;puntero que apunta siempre a la fila de arriba de rdi
	push r12  ;contador de columnas
	push r13  ;contador de filas
	push r14
	
	
	pxor xmm15,xmm15
	movdqu xmm14,[cte_4]
	xor r13,r13  ;contador de filas arranca en 0 
	shr rdx,2  ;cols = cols/4



	mov rbx,rdi
	.cicloexterno:
	xor r12,r12 ;contador de columnas se resetea en 0 por cada ciclo externo
	cmp r13d,ecx ; i < filas 
	je .fin

	cmp r13,0   ;si es la primera vez que ingreso al ciclo no aumentar rdi una fila entera para posicionarse en la siguiente
	je .sigo  
	lea rdi,[rdi+r8]
	lea rsi,[rsi+r9]
	
	.sigo:
	lea rbx,[rbx+r8]
	add r13d,2  

	.ciclointerno:                                                      ;          .     .    .    .                                   
	cmp r12,rdx ; j < cols/4                                                       .     .    .    .
	je .cicloexterno                                                    ;          .     .    .    .
	movdqu xmm0,[rdi]   ; xmm0 =  P3 | P2 | P1 | P0 |                            | P4 | P5 | P6 | P7 | . . .
	movdqu xmm1,[rbx]   ; xmm1 =  P7 | P6 | P5 | P4 |                   matriz = | P0 | P1 | P2 | P3 | . . .   Los promedios seran P0,P1,P4,P5 para el primer cuadrado
	                                                                    ;                                      y P2,P3,P6,P7 para el segundo cuadrado
	movdqu xmm2,xmm0 
	punpcklbw xmm0,xmm15   ;xmm0 = 0 | A1 | 0 | R1 | 0 | G1 | 0 | B1 | 0 | A0 | 0 | R0 | 0 | G0 | 0 | B0
	punpckhbw xmm2,xmm15   ;xmm2 = 0 | A3 | 0 | R3 | 0 | G3 | 0 | B3 | 0 | A2 | 0 | R2 | 0 | G2 | 0 | B2

	movdqu xmm3,xmm0       ;xmm3 = 0 | A1 | 0 | R1 | 0 | G1 | 0 | B1 | 0 | A0 | 0 | R0 | 0 | G0 | 0 | B0
	punpcklwd xmm0,xmm15   ;xmm0 = 0 | 0 | 0 | A0 | 0 | 0 | 0 | R0 | 0 | 0 | 0 | G0 | 0 | 0 | 0 | B0
	punpckhwd xmm3,xmm15   ;xmm3 = 0 | 0 | 0 | A1 | 0 | 0 | 0 | R1 | 0 | 0 | 0 | G1 | 0 | 0 | 0 | B1

	movdqu xmm4,xmm2       ;xmm4 = 0 | A3 | 0 | R3 | 0 | G3 | 0 | B3 | 0 | A2 | 0 | R2 | 0 | G2 | 0 | B2
	punpcklwd xmm2,xmm15   ;xmm2 = 0 | 0 | 0 | A2 | 0 | 0 | 0 | R2 | 0 | 0 | 0 | G2 | 0 | 0 | 0 | B2
	punpckhwd xmm4,xmm15   ;xmm4 = 0 | 0 | 0 | A3 | 0 | 0 | 0 | R3 | 0 | 0 | 0 | G3 | 0 | 0 | 0 | B3

	movdqu xmm5,xmm1
	punpcklbw xmm1,xmm15   ;xmm1 = 0 | A5 | 0 | R5 | 0 | G5 | 0 | B5 | 0 | A4 | 0 | R4 | 0 | G4 | 0 | B4
	punpckhbw xmm5,xmm15   ;xmm5 = 0 | A7 | 0 | R7 | 0 | G7 | 0 | B7 | 0 | A6 | 0 | R6 | 0 | G6 | 0 | B6

	movdqu xmm6,xmm1       ;xmm6 = 0 | A5 | 0 | R5 | 0 | G5 | 0 | B5 | 0 | A4 | 0 | R4 | 0 | G4 | 0 | B4
	punpcklwd xmm1,xmm15   ;xmm1 = 0 | 0 | 0 | A4 | 0 | 0 | 0 | R4 | 0 | 0 | 0 | G4 | 0 | 0 | 0 | B4
	punpckhwd xmm6,xmm15   ;xmm6 = 0 | 0 | 0 | A5 | 0 | 0 | 0 | R5 | 0 | 0 | 0 | G5 | 0 | 0 | 0 | B5

	movdqu xmm7,xmm5       ;xmm7 = 0 | A7 | 0 | R7 | 0 | G7 | 0 | B7 | 0 | A6 | 0 | R6 | 0 | G6 | 0 | B6
	punpcklwd xmm5,xmm15   ;xmm5 = 0 | 0 | 0 | A6 | 0 | 0 | 0 | R6 | 0 | 0 | 0 | G6 | 0 | 0 | 0 | B6
	punpckhwd xmm7,xmm15   ;xmm7 = 0 | 0 | 0 | A7 | 0 | 0 | 0 | R7 | 0 | 0 | 0 | G7 | 0 | 0 | 0 | B7

	paddd xmm0,xmm3        ;xmm0 = 0 | 0 | 0 | A0 + A1 | 0 | 0 | 0 | R0 + R1 | 0 | 0 | 0 | G0 + G1 | 0 | 0 | 0 | B0 + B1
	paddd xmm1,xmm6        ;xmm1 = 0 | 0 | 0 | A4 + A5 | 0 | 0 | 0 | R4 + R5 | 0 | 0 | 0 | G4 + G5 | 0 | 0 | 0 | B4 + B5
	paddd xmm0,xmm1        ;xmm0 = 0 | 0 | 0 | A0 + A1 + A4 + A5 | 0 | 0 | 0 | R0 + R1 + R4 + R5 | 0 | 0 | 0 | G0 + G1 + G4 + G5 | 0 | 0 | 0 | B0 + B1 + B4 + B5

	paddd xmm2,xmm4        ;xmm2 = 0 | 0 | 0 | A2 + A3 | 0 | 0 | 0 | R2 + R3 | 0 | 0 | 0 | G2 + G3 | 0 | 0 | 0 | B2 + B3
	paddd xmm5,xmm7        ;xmm5 = 0 | 0 | 0 | A6 + A7 | 0 | 0 | 0 | R6 + R7 | 0 | 0 | 0 | G6 + G7 | 0 | 0 | 0 | B6 + B7
	paddd xmm2,xmm5        ;xmm2 = 0 | 0 | 0 | A2 + A3 + A6 + A7 | 0 | 0 | 0 | R2 + R3 + R6 + R7 | 0 | 0 | 0 | G2 + G3 + G6 + G7 | 0 | 0 | 0 | B2 + B3 + B6 + B7

	cvtdq2ps xmm0,xmm0
	cvtdq2ps xmm2,xmm2

	divps xmm0,xmm14 ; xmm0 =  0 | 0 | 0 | A1 + A5 + A0 + A4  / 4 | 0 | 0 | 0 | R1 + R5 + R0 + R4 / 4 | 0 | 0 | 0 | G1 + G5 + G0 + G4  / 4| 0 | 0 | 0 | B1 + B5 + B0 + B4 /4   (promedio de valores para el primer cuadrado)
	divps xmm2,xmm14 ; xmm2 =  0 | 0 | 0 | A2 + A3 + A6 + A7  / 4 | 0 | 0 | 0 | R2 + R3 + R6 + R7 / 4 | 0 | 0 | 0 | G2 + G3 + G6 + G7  / 4| 0 | 0 | 0 | B2 + B3 + B6 + B7 /4   (promedio de valores para el segundo cuadrado)

	cvtps2dq xmm0,xmm0
	cvtps2dq xmm2,xmm2

	packusdw xmm0,xmm0  ; xmm0 = 0 | A1 + A5 + A0 + A4  / 4 | 0 | R1 + R5 + R0 + R4 / 4 | 0 | G1 + G5 + G0 + G4  / 4 | 0 | B1 + B5 + B0 + B4 /4 | 0 | A1 + A5 + A0 + A4  / 4 | 0 | R1 + R5 + R0 + R4 / 4 | 0 | G1 + G5 + G0 + G4  / 4 | 0 | B1 + B5 + B0 + B4 /4
	packusdw xmm2,xmm2  ; xmm2 = 0 | A2 + A3 + A6 + A7  / 4 | 0 | R2 + R3 + R6 + R7 / 4 | 0 | G2 + G3 + G6 + G7  / 4 | 0 | B2 + B3 + B6 + B7 /4 | 0 | A2 + A3 + A6 + A7  / 4 | 0 | R2 + R3 + R6 + R7 / 4 | 0 | G2 + G3 + G6 + G7  / 4 | 0 | B2 + B3 + B6 + B7 /4 |
	
   	packuswb xmm0,xmm2  ; xmm0 = A2 + A3 + A6 + A7  / 4 | R2 + R3 + R6 + R7 / 4 | G2 + G3 + G6 + G7  / 4 |  B2 + B3 + B6 + B7 /4 | A2 + A3 + A6 + A7  / 4 | R2 + R3 + R6 + R7 / 4 | G2 + G3 + G6 + G7  / 4 |  B2 + B3 + B6 + B7 /4 | A1 + A5 + A0 + A4  / 4 | R1 + R5 + R0 + R4 / 4 | G1 + G5 + G0 + G4  / 4 | B1 + B5 + B0 + B4 /4 | A1 + A5 + A0 + A4  / 4 | R1 + R5 + R0 + R4 / 4 | 0 | G1 + G5 + G0 + G4  / 4 | B1 + B5 + B0 + B4 /4A1 + A5 + A0 + A4  / 4 | R1 + R5 + R0 + R4 / 4 | G1 + G5 + G0 + G4  / 4 | B1 + B5 + B0 + B4 /4 | 0 | A1 + A5 + A0 + A4  / 4 | 0 | R1 + R5 + R0 + R4 / 4 | 0 | G1 + G5 + G0 + G4  / 4 | 0 | B1 + B5 + B0 + B4 /4
	

    mov r14,rsi        ;hago una copia de rsi
    lea r14,[r14+r9]   ;muevo el puntero a la fila de arriba desde donde esta ubicado rsi
    movdqu [rsi],xmm0  ;proceso los pixeles del primer cuadrado y del segundo de la fila de abajo
    movdqu [r14],xmm0  ;proceso los pixeles del primer cuadrado y del segundo cuadrado de la fila de arriba

    
    add rsi,16
    add rdi,16
    add rbx,16
    add r12,1
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

