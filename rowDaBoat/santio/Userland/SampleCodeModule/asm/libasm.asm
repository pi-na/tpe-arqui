
GLOBAL _syscall

GLOBAL _opcodeExp
GLOBAL _div_zero
GLOBAL clock

section .text

%macro pushCalleeState 0
	push rbx
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popCalleeState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
%endmacro


_syscall:
    push rbp
    mov rbp, rsp
	pushCalleeState

    int 80h

	popCalleeState
	mov rsp, rbp
    pop rbp

    ret
_div_zero:
    mov rax, 0
    div rax
    ret
_opcodeExp:
    UD2
	ret

