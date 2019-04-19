segment .text
	extern write_int
	extern eg1

	global _start:
	_start:
		call eg1
		mov rdi, rax
		call write_int

		mov rax, 1
		mov rdi, 1
		mov rsi, NEWLINE
		mov rdx, 1
		syscall

		mov rax, 60
		mov rdi, 0
		syscall

	NEWLINE db 0xa