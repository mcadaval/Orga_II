; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start


;; GDT
extern GDT_DESC

;; IDT
extern IDT_DESC
extern idt_inicializar

;; PIC
extern resetear_pic
extern habilitar_pic

extern screen_pintar_pantalla

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; xchg bx, bx

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0


    ; habilitar A20
    call habilitar_A20

    ; cargar la GDT
    lgdt [GDT_DESC] 

    ; setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1     ; set PE (Protection Enable) bit in CR0 (Control Register 0)
    mov cr0, eax

    ; pasar a modo protegido
    ; index  ti  rpl
    ; 10010  0    00
    jmp 0x90:modoprotegido  

BITS 32
    modoprotegido:    
    ; acomodar los segmentos
    xor eax, eax
    mov ax, 10100000b
    mov ds, ax          ; index = 20, gdt/ldt = 0, rpl = 00 
    mov ss, ax
    mov es, ax
    mov gs, ax

    mov ax, 10110000b
    mov fs, ax          ; index = 22, gdt/ldt = 0, rpl = 00

    ; setear la pila
    mov ebp, 0x27000
    mov esp, 0x27000

    ; pintar pantalla, todos los colores, que bonito!
    call screen_pintar_pantalla

    ; inicializar el manejador de memoria

    ; inicializar el directorio de paginas

    ; inicializar memoria de tareas

    ; habilitar paginacion

    ; inicializar tarea idle

    ; inicializar todas las tsss

    ; inicializar entradas de la gdt de las tsss

    ; inicializar el scheduler

    ; inicializar la IDT
    lidt [IDT_DESC]
    xor eax, eax
    div eax
    ; configurar controlador de interrupciones

    ; cargar la tarea inicial

    ; saltar a la primer tarea

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
