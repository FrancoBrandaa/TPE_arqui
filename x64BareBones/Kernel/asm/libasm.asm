
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


