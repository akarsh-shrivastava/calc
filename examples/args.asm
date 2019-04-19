section .data
    a dq 0
    b dq 0
    c dq 0
    d dq 0
    e dq 0
    f dq 0
    g dq 0
    h dq 0


section .text
    global args
    args:
        push qword rbp
        mov rbp, rsp
        mov [a], rdi
        mov [b], rsi
        mov [c], rdx
        mov [d], rcx
        mov [e], r8
        mov [f], r9
        mov rax, [rbp+16]
        mov [g], rax
        mov rax, [rbp+24]
        mov [h], rax
        mov rax, [a]
        pop qword rbp
        ret


