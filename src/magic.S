.text
.global ___inst_magic_invoke

___inst_magic_invoke:
    add #(9*4), r5
    mov #0,r0
    mov.l r0, @-r5
    mov.l r0, @-r5
    mov.l r0, @-r5
    mov.l r0, @-r5
    mov.l r0, @-r5
    mov.l r0, @-r5
    mov.l r0, @-r5
    mov.l r0, @-r5
    mov.l r0, @-r5
    rts
    nop
