; void smalltiles_asm (
;   unsigned char *src,
;   unsigned char *dst,
;   int cols,
;   int filas,
;   int src_row_size,
;   int dst_row_size
; ); 

; Parámetros:
;   rdi = src
;   rsi = dst
;   edx = cols
;   ecx = filas
;   r8d = src_row_size
;   r9d = dst_row_size

section .data
DEFAULT REL

section .rodata
; 0_dwords: dd 0,0,0,0
; cuadrante: dd 0,1,2,3

section .text
global smalltiles_asm
smalltiles_asm:  

  ; guardamos registros y dejamos la pila alineada
  push rbp          ; Alineada
  mov rbp, rsp
  push rbx          ; Desalineada
  push r12          ; Alineada
  push r13          ; Desalineada
  push r14          ; Alineada
  push r15          ; Desalineada
  sub rsp, 8        ; Alineada

  mov rax, rdi            ; rax = puntero al pixel en el que estamos parados en src
  mov rbx, r8             ; rbx = src_row_size
  shr rbx, 1              ; rbx = src_row_size / 2 (guardamos esto aca porque vamos a usarlo mucho)

  ; preparamos 4 punteros, uno para recorrer cada cuadrante

  mov r10, rsi            ; r10 = puntero al inicio del primer cuadrante de dst (abajo izquierda)
  
  mov r11, rsi      
  add r11, rbx            ; r11 = puntero al inicio del segundo cuadrante de dst (abajo derecha) 

  mov r12, rsi            
  mov r9, rcx
  shr r9, 1
  imul r9, r8             ; r9 = src_row_size * filas/2
  add r12, r9             ; r12 = puntero al inicio del tercer cuadrante de dst (arriba izquierda)  

  mov r13, r12
  add r13, rbx            ; r13 = puntero al inicio del cuarto cuadrante de dst (arriba derecha)
  
  xor r14, r14            ; r14 = cantidad de iteraciones del ciclo de filas

  .iterar_filas:
    cmp r14, rcx        ; verificamos si ya terminamos de recorrer todas las filas
    jge .fin
    add r14, 2          ; incrementamos de a 2 porque solo nos interesa recorrer las filas de indice par
    xor r15, r15        ; r15 = cantidad de iteraciones del ciclo de columnas

  .iterar_columnas:
    cmp r15, rdx                  ; verificamos si ya terminamos de recorrer todas las columnas
    jge .reubicar_punteros        ; si termiamos con una fila reubicamos los 4 punteros         
    
    ; buscamos 4 pixeles de la imagen src de los cuales solo nos sirven 2, y los guardamos en cada uno de los cuadrantes de la imagen dst 
    movdqu xmm0, [rax]            ; xmm0 = |P3|P2|P1|P0|
    pshufd xmm1, xmm0, 00001000b  ; xmm1 = |P0|P0|P2|P0|
    movq r9, xmm1 

    mov [r10], r9
    mov [r11], r9
    mov [r12], r9
    mov [r13], r9

    add r10, 8
    add r11, 8
    add r12, 8
    add r13, 8

    add rax, 16                   ; movemos el puntero a src 4 pixels
    add r15, 4                    ; incrementamos r15 en 4 unidades pues procesamos 4 columnas
    jmp .iterar_columnas

  .reubicar_punteros:
    add r10, rbx
    add r11, rbx  
    add r12, rbx  
    add r13, rbx
    add rax, rbx
    add rax, rbx
    jmp .iterar_filas  

  .fin:
    ; restauramos los registros 
    add rsp, 8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret


; Implementación alternativa para experimentos

; section .text
; global smalltiles_asm
; smalltiles_asm:  

;   ; guardamos registros y dejamos la pila alineada
;   push rbp          ; Alineada
;   mov rbp, rsp
;   push rbx          ; Desalineada
;   push r12          ; Alineada
;   push r13          ; Desalineada
;   push r14          ; Alineada
;   push r15          ; Desalineada
;   sub rsp, 8        ; Alineada

;   mov rax, rdi            ; rax = puntero al pixel en el que estamos parados en src
;   mov rbx, r8             ; rbx = src_row_size
;   shr rbx, 1              ; rbx = src_row_size / 2 (guardamos esto aca porque vamos a usarlo mucho)

;   ; preparamos 4 punteros, uno para recorrer cada cuadrante

;   mov r10, rsi            ; r10 = puntero al inicio del primer cuadrante de dst (abajo izquierda)

;   mov r11, rsi      
;   add r11, rbx            ; r11 = puntero al inicio del segundo cuadrante de dst (abajo derecha) 

;   mov r12, rsi            
;   mov r9, rcx
;   shr r9, 1
;   imul r9, r8             ; r9 = src_row_size * filas/2
;   add r12, r9             ; r12 = puntero al inicio del tercer cuadrante de dst (arriba izquierda)  

;   mov r13, r12
;   add r13, rbx            ; r13 = puntero al inicio del cuarto cuadrante de dst (arriba derecha)
  
;   ;offset para mover de punta a punta src
;   mov rdi, rcx
;   sub rdi, 2
;   imul rdi, r8

;   ;offset para mover de punta a punta los punteros de dst
;   mov rsi, rcx
;   shr rsi, 1 
;   sub rsi, 1
;   imul rsi, r8

;   xor r14, r14            ; r14 = cantidad de iteraciones del ciclo de filas
;   shr rcx, 1

;   .iterar_filas:
;     cmp r14, rcx        ; verificamos si ya terminamos de recorrer todas las filas
;     jge .fin
;     add r14, 2          ; incrementamos de a 2 porque solo nos interesa recorrer las filas de indice par
;     xor r15, r15        ; r15 = cantidad de iteraciones del ciclo de columnas

;   .iterar_columnas:
;     cmp r15, rdx                  ; verificamos si ya terminamos de recorrer todas las columnas
;     jge .reubicar_punteros        ; si termiamos con una fila reubicamos los 4 punteros         
    
;     ; buscamos 4 pixeles de la imagen src de los cuales solo nos sirven 2, y los guardamos en cada uno de los cuadrantes de la imagen dst 
;     movdqu xmm0, [rax]            ; xmm0 = |P3|P2|P1|P0|
;     pshufd xmm1, xmm0, 00001000b  ; xmm1 = |P0|P0|P2|P0|
;     movq r9, xmm1 

;     mov [r10], r9
;     mov [r11], r9
;     mov [r12], r9
;     mov [r13], r9

;     add rax, rdi

;     movdqu xmm0, [rax]            ; xmm0 = |P3|P2|P1|P0|
;     pshufd xmm1, xmm0, 00001000b  ; xmm1 = |P0|P0|P2|P0|
;     movq r9, xmm1

;     sub rax, rdi

;     add r10, rsi
;     add r11, rsi
;     add r12, rsi
;     add r13, rsi

;     mov [r10], r9
;     mov [r11], r9
;     mov [r12], r9
;     mov [r13], r9

;     sub r10, rsi
;     sub r11, rsi
;     sub r12, rsi
;     sub r13, rsi


;     add r10, 8
;     add r11, 8
;     add r12, 8
;     add r13, 8

;     add rax, 16                   ; movemos el puntero a src 4 pixels
;     add r15, 4                    ; incrementamos r15 en 4 unidades pues procesamos 4 columnas
;     jmp .iterar_columnas

;   .reubicar_punteros:
;     add r10, rbx
;     add r11, rbx  
;     add r12, rbx  
;     add r13, rbx
;     add rax, rbx
;     add rax, rbx
    
;     sub rdi, r8
;     sub rdi, r8
;     sub rdi, r8
;     sub rdi, r8

;     sub rsi, r8
;     sub rsi, r8
;     jmp .iterar_filas  

;   .fin:
;     ; restauramos los registros 
;     add rsp, 8
;     pop r15
;     pop r14
;     pop r13
;     pop r12
;     pop rbx
;     pop rbp
;     ret
