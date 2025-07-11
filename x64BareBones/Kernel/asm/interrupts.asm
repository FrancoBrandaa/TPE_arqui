
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL saveRegisters
GLOBAL getRegisters
GLOBAL setEscFlag

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL _int80Handler

EXTERN getStackBase  
EXTERN showRegisters  
EXTERN irqDispatcher
EXTERN syscallDispatcher
EXTERN exceptionDispatcher

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState
    mov byte [esc_flag], 0

	mov rdi, %1 
	call irqDispatcher


	mov al, 20h
	out 20h, al

	popState
    cmp byte [esc_flag], 1
    jne .donot_save_registers
    catchRegisters
    .donot_save_registers:
    iretq
%endmacro

%macro catchRegisters 0
    mov qword [regs_backup], rax
    mov rax, regs_backup
    add rax, 8
    mov qword [rax], rbx
    add rax, 8
    mov qword [rax], rcx
    add rax, 8
    mov qword [rax], rdx
    add rax, 8
    mov qword [rax], rdi
    add rax, 8
    mov qword [rax], rsi
    add rax, 8
    ;catcheo de rsp anterior (tomanba el actual, error!)
    ;mov qword [rax], rsp
    mov qword rdi, [rsp+24]    ; rsp
    mov qword [rax], rdi
    add rax, 8
    mov qword [rax], rbp
    add rax, 8
    mov qword [rax], r8
    add rax, 8
    mov qword [rax], r9
    add rax, 8
    mov qword [rax], r10
    add rax, 8
    mov qword [rax], r11
    add rax, 8
    mov qword [rax], r12
    add rax, 8
    mov qword [rax], r13
    add rax, 8
    mov qword [rax], r14
    add rax, 8
    mov qword [rax], r15
    add rax, 8
    mov qword rdi, [rsp]      ; rip
    mov qword [rax], rdi
    add rax, 8
    mov qword rdi, [rsp+8]    ; cs
    mov qword [rax], rdi
    add rax, 8
    mov qword rdi, [rsp+16]   ; rflags
    mov qword [rax], rdi
    mov byte [registers_saved], 1    
%endmacro

%macro exceptionHandler 1
    catchRegisters

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

    call getStackBase
    mov [rsp+8*3], rax
    mov rax, userland
    mov [rsp], rax
    iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1



;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;invalid operation Exception
_exception6Handler:
	exceptionHandler 6

_int80Handler:
	
	mov r9, r8
	mov r8, r10
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi 
	mov rdi, rax
	
	call syscallDispatcher

	iretq

haltcpu:
	cli
	hlt
	ret

saveRegisters:
    catchRegisters
    ret

getRegisters:
    mov rax, 0   
    cmp byte [registers_saved], 1
    jne .not_saved
    mov rax, regs_backup
.not_saved:
    ret

setEscFlag:
    mov byte [esc_flag], 1
    ret

section .rodata
    userland equ 0xa00000
    registers_saved db 0

section .bss
    esc_flag resb 1
    regs_backup resq 19
