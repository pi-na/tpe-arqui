GLOBAL cpuVendor
GLOBAL keyRead

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

keyRead:
	push rbp
	mov rbp, rsp

	xor rax, rax
.loop:
	in al, 0x64      ; Read 8042 status register. Can be read at any time.
	and al, 0x01     ; Output register 60h should only be read IIF Bit 0 of status port is set to 1.
	cmp al, 0
	je .loop
	in al, 0x60      

	mov rsp, rbp
	pop rbp
	ret