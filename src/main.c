#include <gint/display.h>
#include <gint/keyboard.h>


#define u32         uint32_t
#define INST_MAX    2
#define _(str, ...) \
    dprint(3, 3+((y++)*12), C_BLACK, str __VA_OPT__(,) __VA_ARGS__)

//---
// Internals
//---

/* TODO: move me */
extern void **__ivk_area_addr;

/* high-level instruction information (GUI-level)*/
struct inst_info {
    u32 rn_data;
    u32 rm_data;
    u32 rn_who;
    u32 rm_who;
    u32 M;
    u32 Q;
    u32 S;
    u32 T;
    int (*code)(int y, struct inst_info *);
};

/* low-level instruction response */
struct inst_resp {
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

static int div1(int y, struct inst_info *info)
{
    struct inst_resp resp;
    u32 ret;

    extern u32 __div1_ivk_exec(void *, void *);
    ret = __div1_ivk_exec(&resp, info);

    _("DIV1    Rm,Rn");
    _("  desc: 1-step division (Rn÷Rm)");
    _("  config:");
    _("    Rn: %d -> %08x (%d)", info->rn_who, info->rn_data, info->rn_data);
    _("    Rm: %d -> %08x (%d)", info->rm_who, info->rm_data, info->rm_data);
    _("    M,Q,S,T: %d,%d,%d,%d", info->M, info->Q, info->S, info->T);
    _("  result:");
    _("    ret: %p - %p", ret, __ivk_area_addr);
    _("    Rn: %08x (%d)", resp.rn, resp.rn);
    _("    Rm: %08x (%d)", resp.rm, resp.rm);
    _("    M,Q,S,T: %d,%d,%d,%d", resp.M, resp.Q, resp.S, resp.T);
    _("    SR: %08x - %08x", resp.SR_before, resp.SR_after);
    _("    MACL: %08x", resp.MACL);
    _("    MACH: %08x", resp.MACH);
    return y;
}


static int dmulsl(int y, struct inst_info *info)
{
    struct inst_resp resp;
    u32 ret;

    extern u32 __dmulsl_ivk_exec(void *, void *);
    ret = __dmulsl_ivk_exec(&resp, info);

    _("DMULS.L  Rm,Rn");
    _("  desc: signed 64bit Rn×Rm");
    _("  config:");
    _("    Rn: %d -> %08x (%d)", info->rn_who, info->rn_data, info->rn_data);
    _("    Rm: %d -> %08x (%d)", info->rm_who, info->rm_data, info->rm_data);
    _("    M,Q,S,T: %d,%d,%d,%d", info->M, info->Q, info->S, info->T);
    _("  result:");
    _("    ret: %p - %p", ret, __ivk_area_addr);
    _("    Rn: %08x (%d)", resp.rn, resp.rn);
    _("    Rm: %08x (%d)", resp.rm, resp.rm);
    _("    M,Q,S,T: %d,%d,%d,%d", resp.M, resp.Q, resp.S, resp.T);
    _("    SR: %08x - %08x", resp.SR_before, resp.SR_after);
    _("    MACL: %08x", resp.MACL);
    _("    MACH: %08x", resp.MACH);
    return y;
}

//---
// Public
//---

int main(void)
{
    struct inst_info table[INST_MAX] = {
        {
            .rn_who = 1,
            .rm_who = 0,
            .rn_data = 10,
            .rm_data = 3,
            .M=0,
            .Q=0,
            .S=0,
            .T=0,
            .code=&div1
        },
        {
            .rn_who = 1,
            .rm_who = 0,
            .rn_data = 10,
            .rm_data = 3,
            .M=0,
            .Q=0,
            .S=0,
            .T=0,
            .code=&dmulsl
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
            case KEY_LN:
                table[idx].M ^= 1;
                break;
            case KEY_SIN:
                table[idx].Q ^= 1;
                break;
            case KEY_COS:
                table[idx].S ^= 1;
                break;
            case KEY_TAN:
                table[idx].T ^= 1;
                break;
            case KEY_LEFT:
                if (idx < INST_MAX - 1) idx += 1;
                break;
            case KEY_RIGHT:
                if (idx > 0) idx -= 1;
                break;
        }
    }
    return 1;
}
