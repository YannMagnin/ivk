#include "./ivk/ivk_utils.h"
.text

//---
// Public
//---

ivk_declare_instruction(div1,   inmi, 0b0011, r1, r2, 0b0100)
ivk_declare_instruction(dmulsl, inmi, 0b0011, r1, r2, 0b1101)
