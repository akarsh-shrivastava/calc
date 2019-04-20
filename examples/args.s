.data
    a: .quad 0
    b: .quad 0
    c: .quad 0
    d: .quad 0
    e: .quad 0
    f: .quad 0
    g: .quad 0
    h: .quad 0


.text
    .globl args
    args:
        pushq %rbp
        movq %rsp, %rbp
        movq %rdi, a(%rip)
        movq %rsi, b(%rip)
        movq %rdx, c(%rip)
        movq %rcx, d(%rip)
        movq %r8, e(%rip)
        movq %r9, f(%rip)
        movq 16(%rbp), %rax
        movq %rax, g(%rip)
        movq 24(%rbp), %rax
        movq %rax, h(%rip)
        movq g(%rip), %rax
        popq %rbp
        ret


