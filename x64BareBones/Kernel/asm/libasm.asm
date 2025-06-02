GLOBAL cpuVendor
GLOBAL outb
GLOBAL inb

section .text
	

;funciones para leer/ escribir en el mapa de entradas y salidas.
;lee un byte del puerto de entrada
inb:
    mov  rdx, rdi
    in   al, dx
    movzx rax, al     ; Extiende AL a 64 bits en RAX (limpia altos)
    ret

;escribe un byte en el puerto de salida
outb:
    mov rdx,rdi
    mov rax,rsi
    out dx,al
    ret

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret
