//---
// ivk/invalid.S - execute invalid instruction
//---
#include "./ivk_utils.h"
.text

/* Execute invalid instruction
** in:
**  - r4    void **     area double pointer
**  - r5    <opcode>    opcode to execute
** notes:
**  - allocate an invoke area
**  - generate the code
**  - execute the code
**  - free the invoke area
*/
asm_declare_function(__ivk_exec_opcode):
    sts.l   pr, @-r15
    mov.l   r8, @-r15
    mov.l   r9, @-r15
    mov     r4, r8
    mov     r5, r9

/* in:
**  - r8    area double pointer (P2) || NULL
**  - r9    opcode
** out:
**  - r8    area double pointer (P2)
**  - r9    opcode
**  - r4    area pointer (P2)
*/
ivk_alloc_area:
    mov.l   @r8, r4
    tst     r4, r4
    bf      ivk_gen_code
    mov.l   malloc, r3
    jsr     @r3
    mov     #126, r4
    mov     r0, r4
    tst     r0, r0
    bt.s    ivk_exit
    mov     #-1, r0
    mov     #0xa0, r3
    shll8   r3
    shll16  r3
    shll    r4
    shll    r4
    shll    r4
    shlr    r4
    shlr    r4
    shlr    r4
    or      r3, r4
    mov.l   r4, @r8

/*
** in:
**  - r8    area double pointer (P2)
**  - r9    opcode
**  - r4    area pointer (P2)
*/
ivk_gen_code:
    mov r4, r5
    ivk_push_rawi(r4, r9)                                   ! <target>
    ivk_push_inst(r4, iiii, 0b0000, 0b0000, 0b0000, 0b1011) ! rts
    ivk_push_inst(r4, iiii, 0b0000, 0b0000, 0b0000, 0b1001) ! nop

ivk_exec_code:
    jsr     @r5
    nop

ivk_exit:
    mov.l   @r15+, r9
    mov.l   @r15+, r8
    lds.l   @r15+, pr
    rts
    nop

.balign 4
malloc:
    .long _malloc
