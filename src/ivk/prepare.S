//---
// ivk/_prepare - instruction declare preparation
//---
#include "./asm_utils.h"
#include "./ivk_utils.h"

.text

/* Check response and request structure validity and return an invoke area
**
** prototype:
**  - r4    <struct inst_resp *>    response structure
**  - r5    <struct inst_req *>     request information
** out:
**  - r4    <struct inst_resp *>    response information
**  - r5    <struct inst_req *>     request information
**  - r6    <void **>               invoke area address
**  - r2    [0:14]                  Rm register ID
**  - r1    [0:14]                  Rn register ID
**  - r0    int                     0 if success, negative otherwise
** notes:
**  - check NULL arguments
**  - allocate invoke area if needed
**  - ensure that req->rn_who and req->rm_who are not set to 15
*/
asm_declare_function(__ivk_prepare):

ivk_prepare_check_args:
    asm_check_null(r4, ivk_prepare_return)
    asm_check_null(r5, ivk_prepare_return)
    ivk_req_get_rn_who(r5, r1, ivk_prepare_return)
    ivk_req_get_rm_who(r5, r2, ivk_prepare_return)

ivk_prepare_gen_area:
    mov.l   ivk_area_addr, r3
    mov.l   @r3, r6
    tst     r6, r6
    bf.s    ivk_prepare_return
    mov     #0, r0

ivk_prepare_gen_area_alloc:
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
    tst     r0, r0
    bf      ivk_prepare_gen_area_addr
    bra     ivk_prepare_return
    mov     #-2, r0

ivk_prepare_gen_area_addr:
    mov     #0xa0, r3
    shll8   r3
    shll16  r3
    shll    r0
    shll    r0
    shll    r0
    shlr    r0
    shlr    r0
    shlr    r0
    or      r3, r0
    mov.l   ivk_area_addr, r3
    mov.l   r0, @r3
    mov     r0, r6
    mov     #0, r0

ivk_prepare_return:
    rts
    nop

.balign 4
ivk_area_addr:
    .long ___ivk_area_addr
malloc:
    .long _malloc
