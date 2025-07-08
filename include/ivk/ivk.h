#ifndef IVK_H
#define IVK_H 1

#include <stdint.h>

#define u32 uint32_t

/* instruction request */
struct ivk_ctx_req {
    u32 rn_data;
    u32 rm_data;
    u32 rn_who;
    u32 rm_who;
    u32 M;
    u32 Q;
    u32 S;
    u32 T;
};

/* low-level instruction response */
struct ivk_ctx_out {
    u32 rn;
    u32 rm;
    u32 M;
    u32 Q;
    u32 S;
    u32 T;
    u32 SR_before;
    u32 SR_after;
    u32 MACL;
    u32 MACH;
};

/* debug information */
extern void **__ivk_area_addr;

#endif /* IVK_H */
