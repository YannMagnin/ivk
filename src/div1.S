#include "./asm.h"
.text

//---
// Public
//---

// __div1_magic_invoke(struct inst_resp *resp, struct inst_req *req)
asm_gen_function(__div1_magic_invoke):
_ivk_entry:
    sts.l   pr, @-r15
    mov.l   r10, @-r15
    mov.l   r9, @-r15
    mov.l   r8, @-r15

_ivk_prepare:
    asm_check_null(r4, _ivk_return)
    asm_check_null(r5, _ivk_return)
    mov.l   @r5, r5
    mov.l   @r4, r4
    ivk_get_area(r6, _ivk_return)
    mov     r6, r10

_ivk_gen_stub:
    ivk_get_rn_who(r5, r1, _ivk_return)
    ivk_get_rm_who(r5, r2, _ivk_return)
    tst     r2, r2
    bt      __special_stub
__default_stub:
    ivk_push_inst_inmi(r6, 0b0011,     r1,     r2, 0b0100)   ! div1 rn,rm
    ivk_push_inst_inii(r6, 0b0110,     r1, 0b0000, 0b0011)   ! mov  rn,r0
    ivk_push_inst_iiii(r6, 0b0000, 0b0000, 0b0000, 0b1011)   ! rts
    ivk_push_inst_inii(r6, 0b0110,     r2, 0b0001, 0b0011)   ! mov  rm,r1
    bra     _ivk_call
    nop
__special_stub:
    ivk_push_inst_inmi(r6, 0b0011,     r1,     r2, 0b0100)   ! div1 rn,rm
    ivk_push_inst_inii(r6, 0b0110,     r1, 0b0010, 0b0011)   ! mov  rn,r2
    ivk_push_inst_inii(r6, 0b0110,     r2, 0b0001, 0b0011)   ! mov  rm,r1
    ivk_push_inst_iiii(r6, 0b0000, 0b0000, 0b0000, 0b1011)   ! rts
    ivk_push_inst_iiii(r6, 0b0110, 0b0010, 0b0000, 0b0011)   ! mov  r2,r0

_ivk_call:
    // todo: context save
    // todo: switch bank register
    // todo: invalitate the cache
    mov     r10, r0

_ivk_return:
    mov.l   @r15+, r8
    mov.l   @r15+, r9
    mov.l   @r15+, r10
    lds.l   @r15+, pr
    rts
    nop
    // TODO: get stub_area
    // TODO: check validity
    // construct "on-the-fly" instructions
    //
    // if rm is not r1:
    //   - div1   rn, rm
    //   - mov    rn, r0
    //   - rts
    //   - mov    rm, r1
    // else:
    //   - div1   rn, rm
    //   - mov    rn, r2
    //   - mov    rm, r1
    //   - rts
    //   - mov    r2, r0
    //
