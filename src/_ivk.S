#include "./asm.h"
#include "./ivk.h"
//---
// _ivk - invoke magic helper
//---
.text

/* Check response and request structure validity and genertae invoke area
** prototype:
**  - r4    <struct inst_resp *>    response structure
**  - r5    <struct inst_request *> request information
** out:
**  - r4    <struct inst_resp *>    response information
**  - r5    <struct inst_request *> request information
**  - r6    <void **>               invoke area address
**  - r2    [0:14]                  Rm register ID
**  - r1    [0:14]                  Rn register ID
**  - r0    int                     0 if success, negative otherwise
** notes:
**  - check NULL arguments
**  - allocate invoke area if needed
**  - ensure req->rn_who/req->rm_who are not set to 15
** todo:
**  - why should I handle r4/r5 as double pointer? */
asm_gen_function(__ivk_prepare):
    asm_check_null(r4, ivk_prepare_return)
    asm_check_null(r5, ivk_prepare_return)
    mov.l   @r5, r5
    mov.l   @r4, r4
    asm_check_null(r4, ivk_prepare_return)
    asm_check_null(r5, ivk_prepare_return)
    ivk_get_rn_who(r5, r1, ivk_prepare_return)
    ivk_get_rm_who(r5, r2, ivk_prepare_return)
    mov.l   ivk_area_addr, r3
    mov.l   @r3, r6
    tst     r6, r6
    bf.s    ivk_prepare_return
    mov     #0, r0
    sts.l   pr, @-r15
    mov.l   r1, @-r15
    mov.l   r2, @-r15
    mov.l   r4, @-r15
    mov.l   r5, @-r15
    mov.l   malloc, r3
    jsr     @r3
    mov     #126, r4
    mov.l   @r15+, r5
    mov.l   @r15+, r4
    mov.l   @r15+, r2
    mov.l   @r15+, r1
    lds.l   @r15+, pr
    mov.l   ivk_area_addr, r3
    mov.l   r0, @r3
    mov     r0, r6
    tst     r6, r6
    bt.s    ivk_prepare_return
    mov     #-2, r0
    mov     #0, r0
ivk_prepare_return:
    rts
    nop
.balign 4
ivk_area_addr:  .long ___ivk_area_addr
malloc:         .long _malloc



//---
// Data area
//---
.data

asm_gen_object(__ivk_area_addr):
    .long   0x00000000
