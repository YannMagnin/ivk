#include <stdbool.h>
#include <string.h>

#include "ivk/instruction.h"
#include "ivk/utils.h"
#include "ivk/ivk.h"

//---
// Public
//---

/* display / run the instruction */
void manual_gui_entry(struct instruction_gui_info *info)
{
    struct ivk_ctx_req req;
    struct ivk_ctx_out out;
    bool executed;
    bool exit;
    int ret;
    int y;

    memset(&req, 0x00, sizeof(req));
    memset(&out, 0x00, sizeof(out));
    req.rn_who = 1;
    req.rm_who = 0;
    req.rn_data = 10;
    req.rm_data = 3;

    ret = 0;
    exit = false;
    executed = false;
    while (exit == false)
    {
        if (executed)
            ret = ((int(*)(void*,void*))info->trampoline)(&out, &req);
        y = 1;
        dclear(C_WHITE);
        _title(info->opname);
        _("  desc: %s", info->desc);
        _("  config:");
        _("    Rn: %d -> %08x (%d)", req.rn_who, req.rn_data, req.rn_data);
        _("    Rm: %d -> %08x (%d)", req.rm_who, req.rm_data, req.rm_data);
        _("    M,Q,S,T: %d,%d,%d,%d", req.M, req.Q, req.S, req.T);
        _("  result:");
        if (executed) {
            _("    ret: %p - %p", ret, __ivk_area_addr);
            _("    Rn: %08x (%d)", out.rn, out.rn);
            _("    Rm: %08x (%d)", out.rm, out.rm);
            _("    M,Q,S,T: %d,%d,%d,%d", out.M, out.Q, out.S, out.T);
            _("    SR: %08x - %08x", out.SR_before, out.SR_after);
            _("    MACL: %08x", out.MACL);
            _("    MACH: %08x", out.MACH);
        } else {
            _("    No executed yep, press [EXE]");
        }
        dupdate();
        switch (getkey().key)
        {
            case KEY_XOT:
                if (req.rn_who < 14) req.rn_who += 1;
                break;
            case KEY_FRAC:
                if (req.rn_who > 0) req.rn_who -= 1;
                break;
            case KEY_LOG:
                if (req.rm_who < 14) req.rm_who += 1;
                break;
            case KEY_FD:
                if (req.rm_who > 0) req.rm_who -= 1;
                break;
            case KEY_LEFTP:
                req.rn_data += 1;
                break;
            case KEY_RIGHTP:
                req.rn_data -= 1;
                break;
            case KEY_COMMA:
                req.rm_data += 1;
                break;
            case KEY_ARROW:
                req.rm_data -= 1;
                break;
            case KEY_LN:
                req.M ^= 1;
                break;
            case KEY_SIN:
                req.Q ^= 1;
                break;
            case KEY_COS:
                req.S ^= 1;
                break;
            case KEY_TAN:
                req.T ^= 1;
                break;
            case KEY_EXE:
                executed = true;
                break;
            case KEY_EXIT:
                exit = true;
                break;
        }
    }
}
