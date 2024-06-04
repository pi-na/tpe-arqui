
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq60Handler

GLOBAL _exception00Handler
GLOBAL _exception06Handler

GLOBAL zero_division_exception
EXTERN irqDispatcher
EXTERN exceptionDispatcher

EXTERN getStackBase
EXTERN printf
SECTION .text


	
%macro copyRegs 0
	mov [regs], r15
	mov [regs+8],  r14
	mov [regs+8*2], r13
	mov [regs+8*3], r12
	mov [regs+8*4], r11
	mov [regs+8*5], r10
	mov [regs+8*6], r9
	mov [regs+8*7], r8
	mov [regs+8*8], rsi 
	mov [regs+8*9], rdi 
	mov [regs+8*10], rbp 
	mov [regs+8*11], rdx 
	mov [regs+8*12], rcx
	mov [regs+8*13], rbx 
	mov [regs+8*14], rax 
	mov [regs+8*15], rsp 
	mov rax, [rsp] ;rip
	mov [regs+8*16], rax 
	
%endmacro
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
	copyRegs
	mov rax, [rsp+8*15]
	mov[regs+8*16],rax ;saving rip
	mov rdi, %1 ; pasaje de parametro
	mov rsi, regs
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi, regs
	call exceptionDispatcher
	popState
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

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;int80h
_irq60Handler:
	push rbx
	push r12
	push r13
	push r14
	push r15
	push rbp

	mov rbp, rsp
	mov r9, r8
	mov r8, rcx
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 60h
	call irqDispatcher

    mov rsp, rbp
	pop rbp
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	iretq

;Zero Division Exception
_exception00Handler:

	copyRegs
	call getStackBase
	mov [rsp+24],rax 
	mov rax, userland
	mov [rsp], rax 
	exceptionHandler 0
	iretq

_exception06Handler:
	copyRegs

	call getStackBase
	mov [rsp+24],rax 
	mov rax, userland
	mov [rsp], rax 
	;push regs
	exceptionHandler 6
	iretq
haltcpu:
	cli
	hlt
	ret


section .rodata
	userland equ 0x400000

section .bss 
	regs resq 17
