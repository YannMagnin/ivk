//---
// ivk/code     - code generation
//---
#include "./asm_utils.h"
#include "./ivk_utils.h"
.text

/* Generate the code that need to be executed
**
** prototypes:
**  - r4    <u16 *>     invoke area address
**  - r5    <u16>       target instruction to exec
**  - r6    [0:14]      Rn ID
**  - r7    [0:14]      Rm ID
** note:
**  - the stub will always be called with rn_data=r0 and rm_data=r1
**  - handle particular register collision
*/
asm_declare_function(__ivk_code_gen):
    tst     r7, r7
    bt      ivk_code_gen_special_stub
ivk_code_gen_default_stub:
    ivk_push_inst(r4, inii, 0b0110,     r7, 0b0001, 0b0011) ! mov   r1,rm
    ivk_push_inst(r4, inii, 0b0110,     r6, 0b0000, 0b0011) ! mov   r0,rn
    ivk_push_rawi(r4, r5)                                   ! <target>
    ivk_push_inst(r4, iini, 0b0110, 0b0000,     r6, 0b0011) ! mov   rn,r0
    ivk_push_inst(r4, iiii, 0b0000, 0b0000, 0b0000, 0b1011) ! rts
    ivk_push_inst(r4, iini, 0b0110, 0b0001,     r7, 0b0011) ! mov   rm,r1
    rts
    nop
ivk_code_gen_special_stub:
    ivk_push_inst(r4, iiii, 0b0110, 0b0010, 0b0000, 0b0011) ! mov   r0,r2
    ivk_push_inst(r4, iiii, 0b0110, 0b0000, 0b0001, 0b0011) ! mov   r1,r0
    ivk_push_inst(r4, inii, 0b0110,     r6, 0b0010, 0b0011) ! mov   r2,rn
    ivk_push_rawi(r4, r5)                                   ! <target>
    ivk_push_inst(r4, iini, 0b0110, 0b0010,     r6, 0b0011) ! mov   rn,r2
    ivk_push_inst(r4, iini, 0b0110, 0b0001,     r7, 0b0011) ! mov   rm,r1
    ivk_push_inst(r4, iiii, 0b0000, 0b0000, 0b0000, 0b1011) ! rts
    ivk_push_inst(r4, iiii, 0b0110, 0b0000, 0b0010, 0b0011) ! mov   r2,r0
    rts
    nop
