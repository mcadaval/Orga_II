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
; 	r8 = src_row_size
; 	r9 = dst_row_size
; 	xmm0 = alpha

global combinar_asm

extern combinar_c

section .text

combinar_asm:
	push rbp
	mov rbp,rsp
	push rbx
	push r12
	push r13
	push r14
	push r15

	xor r14,r14
	xor r15,r15
	xor rax,rax
	mov rbx,rsi 	;guardo una copia de de rsi
	mov r13d,r9d 	;guardo en r13d src_row_size ya que lo voy a modificar
	shr r13d,1
	.cicloexterno:
	cmp r14d,ecx  ; i < filas
	je .fin
	mov rax,r14         ;rax = i
	mul r13             ;rax = i*src_row_size
	mov r12,rax         ;r12 = i*src_row_size
	lea rsi,[rsi+r12]  	;cargo la fila correcta
	lea rbx,[rbx+r12] 	;cargo la fila correcta
	mov r10,rsi         ;guardo en r10 la dir de rsi una vez posicionado en la fila
	mov r11,rbx 		;guardo en r11 el la dir de rbx una vez posicionado en la fila
	add rbx,r9          ;rbx pasa al final de la fila
	sub rbx,16          ;rbx se posiciona al comienzo de los ultimos 4 pixeles de la fila
	
	add r14,1           ;aumento 1 en las filas para la prox iteracion

		.ciclointerno:
		cmp r15d,r13d   ; j < dst_row_size/2
		je .cicloexterno
		movdqu xmm1,[rbx]   ; xmm1 = Pixel(n) | Pixel(n-1) | Pixel(n-2) | Pixel (n-3)
		movdqu xmm2,[rsi]   ; xmm2 = Pixel(3) | Pixel(2) | Pixel(1) | Pixel (0)
		movdqu [rsi],xmm1   ; rsi = Pixel(n-3),Pixel(n-2),Pixel(n-1),Pixel(n)...
		movdqu [rbx],xmm2 	; rbx = Pixel(0),Pixel(1),Pixel(2),Pixel(3)...

		add rsi,16
		sub rbx,16
		add r15d,16
		cmp r15d,r13d
		jne .sigo          ;si estoy en la ultima iteracion del ciclointerno
		mov rsi,r10        ;restauro el valor original de rsi antes de entrar al ciclo interno   
		mov rbx,r11        ;restauro el valor original en rbx antes de entrar al ciclo interno  
		;esto es para que en la proxima iteracion del ciclo externo cuando se cargue en rsi,rbx_
		;las filas correctas lo haga desde la posicion que tenia antes de entar al ciclo interno(antes de ser modificados en otras palabras)
		.sigo:
		jmp .ciclointerno

	;sub rsp, 8
	;call combinar_c
	;add rsp, 8
	.fin:
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rbp
	ret
