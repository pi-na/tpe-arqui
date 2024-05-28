GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_time
GLOBAL sys_tick
GLOBAL sys_clear
GLOBAL sys_restartCursor
GLOBAL sys_divide
GLOBAL sys_screen
GLOBAL invalidOp
GLOBAL sys_setScreen
GLOBAL sys_date
GLOBAL inforeg
EXTERN my_printf 
GLOBAL sys_printmem
GLOBAL divideByZero
GLOBAL sys_infoReg

%macro pushState 0
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
	push rax
%endmacro

%macro popState 0
	pop rax
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
%endmacro
sys_read:
    push rbp
    mov rbp, rsp
    mov r8, 0
    int 80h
    mov rsp, rbp
    pop rbp
    ret

sys_write:
    enter 0, 0

    mov rax, 0x01
    int 80h
    
    leave
    ret

sys_time:
    push rbp
    mov rbp, rsp
    mov r8, 2
    int 80h
    mov rsp, rbp
    pop rbp
    ret

sys_tick:
    push rbp
    mov rbp, rsp
    mov r8, 3
    int 80h
    mov rsp, rbp
    pop rbp
    ret

sys_clear:
    push rbp
    mov rbp, rsp
    mov r8, 4
    int 80h
    mov rsp, rbp
    pop rbp
    ret

sys_restartCursor:
    push rbp
    mov rbp, rsp
    mov r8, 5
    int 80h
    mov rsp, rbp
    pop rbp
    ret

sys_divide:
  push rbp
  mov rbp, rsp
  mov r8, 6
  int 80h
  mov rsp, rbp
  pop rbp
  ret

sys_screen:
  push rbp
  mov rbp, rsp
  mov r8, 7
  int 80h
  mov rsp, rbp
  pop rbp
  ret

sys_setScreen:
  push rbp
  mov rbp, rsp
  mov r8, 9
  int 80h
  mov rsp, rbp
  pop rbp
  ret

sys_date:
  push rbp
  mov rbp, rsp
  mov r8, 10
  int 80h
  mov rsp, rbp
  pop rbp
  ret

sys_infoReg:
    push rbp
    mov rbp, rsp
    mov r8, 11
    int 80h
    mov rsp, rbp
    pop rbp
    ret

; Retrivied from https://mudongliang.github.io/x86/html/file_module_x86_id_318.html
invalidOp:
    push rbp
    mov rbp, rsp

    UD2

    mov rsp, rbp
    pop rbp
    ret

divideByZero:
    push rbp
    mov rbp, rsp

    mov rdx, 1
    mov rax, 0
    div rax

    mov rsp, rbp
    pop rbp
    ret

sys_printmem:
    push rbp
    mov rbp, rsp
    
    mov r8, 8
    int 80h
    
    mov rsp, rbp
    pop rbp
    ret