section .data
DEFAULT REL
mascara : db 0x1, 0x2, 0x0, 0x3, 0x5, 0x6, 0x4, 0x7, 0x9, 0xA, 0x8, 0xB, 0xD, 0xE, 0xC, 0xF,
; esta mascara rota correctamente los canales reubicando los indices del registro xmm de la siguiente manera  
; xmm = | A3 (0xF) | R3 (0xE) | G3 (0xD) | B3 (0xC) .... A0 (0x3) | R0 (0x2)| G0 (0x1)| B0 (0x0)
; xmm luego del shuffle con la mascara que cree : 
; xmm = | A3 (0xF) | B3 (0xC) | R3 (0xE) | G3 (0xD) .... A0 (0x3) | B0 (0x0)| R0 (0x2)| G0 (0x1)

section .text
global rotar_asm
rotar_asm:
;rdi = unsigned char *src
;rsi = unsigned char *dst
;edx = int cols
;ecx = int filas
;r8d = int src_row_size
;r9d = int dst_row_size
	push rbp
	mov rbp,rsp
	push rbx


	mov eax,edx 			;eax = edx (columnas)
	mul ecx     			;eax = edx * ecx (columnas * filas)
	mov ebx,4
	div ebx					;eax = eax / ebx ((columnas * filas )/ 4)
	movdqu xmm3,[mascara] 	;muevo a xmm1  el contendio de la mascara para acceder 1 sola vez a memoria
	.ciclo:
	cmp eax,0
	je .fin
	movdqu xmm1,[rdi]       ; xmm1 = | A3 | R3 | G3 | B3 | .... | A0  | R0 | G0 | B0        
	pshufb xmm1, xmm3		; xmm1 = | A3 | B3 | G3 | G3 | .... | A0  | B0 | R0 | G0  reordeno todo sobre xmm1
	movdqu [rsi],xmm1       ;[rsi] = | A3 | B3 | R3 | G3 | .... | A0  | B0 | R0 | G0  ...... paso el contenido de xmm1 a la dir de la imagen destino

	add rdi,16              ;actualizo los puntero 16 posiciones xq es lo que leeo en cada iteracion
	add rsi,16
	dec eax                 ; decrementa 1 la iteracion del ciclo que son la cantidad de pixeles de la imagen sobre 16
	jmp .ciclo

	.fin:
	pop rbx
	pop rbp
	ret



; Implementación alternativa sin simd

; section .data
; DEFAULT REL

; section .text
; global rotar_asm
; rotar_asm:
; ;rdi = unsigned char *src
; ;rsi = unsigned char *dst
; ;edx = int cols
; ;ecx = int filas
; ;r8d = int src_row_size
; ;r9d = int dst_row_size
; 	push rbp
; 	mov rbp,rsp
; 	push rbx
; 	push r12
; 	push r13
; 	push r14


; 	mov eax,edx 			;eax = edx (columnas)
; 	mul ecx     			;eax = edx * ecx (columnas * filas)
; 	.ciclo:
; 	cmp eax,0
; 	je .fin
; 	mov bl,[rdi]  ; bl = B_source
; 	add rdi,1
; 	mov r12b,[rdi] ; r12b = G_source
; 	add rdi,1
; 	mov r13b,[rdi] ; r13b = R_source
; 	add rdi,1
; 	mov r14b,[rdi] ;r14b = A_source
; 	add rdi,1  ;rdi apunta al siguiente pixel para la siguiente iteracion

; 	mov [rsi],r12b   	; B_destino = G_source
; 	add rsi,1
; 	mov [rsi],r13b       ; G_destino = R_source
; 	add rsi,1
; 	mov [rsi],bl         ;R_destino = B_source
; 	add rsi,1
; 	mov [rsi],r14b      ;A_destino = A_source
; 	add rsi,1        ;aumento rsi para la proxima iteracion

; 	dec eax                 ;decrementa 1 la iteracion del ciclo que son la cantidad de pixeles de la imagen
; 	jmp .ciclo

; 	.fin:
; 	pop r14
; 	pop r13
; 	pop r12
; 	pop rbx
; 	pop rbp
; 	ret

