#ifndef IVK_UTILS_H
#define IVK_UTILS_H 1

#include "./asm_utils.h"

//---
// ivk_declare_*()  - ivk declaration
//---

/* internal register used to store particular data */
#define ivk_rn  r1
#define ivk_rm  r2

/* declare a new instruction
 * notes:
 *  - assume r4 contain <struct ivk_inst_resp*>
 *  - assume r5 contain <struct ivk_inst_req*> */
#define ivk_declare_instruction(name, type, x3, x2, x1, x0) \
    asm_declare_function(__##name##_ivk_exec):      ;\
        sts.l   pr, @-r15                           ;\
        ivk_call_prepare(name##_ivk_exec_exit)      ;\
        ivk_gen_inst_##type(r7, x3, x2, x1, x0)     ;\
        ivk_call_exec()                             ;\
    name##_ivk_exec_exit:                           ;\
        lds.l   @r15+, pr                           ;\
        rts                                         ;\
        nop

//---
// ivk_call_*() - call helper
//---

/* call ivk_prepare and check return */
#define ivk_call_prepare(label_exit) \
    mov.l   1f, r0              ;\
    jsr     @r0                 ;\
    nop                         ;\
    bra     2f                  ;\
    nop                         ;\
.balign 4                       ;\
1:                              ;\
    .long   ___ivk_prepare      ;\
2:

/* call ikv_exec() */
#define ivk_call_exec() \
    mov.l   1f, r0              ;\
    jsr     @r0                 ;\
    nop                         ;\
    bra     2f                  ;\
    nop                         ;\
.balign 4                       ;\
1:                              ;\
    .long   ___ivk_exec_area    ;\
2:

//---
// ivk_get_*() - invokation macros
//---

/* get info->rn_data */
#define ivk_req_get_rn_data(info, out) \
    mov.l   @(0, info), out

/* get info->rm_data */
#define ivk_req_get_rm_data(info, out) \
    mov.l   @(4, info), out

/* get info->rn_who */
#define ivk_req_get_rn_who(info, out, error) \
    mov.l   @(8, info), out     ;\
    mov     #15, r0             ;\
    cmp/eq  r0, out             ;\
    bt.s    error               ;\
    mov     #-3, r0

/* get info->rm_who */
#define ivk_req_get_rm_who(info, out, error) \
    mov.l   @(12, info), out    ;\
    mov     #15, r0             ;\
    cmp/eq  r0, out             ;\
    bt      error               ;\
    mov     #-3, r0

/* get and generate SR.M mask */
#define ivk_req_get_M_mask(info, out) \
    mov.l   @(16, info), r0     ;\
    cmp/pl  r0                  ;\
    movt    r0                  ;\
    shll8   r0                  ;\
    shll    r0                  ;\
    or      r0, out

/* get and generate SR.Q mask */
#define ivk_req_get_Q_mask(info, out) \
    mov.l   @(20, info), r0     ;\
    cmp/pl  r0                  ;\
    movt    r0                  ;\
    shll8   r0                  ;\
    or      r0, out

/* get and generate SR.S mask */
#define ivk_req_get_S_mask(info, out) \
    mov.l   @(24, info), r0     ;\
    cmp/pl  r0                  ;\
    movt    r0                  ;\
    shll    r0                  ;\
    or      r0, out

/* get and generate SR.T mask */
#define ivk_req_get_T_mask(info, out) \
    mov.l   @(28, info), r0     ;\
    cmp/pl  r0                  ;\
    movt    r0                  ;\
    or      r0, out

//---
// ivk_resp_set_*() - response setter
//---

/* save Rn data */
#define ivk_resp_set_rn_data(resp, data) \
    mov.l   data, @(0, resp)

/* save Rm data */
#define ivk_resp_set_rm_data(resp, data) \
    mov.l   data, @(4, resp)

/* save SR.M status bit */
#define ivk_resp_set_M(resp, sr) \
    mov     __h(hash)2, r3      ;\
    shll8   r3                  ;\
    and     sr, r3              ;\
    cmp/pl  r3                  ;\
    movt    r3                  ;\
    mov.l   r3, @(8, resp)

/* save SR.Q status bit */
#define ivk_resp_set_Q(resp, sr) \
    mov     __h(hash)1, r3      ;\
    shll8   r3                  ;\
    and     sr, r3              ;\
    cmp/pl  r3                  ;\
    movt    r3                  ;\
    mov.l   r3, @(12, resp)

/* save SR.S status bit */
#define ivk_resp_set_S(resp, sr) \
    mov     __h(hash)2, r3      ;\
    and     sr, r3              ;\
    cmp/pl  r3                  ;\
    movt    r3                  ;\
    mov.l   r3, @(16, resp)

/* save SR.T status bit */
#define ivk_resp_set_T(resp, sr) \
    mov     __h(hash)1, r3      ;\
    and     sr, r3              ;\
    cmp/pl  r3                  ;\
    movt    r3                  ;\
    mov.l   r3, @(20, resp)

/* save SR register before invocation */
#define ivk_resp_set_sr_before(resp, sr) \
    mov.l   sr, @(24, resp)

/* save SR register after invocation */
#define ivk_resp_set_sr_after(resp, sr) \
    mov.l   sr, @(28, resp)

/* save MACL register */
#define ivk_resp_set_MACL(resp)  \
    sts     macl, r3            ;\
    mov.l   r3, @(32, resp)

/* save MACH register */
#define ivk_resp_set_MACH(resp)  \
    sts     mach, r3            ;\
    mov.l   r3, @(36, resp)

//---
// ivk_gen_inst_*() - generate instruction
//
// note:
//  - r3 is reserved
//  - r0 is reserved
//---

/* generate a iiii.nnnn.mmmm.iiii instruction */
#define ivk_gen_inst_inmi(out, i3, n2, m1, i0)  \
    mov     __h(hash)4, r3      ;\
    mov     __h(hash)i3,r0      ;\
    shld    r3,r0               ;\
    or      n2,r0               ;\
    shld    r3,r0               ;\
    or      m1,r0               ;\
    shld    r3,r0               ;\
    or      __h(hash)i0,r0      ;\
    mov     r0, out

/* generate a iiii.nnnn.iiii.iiii instruction */
#define ivk_gen_inst_inii(out, i3, n2, i1, i0)  \
    mov     __h(hash)4, r3      ;\
    mov     __h(hash)i3,r0      ;\
    shld    r3,r0               ;\
    or      n2,r0               ;\
    shld    r3,r0               ;\
    or      __h(hash)i1,r0      ;\
    shld    r3,r0               ;\
    or      __h(hash)i0,r0      ;\
    mov     r0, out

/* generate a iiii.iiii.iiii.iiii instruction */
#define ivk_gen_inst_iiii(out, i3, i2, i1, i0)  \
    mov     __h(hash)4, r3      ;\
    mov     __h(hash)i3,r0      ;\
    shld    r3,r0               ;\
    or      __h(hash)i2,r0      ;\
    shld    r3,r0               ;\
    or      __h(hash)i1,r0      ;\
    shld    r3,r0               ;\
    or      __h(hash)i0,r0      ;\
    mov     r0, out


/* generate a iiii.iiii.nnnn.iiii instruction */
#define ivk_gen_inst_iini(out, i3, i2, n1, i0)  \
    mov     __h(hash)4, r3      ;\
    mov     __h(hash)i3,r0      ;\
    shld    r3,r0               ;\
    or      __h(hash)i2,r0      ;\
    shld    r3,r0               ;\
    or      n1,r0               ;\
    shld    r3,r0               ;\
    or      __h(hash)i0,r0      ;\
    mov     r0, out

//---
// ivk_push_*() - invoke area push helper
//---

/* create an instruction then push it to the area */
#define ivk_push_inst(area, type, x3, x2, x1, x0) \
    ivk_gen_inst_##type(r0, x3, x2, x1, x0)     ;\
    ivk_push_rawi(area, r0)

/* push an instruction to the invoka area */
#define ivk_push_rawi(area, inst) \
    mov.w   inst, @area         ;\
    add     __h(hash)2, area

#endif /* IVK_UTILS_H */
