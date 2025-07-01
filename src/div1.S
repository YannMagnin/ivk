#include "./asm.h"
#include "./ivk.h"
.text

//---
// Public
//---

// __div1_magic_invoke(struct inst_resp *resp, struct inst_req *req)
asm_gen_function(__div1_magic_invoke):
    sts.l   pr, @-r15
_div1_setup:
    ivk_call_prepare(_div1_exit)
_div1_gen_stub:
    tst     r2, r2
    bt.s    _div1_special_stub
    ivk_push_inst_inmi(r6, 0b0011,     r1,     r2, 0b0100)   ! div1 rn,rm
_div1_default_stub:
    ivk_push_inst_inii(r6, 0b0110,     r1, 0b0000, 0b0011)   ! mov  rn,r0
    ivk_push_inst_iiii(r6, 0b0000, 0b0000, 0b0000, 0b1011)   ! rts
    ivk_push_inst_inii(r6, 0b0110,     r2, 0b0001, 0b0011)   ! mov  rm,r1
    bra     _div1_ivk_call
    nop
_div1_special_stub:
    ivk_push_inst_inii(r6, 0b0110,     r1, 0b0010, 0b0011)   ! mov  rn,r2
    ivk_push_inst_inii(r6, 0b0110,     r2, 0b0001, 0b0011)   ! mov  rm,r1
    ivk_push_inst_iiii(r6, 0b0000, 0b0000, 0b0000, 0b1011)   ! rts
    ivk_push_inst_iiii(r6, 0b0110, 0b0010, 0b0000, 0b0011)   ! mov  r2,r0
_div1_ivk_call:
    ivk_call_exec()
_div1_exit:
    lds.l   @r15+, pr
    rts
    nop
