segment .data
    a dq 0
    b dq 0
    c dq 0
    d dq 0


segment .text
    global eg1
    eg1:
        push qword rbp
        mov rbp, rsp
        extern write_int
        mov rax, 80
        pop qword rbp
        ret


