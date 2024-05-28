GLOBAL sys_write

sys_write:
    push rbp
    mov rbp, rsp
    mov r8, 1
    int 80h
    mov rsp, rbp
    pop rbp
    ret
