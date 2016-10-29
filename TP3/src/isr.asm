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
extern fin_intr_pic1_aux

;; SCREEN
extern print_exception_message

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
.loopear:
    ; To Infinity And Beyond!!
    mov eax, 0xFFF2
    mov ebx, 0xFFF2
    mov ecx, 0xFFF2
    mov edx, 0xFFF2
    push %1
    call print_exception_message
    jmp $
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
reloj_numero:           dd 0x00000000
reloj:                  db '|/-\'


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
call fin_intr_pic1_aux   ;llama al pic para avisarle que atendio una interrupcion
pushad                   ;pushea todos los registros
call proximo_reloj
popad                    ;popea todos los registros
iret
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33

_isr33:
call fin_intr_pic1_aux     ;llama al pic para avisarle que atendio una interrupcion
pushad                     ;pushea todos los registros
xor eax, eax
in al, 0x60                ;lee del puerto 60
push eax
call rutina_teclado
add esp, 4
popad                      ;popea todos los registros
; xchg bx, bx
iret

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr80

_isr80:
mov eax,0x42
iret

global _isr102

_isr102:
mov eax,0x42
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
