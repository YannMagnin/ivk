#ifndef __IVK_H__
#define __IVK_H__ 1

//---
// ivk_*() - invokation macros
//---

/* get info->rn_who */
#define ivk_get_rn_who(info, out, error) \
    mov.l   @(8, info), out     ;\
    mov     #15, r0             ;\
    cmp/eq  r0, out             ;\
    bt.s    error               ;\
    mov     #-3, r0

/* get info->rm_who */
#define ivk_get_rm_who(info, out, error) \
    mov.l   @(12, info), out    ;\
    mov     #15, r0             ;\
    cmp/eq  r0, out             ;\
    bt      error               ;\
    mov     #-3, r0

#define ivk_call_prepare(label_exit) \
    mov.l   1f, r0              ;\
    jsr     @r0                 ;\
    nop                         ;\
    tst     r0,r0               ;\
    bf      label_exit          ;\
    bra     2f                  ;\
.balign 4                       ;\
1:                              ;\
    .long   ___ivk_prepare      ;\
2:


//---
// asm_push_inst_*() - generate and push instruction to the area
//
// note:
//  - r3 is reserved
//  - r0 is reserved
//---

/* generate a iiii.nnnn.mmmm.iiii instruction */
#define ivk_push_inst_inmi(area, i3, n2, m1, i0)  \
    mov     __h(hash)4, r3  ;\
    mov     __h(hash)i3,r0  ;\
    shld    r3,r0           ;\
    or      n2,r0           ;\
    shld    r3,r0           ;\
    or      m1,r0           ;\
    shld    r3,r0           ;\
    or      __h(hash)i0,r0  ;\
    mov.w   r0, @area       ;\
    add     __h(hash)2,area

/* generate a iiii.nnnn.iiii.iiii instruction */
#define ivk_push_inst_inii(area, i3, n2, i1, i0)  \
    mov     __h(hash)4, r3  ;\
    mov     __h(hash)i3,r0  ;\
    shld    r3,r0           ;\
    or      n2,r0           ;\
    shld    r3,r0           ;\
    or      __h(hash)i1,r0  ;\
    shld    r3,r0           ;\
    or      __h(hash)i0,r0  ;\
    mov.w   r0, @area       ;\
    add     __h(hash)2,area

/* generate a iiii.iiii.iiii.iiii instruction */
#define ivk_push_inst_iiii(area, i3, i2, i1, i0)  \
    mov     __h(hash)4, r3  ;\
    mov     __h(hash)i3,r0  ;\
    shld    r3,r0           ;\
    or      __h(hash)i2,r0  ;\
    shld    r3,r0           ;\
    or      __h(hash)i1,r0  ;\
    shld    r3,r0           ;\
    or      __h(hash)i0,r0  ;\
    mov.w   r0, @area       ;\
    add     __h(hash)2,area



#endif /* __IVK_H__ */
