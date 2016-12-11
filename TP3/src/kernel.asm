; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

;;DEFINES
%define PAGE_DIRECTORY_ADDR           0x00027000
%define KERNEL_STACK_BOT              0x00027000
%define IDLE_SELECTOR                 0x00000088
%define KERNEL_CODE_SELECTOR          0x00000090
%define KERNEL_DATA_SELECTOR          0x000000A0
%define SCREEN_SELECTOR               0x000000B0
%define INIT_TASK_SELECTOR            0x000000B8

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
    jmp KERNEL_CODE_SELECTOR:modoprotegido  

BITS 32
    modoprotegido:    
    ; acomodar los segmentos
    xor eax, eax
    mov ax, KERNEL_DATA_SELECTOR
    mov ds, ax          ; index = 20, gdt/ldt = 0, rpl = 00 
    mov ss, ax
    mov es, ax
    mov gs, ax

    mov ax, SCREEN_SELECTOR
    mov fs, ax          ; index = 22, gdt/ldt = 0, rpl = 00

    ; setear la pila
    mov ebp, KERNEL_STACK_BOT
    mov esp, KERNEL_STACK_BOT

    ; inicializar el manejador de memoria
    mov eax, PAGE_DIRECTORY_ADDR
    push eax
    call mmu_inicializar_dir_kernel
    call mmu_inicializar_dir_idle

    ; inicializar el directorio de paginas
    mov eax, PAGE_DIRECTORY_ADDR
    mov cr3, eax

    ; habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; inicializar tarea idle
    ; inicializar todas las tsss
    ; inicializar entradas de la gdt de las tsss
    ; inicializar memoria de tareas
    call tss_inicializar

    ; inicializar el scheduler
    call sched_inicializar
    ; pintar pantalla, todos los colores, que bonito!
    call inicializar_pantalla

    ; inicializar la IDT
    call idt_inicializar
    lidt [IDT_DESC]

    ; configurar controlador de interrupciones
    call resetear_pic  ;remapeo del pic
    call habilitar_pic ;prende el pics
    sti

    ; cargar la tarea inicial
    mov ax, INIT_TASK_SELECTOR
    ltr ax

    ; saltar a la primer tarea
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
