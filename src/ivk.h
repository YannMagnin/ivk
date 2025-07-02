#ifndef __IVK_H__
#define __IVK_H__ 1

#include "./ivk/asm_utils.h"
#include "./ivk/ivk_utils.h"

//---
// Public
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

#endif /* __IVK_H__ */
