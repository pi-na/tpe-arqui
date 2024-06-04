GLOBAL cpuVendor
GLOBAL get_seconds
GLOBAL get_hours
GLOBAL get_minutes

GLOBAL getKey
GLOBAL outSpeaker
GLOBAL inSpeaker
section .text
	
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

%macro enter_func 0
	push rbp
	mov rbp, rsp
%endmacro

%macro leave_func 0
	mov rsp, rbp
	pop rbp
	ret
%endmacro

%macro get_rtc_value 1
	mov al, %1
	out 70h, al
	in al, 71h
	mov rdi, rax
%endmacro

get_seconds:
	enter_func
	get_rtc_value 00
	leave_func

get_minutes:
	enter_func
	get_rtc_value 02
	leave_func

get_hours:
	enter_func
	get_rtc_value 04
	leave_func

getKey:
	enter_func
	xor rax, rax
	.loop:
		in al, 64h
		and al, 1h
		cmp al, 0
		je .loop
		jmp .fin
	.fin:
		in al, 60h
	leave_func


inSpeaker:
	push rbp
	mov rbp, rsp

	mov rdx, rdi
	in al, dx

	mov rsp, rbp
	pop rbp
	ret

outSpeaker:
	push rbp
	mov rbp, rsp 

	mov rax, rsi 
	mov rdx, rdi 
	out dx, al 

	mov rsp, rbp
	pop rbp
	ret