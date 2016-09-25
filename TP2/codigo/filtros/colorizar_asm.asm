; void colorizar_asm (
; 	unsigned char *src,
; 	unsigned char *dst,
; 	int cols,
; 	int filas,
; 	int src_row_size,
; 	int dst_row_size,
;   float alpha
; );

; Parámetros:
; 	rdi = src
; 	rsi = dst
; 	rdx = cols
; 	rcx = filas
; 	r8 = src_row_size
; 	r9 = dst_row_size
;   xmm0 = alpha


global colorizar_asm

section .rodata
unos_flotantes: dd 1.0,1.0,1.0,1.0
alpha_caso1: dd -1.0,-1.0,1.0,0.0      
alpha_caso2: dd -1.0,1.0,-1.0,0.0    
alpha_caso3a: dd -1.0,-1.0,1.0,0.0  
alpha_caso3b: dd 1.0,-1.0,-1.0,0.0    
alpha_caso4: dd 1.0,-1.0,-1.0,0.0
full_255: dd 255,255,255,255

; mask_byte1: db 1,2,2,5,6,6,0,0,0,0,0,0,0,0,0,0
; mask_byte2: db 0,0,1,4,4,5,0,0,0,0,0,0,0,0,0,0

mask_byte1: db 1,0,2,0,2,0,5,0,6,0,6,0,0,0,0,0
mask_byte2: db 0,0,0,0,1,0,4,0,4,0,5,0,0,0,0,0

mask_and: db 255,0,255,0,255,0,255,0,255,0,255,0,0,0,0,0
mask_shuf: db 0,2,4,6,8,10,0,0,0,0,0,0,0,0,0,0
section .text

colorizar_asm:
  ; guardamos registros y dejamos la pila alineada
  push rbp      ; Alineada
  mov rbp, rsp
  push rbx      ; Deslineada
  push r12      ; Alineada
  push r13      ; Desalineada
  push r14      ; Alineada

  ; seteamos 5 punteros
  mov r12, rdi          ; r12 = puntero a fila de abajo de vecinos en src
  
  mov r13, rdi
  add r13, r8           ; r13 = puntero a fila del medio de vecinos en src
  
  mov r14, r13
  add r14, r8           ; r14 = puntero a fila de arriba de vecinos en src

  add rdi, r8           
  add rdi, 4            ; rdi = puntero al pixel donde estoy posicionado en src
  
  add rsi, r9           
  add rsi, 4            ; rsi = puntero al pixel donde estoy posicionado en dst

  mov r10, 2            ; r10 = cantidad de iteraciones del ciclo de filas (inicializado en 2 porque no queremos recorrer los extremos)

  ; iteramos sobre todos los pixels de la imagen src con excepcion de las filas y columnas de los extremos
  .iterar_filas:
    cmp r10, rcx        ; verificamos si ya terminamos
    jge .fin 
    inc r10             ; incrementamos r10

    mov r11, 2          ; r11 = cantidad de iteraciones del ciclo de columnas (inicializado en 2 porque no queremos recorrer los extremos)
 
  .iterar_columnas:
    cmp r11, rdx        ; verificamos si ya terminamos
    jge .incrementar_punteros 
    add r11, 2             ; incrementamos r11
    jmp .resolver_calculos

  .incrementar_punteros:
    add rdi, 8
    add rsi, 8
    add r12, 8
    add r13, 8
    add r14, 8
    jmp .iterar_filas

  .resolver_calculos:
    ; ahora calculamos los maximos para cada color, de a 2 pixeles a la vez con sus respectivos vecinos
    ; en los comentarios usamos indices especificos para que no quede tan engorroso, pero los indices van variando en cada iteracion
    movdqu xmm1, [r12]  ; xmm1 = |A(0,3)|R(0,3)|G(0,3)|B(0,3)|A(0,2)|R(0,2)|G(0,2)|B(0,2)|A(0,1)|R(0,1)|G(0,1)|B(0,1)|A(0,0)|R(0,0)|G(0,0)|B(0,0)|
    movdqu xmm2, [r13]  ; xmm2 = |A(1,3)|R(1,3)|G(1,3)|B(1,3)|A(1,2)|R(1,2)|G(1,2)|B(1,2)|A(1,1)|R(1,1)|G(1,1)|B(1,1)|A(1,0)|R(1,0)|G(1,0)|B(1,0)|
    movdqu xmm8, xmm2   ; xmm8 = xmm2 (lo guardamos para usarlo al final de la iteracion)
    movdqu xmm3, [r14]  ; xmm3 = |A(2,3)|R(2,3)|G(2,3)|B(2,3)|A(2,2)|R(2,2)|G(2,2)|B(2,2)|A(2,1)|R(2,1)|G(2,1)|B(2,1)|A(2,0)|R(2,0)|G(2,0)|B(2,0)|
    
    pmaxub xmm1, xmm2   ; xmm1 = |max(A(0,3),A(1,3))|max(R(0,3),R(1,3))|max(G(0,3),G(1,3))|max(B(0,3),B(1,3))|...
    pmaxub xmm1, xmm3   ; xmm1 = |max(A(0,3),A(1,3),A(2,3))|max(R(0,3),R(1,3),R(2,3))|max(G(0,3),G(1,3),G(2,3))|max(B(0,3),B(1,3),B(2,3))|...

    ; cambiamos la notacion para que sea entendible
    ; xmm1 = |M_A3|M_R3|M_G3|M_B3|M_A2|M_R2|M_G2|M_B2|M_A1|M_R1|M_G1|M_B1|M_A0|M_R0|M_G0|M_B0|
    movdqu xmm2, xmm1   ; xmm2 = xmm1
    psrldq xmm2, 4      ; xmm2 = |0|0|0|0|M_A3|M_R3|M_G3|M_B3|M_A2|M_R2|M_G2|M_B2|M_A1|M_R1|M_G1|M_B1|
    pmaxub xmm2, xmm1   ; xmm2 = |..|..|..|..|max(M_A2,M_A3)|max(M_R2,M_R3)|max(M_G2,M_G3)|max(M_B2,M_B3)|...
    movdqu xmm3, xmm2   ; xmm3 = xmm2
    psrldq xmm3, 4      ; xmm2 = |..|..|..|..|..|..|..|..|max(M_A2,M_A3)|max(M_R2,M_R3)|max(M_G2,M_G3)|max(M_B2,M_B3)|...
    pmaxub xmm2, xmm3   ; xmm2 = |..|..|..|..|..|..|..|..|max(M_A2,M_A3,M_A1)|max(M_R2,M_R3,M_R1)|max(M_G2,M_G3,M_G1)|max(M_B2,M_B3,M_B1)|...
    ; entonces nos queda guardado max de cada color de ambos pixeles en los 8 bytes mas bajos de xmm2

    ; calculamos las funciones phi. Basicamente hacemos las comparaciones necesarias para determinar los valores de las funciones (explicado en informe)
    movdqu xmm3, [mask_byte1]     ; ponemos un mascara en xmm3 para hacer shuffle de bytes
    movdqu xmm4, xmm2 
    pshufb xmm2, xmm3             ; xmm2 = ......|MAX_R1|MAX_R1|MAX_G1|MAX_R0|MAX_R0|MAX_G0|
    movdqu xmm3, [mask_byte2]     ; ponemos un mascara en xmm3 para hacer shuffle de bytes
    pshufb xmm4, xmm3             ; xmm4 = ......|MAX_G1|MAX_B1|MAX_B1|MAX_G0|MAX_B0|MAX_B0|
    
    ; extendemos a word con 0s porque sino la comparaciones pueden dar mal, ya que el cmp que tenemos es scon signo
    movdqu xmm5, [mask_and]       
    pand xmm4, xmm5               ; aca ponemos en 0 los bytes mas altos de cada word de xmm4
    pand xmm2, xmm5               ; aca ponemos en 0 los bytes mas altos de cada word de xmm2

    ; pcmpgtb xmm4, xmm2            ; solo nos importan las comparaciones de los bytes que seteamos
    pcmpgtw xmm4, xmm2            ; hacemos la comparacion
    movdqu xmm3, [mask_shuf]      
    pshufb xmm4, xmm3             ; como queremos trabajar con bytes reordenamos los resultados para que queden en los 6 bytes mas bajos

    xor rax, rax
    pmovmskb eax, xmm4      ; pasamos la mascara a 16 bits para poder utilizar los registros de proposito general y los saltos condicionales
    xor ax, 0xFFF           ; negamos los resultados porque nos sirven las negaciones para determinar los saltos
    mov rbx, rax            ; la copiamos para poder hacer la logica con los resultados de los 2 pixels
    and ax, 101b            ; con un and anulamos los 3 bytes mas altos para ver las funciones phi del primer pixel. Tambien anulamos la segunda comparacion pues la usaremos depues solo si es necesario 
    movss xmm6, xmm0 
    shufps xmm6, xmm6, 0d           ; xmm6 = | alpha | alpha | alpha | alpha | 
    movups xmm4, [unos_flotantes]   ; xmm4 = | 1.0 | 1.0 | 1.0 | 1.0 |
    cmp ax, 101b
    je .caso1_p1
    cmp ax, 1b
    je .caso2_p1
    cmp ax, 100b
    je .caso3_p1
    ; caso 4
    ; MAX_R < MAX_G y MAX_G < MAX_B
    ; phi_R = 1 - alpha
    ; phi_G = 1 - alpha
    ; phi_B = 1 + alpha 
    movups xmm5, [alpha_caso4]      ; xmm5 = |0.0|-1.0|-1.0|1.0|
    jmp .sumar_alpha_p1

  .caso1_p1:
    ; MAX_R >= MAX_G y MAX_G >= MAX_B
    ; phi_R = 1 + alpha
    ; phi_G = 1 - alpha
    ; phi_B = 1 - alpha
    movups xmm5, [alpha_caso1]      ; xmm5 = |0.0|1.0|-1.0|-1.0|
    jmp .sumar_alpha_p1

  .caso2_p1:
    ; MAX_R < MAX_G y MAX_G >= MAX_B
    ; phi_R = 1 - alpha
    ; phi_G = 1 + alpha
    ; phi_B = 1 - alpha
    movups xmm5, [alpha_caso2]      ; xmm5 = |0.0|-1.0|1.0|-1.0|
    jmp .sumar_alpha_p1

  .caso3_p1:
    ; MAX_R >= MAX_G y MAX_G < MAX_B
    ; phi_R = todavia no lo sabemos
    ; phi_G = 1 - alpha
    ; phi_B = todavia no lo sabemos
    ; en este caso tenemos que ver el resultado de MAX_R >= MAX_B (lo buscamos en rbx)
    xor rax, rax
    mov ax, bx
    and ax, 10b
    cmp ax, 10b
    je .caso3a_p1
    ; caso 3b
    ; MAX_R >= MAX_G y MAX_G < MAX_B y MAX_R < MAX_B
    ; phi_R = 1 - alpha
    ; phi_G = 1 - alpha
    ; phi_B = 1 + alpha
    movups xmm5, [alpha_caso3b]      ; xmm5 = |0.0|-1.0|-1.0|1.0|
    jmp .sumar_alpha_p1

  .caso3a_p1:
    ; MAX_R >= MAX_G y MAX_G < MAX_B y MAX_R >= MAX_B
    ; phi_R = 1 + alpha
    ; phi_G = 1 - alpha
    ; phi_B = 1 - alpha
    movups xmm5, [alpha_caso3a]       ; xmm5 = |0.0|1.0|-1.0|-1.0|

  .sumar_alpha_p1:
    mulps xmm5, xmm6        ; xmm5 = |+-alpha|+-alpha|+-alpha|+-alpha|
    addps xmm5, xmm4        ; xmm5 = |+-alpha|1 +-alpha|1 +-alpha|1 +-alpha| = |...|phi_R0|phi_G0|phi_B0|

;------------------------------------------------
    ; hacemos exactamente el mismo procedimiento con el otro pixel
    mov rax, rbx          ; copiamos la mascara a rax por si necesitamos acceder a la segunda comparacion
    and bx, 101000b       ; con un and anulamos los 3 bytes mas bajos para ver las funciones phi del segundo pixel. Tambien anulamos la segunda comparacion pues la usaremos depues solo si es necesario 
    ; movss xmm6, xmm0
    ; shufps xmm6, xmm6, 0d           ; xmm6 = | alpha | alpha | alpha | alpha | 
    cmp bx, 101000b
    je .caso1_p2
    cmp bx, 1000b
    je .caso2_p2
    cmp bx, 100000b
    je .caso3_p2
    ; caso 4
    ; MAX_R < MAX_G y MAX_G < MAX_B
    ; phi_R = 1 - alpha
    ; phi_G = 1 - alpha
    ; phi_B = 1 + alpha 
    movups xmm7, [alpha_caso4]       ; xmm5 = |0.0|-1.0|-1.0|1.0|
    jmp .sumar_alpha_p2

  .caso1_p2:
    ; MAX_R >= MAX_G y MAX_G >= MAX_B
    ; phi_R = 1 + alpha
    ; phi_G = 1 - alpha
    ; phi_B = 1 - alpha
    movups xmm7, [alpha_caso1]       ; xmm7 = |0.0|1.0|-1.0|-1.0|
    jmp .sumar_alpha_p2

  .caso2_p2:
    ; MAX_R < MAX_G y MAX_G >= MAX_B
    ; phi_R = 1 - alpha
    ; phi_G = 1 + alpha
    ; phi_B = 1 - alpha
    movups xmm7, [alpha_caso2]       ; xmm7 = |0.0|-1.0|1.0|-1.0|
    jmp .sumar_alpha_p2

  .caso3_p2:
    ; MAX_R >= MAX_G y MAX_G < MAX_B
    ; phi_R = todavia no lo sabemos
    ; phi_G = 1 - alpha
    ; phi_B = todavia no lo sabemos
    ; en este caso tenemos que ver el resultado de MAX_R >= MAX_B (lo buscamos en rbx)
    xor rbx, rbx
    mov bx, ax
    and bx, 10000b
    cmp bx, 10000b
    je .caso3a_p2
    ; caso 3b
    ; MAX_R >= MAX_G y MAX_G < MAX_B y MAX_R < MAX_B
    ; phi_R = 1 - alpha
    ; phi_G = 1 - alpha
    ; phi_B = 1 + alpha
    movups xmm7, [alpha_caso3b]       ; xmm7 = |0.0|-1.0|-1.0|1.0|
    jmp .sumar_alpha_p2

  .caso3a_p2:
    ; MAX_R >= MAX_G y MAX_G < MAX_B y MAX_R >= MAX_B
    ; phi_R = 1 + alpha
    ; phi_G = 1 - alpha
    ; phi_B = 1 - alpha
    movups xmm7, [alpha_caso3a]       ; xmm7 = |0.0|1.0|-1.0|-1.0|

  .sumar_alpha_p2:
    mulps xmm7, xmm6          ; xmm7 = |+-alpha|+-alpha|+-alpha|+-alpha|
    addps xmm7, xmm4          ; xmm7 = |0|1 +-alpha|1 +-alpha|1 +-alpha| = |...|phi_R1|phi_G1|phi_B1|

    ; shuffleamos para tener los pixels que queremos
    pxor xmm2, xmm2           
    pshufd xmm9, xmm8, 1001b  ; xmm9 = |..|..|..|..|..|..|..|..|A(1,2)|R(1,2)|G(1,2)|B(1,2)|A(1,1)|R(1,1)|G(1,1)|B(1,1)|
    
    ; desempaquetamos y convertimos a float
    punpcklbw xmm9, xmm2      ; xmm9 = | 0 |A(1,2)| 0 |(1,2)| 0 |G(1,2)| 0 |B(1,2)| 0 |A(1,1)| 0 |R(1,1)| 0 |G(1,1)| 0 |B(1,1)|
    movdqu xmm10, xmm9        ; xmm10 = xmm9
    
    punpcklwd xmm9, xmm2      ; xmm9 = | 0 | 0 | 0 |A(1,1)| 0 | 0 | 0 |R(1,1)| 0 | 0 | 0 |G(1,1)| 0 | 0 | 0 |B(1,1)|
    cvtdq2ps xmm9, xmm9       ; xmm9 = |A(1,1)|R(1,1)|G(1,1)|B(1,1)| (FLOATS)

    punpckhwd xmm10, xmm2     ; xmm10 = | 0 | 0 | 0 |A(1,2)| 0 | 0 | 0 |R(1,2)| 0 | 0 | 0 |G(1,2)| 0 | 0 | 0 |B(1,2)|
    cvtdq2ps xmm10, xmm10     ; xmm10 = |A(1,2)|R(1,2)|G(1,2)|B(1,2)| (FLOATS)
    
    ; hacemos el producto
    mulps xmm5, xmm9          ; xmm5 = |...|R(1,1)*phi_R0|G(1,1)*phi_G0|B(1,1)*phi_B0|
    mulps xmm7, xmm10         ; xmm7 = |...|R(1,2)*phi_R1|G(1,2)*phi_G1|B(1,2)*phi_B1|

    ; convertimos a int y empaquetamos
    cvtps2dq xmm5, xmm5       ; xmm5 = |...|R(1,1)*phi_R0|G(1,1)*phi_G0|B(1,1)*phi_B0| (INT DW)         
    cvtps2dq xmm7, xmm7       ; xmm7 = |...|R(1,2)*phi_R1|G(1,2)*phi_G1|B(1,2)*phi_B1| (INT DW)

    packusdw xmm5, xmm7       ; xmm5 = |...|R(1,2)*phi_R1|G(1,2)*phi_G1|B(1,2)*phi_B1|...|R(1,1)*phi_R0|G(1,1)*phi_G0|B(1,1)*phi_B0| (INT W)
    packuswb xmm5, xmm5       ; xmm5 = |..|..|..|..|..|..|..|..|...|R(1,2)*phi_R1|G(1,2)*phi_G1|B(1,2)*phi_B1|...|R(1,1)*phi_R0|G(1,1)*phi_G0|B(1,1)*phi_B0| (INT B)

    pextrq rax, xmm5, 0     ; rax = |...|R(1,2)*phi_R1|G(1,2)*phi_G1|B(1,2)*phi_B1|...|R(1,1)*phi_R0|G(1,1)*phi_G0|B(1,1)*phi_B0|
    mov [rsi], rax

    ; movemos los punteros para la proxima iteracion
    add rdi, 8
    add rsi, 8
    add r12, 8
    add r13, 8
    add r14, 8
    jmp .iterar_columnas

  .fin:
    ; restauramos registros y retornamos
    pop r14     
    pop r13      
    pop r12      
    pop rbx   
    pop rbp 
	  ret