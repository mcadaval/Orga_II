; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

;; ISR
extern rutina_teclado

;; PIC
extern fin_intr_pic1

;; SCREEN
extern print_exception_message
extern flamear_bandera
extern actualizar_reloj_actual
extern print_modo_estado
extern matar_en_screen
extern matar_bandera

;; GAME
extern game_service

;; SCHED
extern sched_proximo_indice
extern dame_tarea_actual
extern matar_tarea
extern actualizar_flag_idle

extern contador_tareas
extern tarea
extern tarea_idle
extern num_tareas_vivas;


;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
.loopear:
    push %1
    xchg bx, bx
    ; push gs 
    ; push fs
    ; push es
    ; push ds
    ; push ebp
    ; push edi
    ; push esi
    ; push edx
    ; push ecx
    ; push ebx
    ; push eax

    ; mov eax, cr4 
    ; push eax
    ; push eax, cr3
    ; push eax
    ; push eax, cr2
    ; push eax
    ; push eax, cr0
    ; push eax 
    ; call guardar_estado_registros

    ; call print_exception_message ;imprime mensaje de excepcion
    call actualizar_reloj_actual ;actualiza el reloj de la tarea actual    
    call matar_tarea             ;mata la tarea
    call matar_en_screen
    ; call print_modo_estado
    xchg bx, bx
    jmp 0x88:0                   ;cambia a tarea idle
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
xchg bx, bx
pushad                       ;pushea todos los registros
call fin_intr_pic1           ;llama al pic para avisarle que atendio una interrupcion
call actualizar_reloj_actual ;actualiza el reloj de la tarea actual
call dame_tarea_actual
mov bx, ax                   ;guarda en bx el id de la tarea actual
call proximo_reloj           ;llama al proximo reloj
call sched_proximo_indice    ;obtiene el proximo indice

xor edx, edx
mov dl, [tarea_idle]        ;edx
xor ecx, ecx
mov cl, [tarea]             ;ecx
xor esi, esi
mov si, [contador_tareas]   ;esi
xor edi, edi
mov di, [num_tareas_vivas]  ;edi

cmp ax, bx                   ;verifica si el proximo indice es igual al indice que se ejecuta ahora 
je .end                      ;si es igual, entonces no hay cambio de contexto
    shl ax, 3
    mov [selector], ax
xchg bx, bx
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
xchg bx, bx
iret

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr80

_isr80:
pushad
xchg bx, bx
call actualizar_reloj_actual ;actualiza el reloj de la tarea actual
mov edi, cr3
push edi                   ;pushea parametros a la pila
push ecx                   
push ebx
push eax
call game_service          ;llama a game_service
call actualizar_flag_idle  ;actualiza el flag que indica que la tarea idle esta corriendo
xchg bx, bx
jmp 0x88:0                 ;cambia a tarea idle 
add esp, 16 
popad                      ;popea todos los registros
iret

global _isr102

_isr102:
xchg bx, bx
pushad
call actualizar_reloj_actual ;actualiza el reloj de la tarea actual
call actualizar_flag_idle  ;actualiza el flag que indica que la tarea idle esta corriendo
str ax
shr ax, 3
cmp ax, 9
jge .es_bandera
call matar_tarea
call matar_en_screen

.es_bandera:
; xor eax, eax
; call dame_tarea_actual
; push eax
; call flamear_bandera
xchg bx, bx
jmp 0x88:0                 ;cambia a tarea idle
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
