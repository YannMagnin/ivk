//---
// ivk/_exec    - execute generated code area
//---
#include "./ivk_utils.h"
.text

/* exectute the provided instruction and fill the response information
**
** prototype:
**  - r4    <sturct inst_resp>      response info
**  - r5    <struct inst_req>       request info
**  - r6    <void **>               invoke area address
**  - r7    <u16>                   target instruction to exec
**  - r1    [0:14]                  Rn ID
**  - r2    [0:14]                  Rm ID
*/
asm_declare_function(__ivk_exec_area):
    mov.l   r12, @-r15
    mov.l   r11, @-r15
    mov.l   r10, @-r15
    mov.l   r9, @-r15
    mov.l   r8, @-r15
    sts.l   pr, @-r15
    stc.l   sr, @-r15

    // todo: better calling convention
ivk_exec_arg_save:
    mov     r1, r8
    mov     r2, r9
    mov     r4, r10
    mov     r5, r11
    mov     r6, r12

ivk_exec_gen_code:
    mov     r6, r4
    mov     r7, r5
    mov     r1, r6
    mov.l   1f, r0
    jsr     @r0
    mov     r2, r7
    bra     ivk_exec_gen_sr
    nop
.balign 4
1:
    .long   ___ivk_code_gen

ivk_exec_gen_sr:
    mov #0, r1
    ivk_req_get_M_mask(r11, r1)
    ivk_req_get_Q_mask(r11, r1)
    ivk_req_get_S_mask(r11, r1)
    ivk_req_get_T_mask(r11, r1)
    mov.l   1f, r0
    or      r0, r1
    ldc     r1, sr
    bra     ivk_exec
    nop
.balign 4
1:  .long   0x400000f0

ivk_exec:
    stc     sr, r2
    ivk_resp_set_sr_before(r10, r2)
    ivk_req_get_rn_data(r11, r0)
    ivk_req_get_rm_data(r11, r1)
    jsr     @r12
    nop

ivk_exec_fetch_cpu_ctx:
    stc     sr, r2
    ivk_resp_set_sr_after(r10, r2)
    ivk_resp_set_rn_data(r10, r0)
    ivk_resp_set_rm_data(r10, r1)
    ivk_resp_set_M(r10, r2)
    ivk_resp_set_Q(r10, r2)
    ivk_resp_set_S(r10, r2)
    ivk_resp_set_T(r10, r2)
    ivk_resp_set_MACL(r10)
    ivk_resp_set_MACH(r10)
    mov     #0, r0

ivk_exec_exit:
    ldc.l   @r15+, sr
    lds.l   @r15+, pr
    mov.l   @r15+, r8
    mov.l   @r15+, r9
    mov.l   @r15+, r10
    mov.l   @r15+, r11
    rts
    mov.l   @r15+, r12
