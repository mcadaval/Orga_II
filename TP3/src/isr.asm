; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

;;DEFINES
%define IDLE_SELECTOR                 0x00000088

;; ISR
extern rutina_teclado

;; PIC
extern fin_intr_pic1

;; SCREEN
extern flamear_bandera
extern actualizar_reloj_actual
extern matar_en_screen
extern guardar_estado_registros
extern excepcion_bandera
extern imprimir_paginas
extern excepcion_tarea

;; GAME
extern game_service

;; SCHED
extern sched_proximo_indice
extern dame_tarea_actual
extern matar_tarea
extern actualizar_flag_idle

;;TSS
extern resetear_bandera_tss


;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
.loopear:
    call matar_tarea             ;mata la tarea
    push %1                      ;pushea todos los registros para guardar la info sobre la ultima excepcion
    push gs 
    push fs
    push es
    push ds
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax

    mov eax, cr4 
    push eax
    mov eax, cr3
    push eax
    mov eax, cr2
    push eax
    mov eax, cr0
    push eax 
    call guardar_estado_registros
    call matar_en_screen         
    jmp IDLE_SELECTOR:0                   ;cambia a tarea idle
    jmp $
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
reloj_numero:           dd 0x00000000
reloj:                  db '|/-\'

offset: dd 0x0
selector: dw 0x0

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32

_isr32:
pushad                       ;pushea todos los registros
call fin_intr_pic1           ;llama al pic para avisarle que atendio una interrupcion
call resetear_bandera_tss    ;si venimos de ejecutar una bandera reseteamos el eip de la tss de la misma
call actualizar_reloj_actual ;actualiza el reloj de la tarea actual
call dame_tarea_actual
mov bx, ax                   ;guarda en bx el id de la tarea actual
call proximo_reloj           ;llama al proximo reloj
call sched_proximo_indice    ;obtiene el proximo indice
cmp ax, bx                   ;verifica si el proximo indice es igual al indice que se ejecuta ahora 
je .end                      ;si es igual, entonces no hay cambio de contexto
    shl ax, 3
    mov [selector], ax
    jmp far [offset]

.end:
    popad                    ;popea todos los registros
    iret
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33

_isr33:
pushad                     ;pushea todos los registros
call fin_intr_pic1         ;llama al pic para avisarle que atendio una interrupcion
xor eax, eax
in al, 0x60                ;lee del puerto 60
push eax
call rutina_teclado
add esp, 4
popad                      ;popea todos los registros
iret

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr80

_isr80:
pushad
mov esi, eax              ;guarda los parametros de entrada de la syscall en registros que se preservan
mov edi, ecx 
str ax
shr ax, 3                 ;verifica si es tarea o bandera
cmp ax, 9
jl .es_tarea              ;si es bandera muere porque no puede llamar esta syscall
call matar_tarea
call excepcion_tarea
call matar_en_screen
.es_tarea:                 ;si es tarea se ejecuta el servicio solicitado
mov eax, cr3
push eax                   ;pushea parametros a la pila
push edi                   
push ebx
push esi
call game_service          ;llama a game_service
call imprimir_paginas      ;imprime paginas nuevas mapeadas 
call actualizar_flag_idle  ;actualiza el flag que indica que la tarea idle esta corriendo
jmp IDLE_SELECTOR:0        ;cambia a tarea idle 
add esp, 16 
popad                      ;popea todos los registros
iret

global _isr102

_isr102:
pushad
str ax
shr ax, 3
cmp ax, 9
jge .es_bandera            ;si es una tarea muere porque no puede llamar esta syscall
call matar_tarea
call excepcion_bandera
call matar_en_screen
jmp .saltar

.es_bandera:
call flamear_bandera       ;si es bandera llamamos a la funcion para flamear
.saltar:
call actualizar_flag_idle  ;actualiza el flag que indica que la tarea idle esta corriendo
jmp IDLE_SELECTOR:0                 ;cambia a tarea idle
popad
iret

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
    pushad

    inc DWORD [reloj_numero]
    mov ebx, [reloj_numero]
    cmp ebx, 0x4
    jl .ok
        mov DWORD [reloj_numero], 0x0
        mov ebx, 0
    .ok:
        add ebx, reloj
        imprimir_texto_mp ebx, 1, 0x0f, 24, 79

    popad
    ret
