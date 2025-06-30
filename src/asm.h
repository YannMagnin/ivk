#ifndef __ASM_UTIL_H__
#define __ASM_UTIL_H__  1

//---
// Internals
//---

/* woraround to trick the */
#define hash    #
#define __h(x)  x

//---
// Public
//---

/* define a function using special indication
 *
 * notes:
 * - the function's name must start with a '_'
 * - the function should start in a 4-aligned address in order to benefit of
 *      the ILP (Instruction Level Parallelism) */
#define asm_gen_function(name)   \
    .balign 4                   ;\
    .global _##name             ;\
    .type   _##name,@function   ;\
    _##name

//---
// asm_check_*() - short-hand check
//---

/* basic NULL return and return -1 */
#define asm_check_null(reg, label)  \
    tst     reg, reg            ;\
    bt.s    label               ;\
    mov     __h(hash)1, r0

//---
// ivk_*() - invokation define
//---

//---
// get the invokation area (small 126 bytes spaces)
//
// notes:
//  - assume r4,r5 must be restored
//  - assume r8,r9,r10 unused
//  - invoke malloc if the area is not created
// out:
//  - r0    invokation area address
// reserved:
//  - r10   quick save high-level area addr
//  - r8    quick save r4
//  - r9    quick save r5
//  - r0    return
//---
#define ivk_get_area(rn, label_return)   \
    mov.l   1f, r10             ;\
    mov.l   @r10, r0            ;\
    tst     r0, r0              ;\
    bf      __ivk_get_area_end  ;\
    mov.l   2f, r1              ;\
    mov     r4, r8              ;\
    mov     r5, r9              ;\
    jsr     @r1                 ;\
    mov     __h(hash)126, r4    ;\
    mov.l   r0, @r10            ;\
    tst     r0, r0              ;\
    mov     r8, r4              ;\
    bf.s    __ivk_get_area_end  ;\
    mov     r9, r5              ;\
    bra     label_return        ;\
    mov     #-2, r0             ;\
.balign 4                       ;\
1:  .long ___ivk_area_addr      ;\
2:  .long _malloc               ;\
__ivk_get_area_end:             ;\
    mov     r0, rn

/* */
#define ivk_get_info(off, info, out, error) \

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

#endif /* __ASM_UTIL_H__ */
