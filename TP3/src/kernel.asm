; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

;;DEFINES
%define PAGE_DIRECTORY_ADDR 0x27000
%define IDLE_SELECTOR        0x88

;; GDT
extern GDT_DESC

;; IDT
extern IDT_DESC
extern idt_inicializar

;; MMU
extern mmu_inicializar_dir_kernel
extern mmu_inicializar_dir_idle
extern mmu_inicializar_dir_tarea

;; PIC
extern resetear_pic
extern habilitar_pic

;; SCREEN
extern screen_pintar_pantalla
extern print_modo_estado
extern print_modo_mapa
extern inicializar_pantalla

;; TSS
extern tss_inicializar

;; SCHED
extern sched_inicializar

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
    ; call screen_pintar_pantalla
    ; call inicializar_pantalla
    ; call print_modo_estado

    ; inicializar el manejador de memoria
    mov eax, PAGE_DIRECTORY_ADDR
    push eax
    call mmu_inicializar_dir_kernel
    call mmu_inicializar_dir_idle

    ; inicializar el directorio de paginas
    mov eax, PAGE_DIRECTORY_ADDR
    mov cr3, eax

    ; inicializar memoria de tareas


    ; habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; inicializar tarea idle
    ; inicializar todas las tsss
    ; inicializar entradas de la gdt de las tsss
    call tss_inicializar

    ; inicializar el scheduler
    call sched_inicializar
    call inicializar_pantalla
    call print_modo_estado

    ; inicializar la IDT
    call idt_inicializar
    lidt [IDT_DESC]

    ; configurar controlador de interrupciones
    call resetear_pic  ;remapeo del pic
    call habilitar_pic ;prende el pics
    sti

    ; cargar la tarea inicial
    mov ax, 23
    shl ax, 3
    ltr ax

    ; saltar a la primer tarea
    xchg bx, bx
    jmp IDLE_SELECTOR:0

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
