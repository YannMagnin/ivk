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
**  - ensure req->rn_who/req->rm_who are not set to 15 */
asm_gen_function(__ivk_prepare):
    asm_check_null(r4, ivk_prepare_return)
    asm_check_null(r5, ivk_prepare_return)
    ivk_req_get_rn_who(r5, r1, ivk_prepare_return)
    ivk_req_get_rm_who(r5, r2, ivk_prepare_return)
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


/* prototype:
**  - r4    <sturct inst_resp>      response info
**  - r5    <struct inst_req>       request info
*/
asm_gen_function(__ivk_exec_area):
    sts.l   pr, @-r15
    stc.l   sr, @-r15

ivk_exec_gen_sr:
    mov #0, r1
    ivk_req_get_M_mask(r5, r1)
    ivk_req_get_Q_mask(r5, r1)
    ivk_req_get_S_mask(r5, r1)
    ivk_req_get_T_mask(r5, r1)
    mov.l   1f, r0
    or      r0, r1
    ldc     r1, sr
    bra     ivk_exec_invalidate_cache
    nop
.balign 4
1:  .long   0x400000f0

ivk_exec_invalidate_cache:
    // FIXME: invalidate the cache to avoid ivk_area to be corrupted
    // FIXME: ...but we need to be in P2 or IL to execute the following code
    // FIXME: ...otherwise the device will hard-crash without notice !
    //mov.l   1f, r1
    //mov.l   2f, r0
    //mov.l   r0, @r1
    //synco
    //mov.l   @r1, r1
    //or      #0x08, r0
    //mov.l   r0, @r1
    //synco
    bra     ivk_exec
    nop
.balign 4
1:  .long   0xff00001c
2:  .long   0x0000010f

ivk_exec:
    stc     sr, r2
    ivk_resp_set_sr_before(r4, r2)
    ivk_req_get_rn_data(r5, r0)
    ivk_req_get_rm_data(r5, r1)
    mov.l   r4, @-r15
    mov.l   r5, @-r15
    mov.l   1f, r3
    mov.l   @r3, r3
    jsr     @r3
    nop
    mov.l   @r15+, r5
    bra     ivk_exec_fetch_cpu_ctx
    mov.l   @r15+, r4
.balign 4
1:
    .long   ___ivk_area_addr

ivk_exec_fetch_cpu_ctx:
    stc     sr, r2
    ivk_resp_set_sr_after(r4, r2)
    ivk_resp_set_rn_data(r4, r0)
    ivk_resp_set_rm_data(r4, r1)
    ivk_resp_set_M(r4, r2)
    ivk_resp_set_Q(r4, r2)
    ivk_resp_set_S(r4, r2)
    ivk_resp_set_T(r4, r2)
    ivk_resp_set_MACL(r4)
    ivk_resp_set_MACH(r4)
    mov     #0, r0

ivk_exec_exit:
    ldc.l   @r15+, sr
    lds.l   @r15+, pr
    rts
    nop


//---
// Data area
//---
.data

asm_gen_object(__ivk_area_addr):
    .long   0x00000000
