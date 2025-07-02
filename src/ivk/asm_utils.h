#ifndef __ASM_UTIL_H__
#define __ASM_UTIL_H__  1

//---
// Internals
//---

/* woraround to trick the preprocessor */
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
#define asm_declare_function(name) \
    .balign 4                   ;\
    .global _##name             ;\
    .type   _##name,@function   ;\
    _##name

#define asm_declare_object(name) \
    .balign 4                   ;\
    .global _##name             ;\
    .type   _##name,@object     ;\
    _##name

//---
// asm_check_*() - short-hand check
//---

/* basic NULL return and return -1 */
#define asm_check_null(reg, label)  \
    tst     reg, reg            ;\
    bt.s    label               ;\
    mov     __h(hash)1, r0

#endif /* __ASM_UTIL_H__ */
