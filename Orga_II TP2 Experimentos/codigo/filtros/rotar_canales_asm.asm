section .data
DEFAULT REL

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
	push r12
	push r13
	push r14


	mov eax,edx 			;eax = edx (columnas)
	mul ecx     			;eax = edx * ecx (columnas * filas)
	.ciclo:
	cmp eax,0
	je .fin
	mov bl,[rdi]  ; bl = B_source
	add rdi,1
	mov r12b,[rdi] ; r12b = G_source
	add rdi,1
	mov r13b,[rdi] ; r13b = R_source
	add rdi,1
	mov r14b,[rdi] ;r14b = A_source
	add rdi,1  ;rdi apunta al siguiente pixel para la siguiente iteracion

	mov [rsi],r12b   	; B_destino = G_source
	add rsi,1
	mov [rsi],r13b       ; G_destino = R_source
	add rsi,1
	mov [rsi],bl         ;R_destino = B_source
	add rsi,1
	mov [rsi],r14b      ;A_destino = A_source
	add rsi,1        ;aumento rsi para la proxima iteracion

	dec eax                 ;decrementa 1 la iteracion del ciclo que son la cantidad de pixeles de la imagen
	jmp .ciclo

	.fin:
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rbp
	ret
