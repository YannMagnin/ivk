#include <gint/display.h>
#include <gint/keyboard.h>


#define u32         uint32_t
#define INST_MAX    1
#define _print(str, ...) \
    dprint(0, (y++)*12, C_BLACK, str __VA_OPT__(,) __VA_ARGS__)

//---
// Internals
//---

/* TODO: move me */
void **__ivk_area_addr = (void*)0x00000000;

/* high-level instruction information (GUI-level)*/
struct inst_info {
    u32 rn_data;
    u32 rm_data;
    u32 rn_who;
    u32 rm_who;
    u32 M;
    u32 Q;
    u32 T;
    int (*code)(int y, struct inst_info *);
};

/* low-level instruction response */
struct inst_resp {
    u32 rn;
    u32 rm;
    u32 M;
    u32 Q;
    u32 T;
    u32 SR_before;
    u32 SR_after;
    u32 MACL;
    u32 MACH;
};

static int div1(int y, struct inst_info *info)
{
    struct inst_resp resp;
    u32 ret;

    extern u32 __div1_magic_invoke(void *, void *);
    ret = __div1_magic_invoke(&resp, &info);

    _print("DIV1    Rm,Rn");
    _print("  desc: '1-step division (Rn ÷ Rm)'");
    _print("  config:");
    _print("    Rn: %d -> %08x", info->rn_who, info->rn_data);
    _print("    Rm: %d -> %08x", info->rm_who, info->rm_data);
    _print("    M, Q, T: %d, %d, %d", info->M, info->Q, info->T);
    _print("  result:");
    _print("    ret: %p - %p", ret, __ivk_area_addr);
    _print("    Rn: %08x", resp.rn);
    _print("    Rm: %08x", resp.rm);
    _print("    M, Q, T: %d, %d, %d", resp.M, resp.Q, resp.T);
    _print("    MACL: %08x", resp.MACL);
    _print("    MACH: %08x", resp.MACH);
    return y;
}

//---
// Public
//---

int main(void)
{
    struct inst_info table[INST_MAX] = {
        {
            .rn_who = 0,
            .rm_who = 0,
            .rn_data=0xdeadbeef,
            .rm_data=0xb0cad0de,
            .M=0,
            .Q=0,
            .T=0,
            .code=&div1
        },
    };
    int idx;

    idx = 0;
    while (true)
    {
        dclear(C_WHITE);
        table[idx].code(0, &table[idx]);
        dupdate();

        switch (getkey().key)
        {
            case KEY_XOT:
                if (table[idx].rn_who < 15) table[idx].rn_who += 1;
                break;
            case KEY_FRAC:
                if (table[idx].rn_who > 0) table[idx].rn_who -= 1;
                break;
            case KEY_LOG:
                if (table[idx].rm_who < 15) table[idx].rm_who += 1;
                break;
            case KEY_FD:
                if (table[idx].rm_who > 0) table[idx].rm_who -= 1;
                break;
            case KEY_LEFTP:
                table[idx].rn_data += 1;
                break;
            case KEY_RIGHTP:
                table[idx].rn_data -= 1;
                break;
            case KEY_COMMA:
                table[idx].rm_data += 1;
                break;
            case KEY_ARROW:
                table[idx].rm_data -= 1;
                break;
            case KEY_SIN:
                table[idx].M ^= 1;
                break;
            case KEY_COS:
                table[idx].Q ^= 1;
                break;
            case KEY_TAN:
                table[idx].T ^= 1;
                break;
        }
    }
    return 1;
}
