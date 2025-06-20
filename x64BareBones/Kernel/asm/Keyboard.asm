
global returnKey
extern setEscFlag
section .text 
returnKey:
    xor    eax, eax    ; limpia EAX completamente , asi retorno bien en rax todo
    in al, 0x60
    cmp al, 0x01 ; compara con el codigo de retorno del escape
    jne notEscape ; si no es escape, salta a notEscape
    call setEscFlag 
    notEscape:
    ret





