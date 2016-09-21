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
  mov ebx, r8d            ; rbx = src_row_size
  shr rbx, 1              ; rbx = src_row_size / 2 (guardamos esto aca porque vamos a usarlo mucho)

  ; preparamos 4 punteros, uno para recorrer cada cuadrante

  mov r10, rsi            ; r10 = puntero al inicio del primer cuadrante de src (abajo izquierda)
  
  mov r11, rsi      
  add r11, rbx            ; r11 = puntero al inicio del segundo cuadrante de src (abajo derecha) 

  mov r12, rsi            
  mov r9d, ecx
  shr r9, 1
  imul r9d, r8d           ; r9d = src_row_size * filas/2
  add r12, r9             ; r12 = puntero al inicio del tercer cuadrante de src (arriba izquierda)  

  mov r13, r12
  add r13, rbx            ; r13 = puntero al inicio del cuarto cuadrante de src (arriba derecha)
  
  xor r14, r14            ; r14 = cantidad de iteraciones del ciclo de filas
  
  ; r14 = iteracion filas
  ; r15 = iteracion columnas

  ; mov r10, rdi
  ; mov r11, rdi
  ; add r11, src_row_size/2
  ; mov r12, rdi
  ; add r12, src_row_size * filas/2
  ; mov r13, r12
  ; add r13, src_row_size/2

  ; ; vemos el resto de dividir cols/2 por 4 para saber cuanto nos tenemos que mover cuando lleguemos a la ultima iteracion de cada fila. Si no hacemos esto podemos acceder a cosas incorrectas.

  ; mov eax, edx          ; rax = cols
  ; shr rax, 1            ; rax = cols / 2
  ; xor rdx, rdx          ; rdx = 0
  ; mov rbx, 4            ; rbx = 4
  ; div rbx               ; el resto de la division rdx:rax/rbx queda guardando en rdx


  .iterar_filas:
    cmp r14d, ecx        ; verificamos si ya terminamos de recorrer todas las filas
    jge .fin
    add r14d, 2          ; incrementamos de a 2 porque solo nos interesa recorrer las filas de indice par
    xor r15, r15         ; r15 = cantidad de iteraciones del ciclo de columnas

  .iterar_columnas:
    cmp r15d, edx                 ; verificamos si ya terminamos de recorrer todas las columnas
    jge .reubicar_punteros        ; si termiamos con una fila reubicamos los 4 punteros         
    
    ; buscamos 4 pixeles de la imagen src de los cuales solo nos sirven 2, y los guardamos en cada uno de los cuadrantes de la imagen dst 
    movdqu xmm0, [rax]            ; xmm0 = |P3|P2|P1|P0|
    pshufd xmm1, xmm0, 10000000b  ; xmm1 = |P1|P0|P0|P0|
    ; pextrq r9, xmm1, 0b           ; r9 = P1P0 
    movdqu [r10], xmm1
    movdqu [r11], xmm1
    movdqu [r12], xmm1
    movdqu [r13], xmm1

    ; mov [r10], r9
    ; mov [r11], r9
    ; mov [r12], r9
    ; mov [r13], r9

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

  .fin
    ; restauramos los registros 
    add rsp, 8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret









;  ; vamos a recorrer individualmente cada cuadrante de la imagen destino y lo vamos completando

;   xor r10, r10          ; r10 = 0
;   mov r10d, ecx         ; r10 = filas
;   shr r10d, 3           ; r10 = filas / 8     (dejamos fija esta constante)

;   xor r11, r11          ; r11 = 0
;   mov r11d, edx         ; r11 = cols
;   shr r11d, 3           ; r11 = cols / 8      
;   sub r11, 1            ; r11 = cols / 8 - 1  (dejamos fija esta constante)


;   xor r12, r12          ; r12 = 0  (en r12 vamos guardando la fila en la que estamos) 
;   mov rax, rsi          ; en rax vamos guardando el puntero al pixel en el que estamos parados

;   ; vemos el resto de dividir cols/2 por 4 para saber cuanto nos tenemos que mover cuando lleguemos a la ultima iteracion de cada fila. Sino accedemos a cosas incorrectas.

;   mov eax, edx          ; rax = cols
;   shr rax, 1            ; rax = cols / 2
;   xor rdx, rdx          ; rdx = 0
;   mov rbx, 4            ; rbx = 4
;   div rbx               ; el resto de la division rdx:rax/rbx queda guardando en rdx


;   iterar_filas:
;     cmp r12, r10        ; verificamos si ya terminamos de recorrer el cuadrante
;     jge .fin
;     inc r12             ; incrementamos el numero de filas
;     xor r13, r13        ; r13 = 0  (en r13 vamos guardando la columna en la que estamos)
   
;     iterar_columnas:
;       cmp r13, r11      ; verificamos si ya terminamos de recorrer la fila
;       jge .iterar_filas 
;       lea rdx, [r13+1]  ; verificamos si estamos en la ultima iteracion de la fila  

;       cmp 
;       mov 


    
;     movups xmm0, cuadrante
;     movd xmm1, r10d
;     pshufd xmm1, xmm1, 0d
;     pcmpeqb xmm0, xmm1


;     pinsrd xmm1, r10d, 01d
;     PCMPEQB mm0
    
; PINSRD 
; 	ret
