
global returnKey

section .text 
returnKey:
    xor    eax, eax    ; limpia EAX completamente , asi retorno bien en rax todo
    in al, 0x60
    ret