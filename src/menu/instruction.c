#include <string.h>
#include <stdbool.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/gint.h>

#include "ivk/instruction.h"
#include "ivk/menu.h"
#include "ivk/utils.h"
#include "ivk/ivk.h"

//---
// instruction test menu
//---

/* display / run the instruction */
void menu_instruction_exec(IvkMenuInst *menu, int idx)
{
    struct instruction_gui_info *info;
    struct ivk_ctx_req req;
    struct ivk_ctx_out out;
    key_event_t ev;
    bool executed;
    bool exit;
    int ret;
    int y;

    (void)menu;

    info = &instruction_table[idx];
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

        while((ev = pollevent()).type != KEYEV_NONE)
        {
            if (ev.type == KEYEV_UP)
                continue;
            switch (ev.key)
            {
                case KEY_RIGHTP:
                    req.rn_data += 1;
                    break;
                case KEY_LEFTP:
                    req.rn_data -= 1;
                    break;
                case KEY_ARROW:
                    req.rm_data += 1;
                    break;
                case KEY_COMMA:
                    req.rm_data -= 1;
                    break;
            }
            if (ev.type != KEYEV_DOWN)
                continue;
            switch (ev.key)
            {
                case KEY_MENU:
                    gint_osmenu();
                    break;
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
}

int menu_instruction_display_row(IvkMenuInst *menu, int y, int idx)
{
    if (idx >= menu->nb_entry)
        return -1;
    dtext(4, 10 + (y * 14) + 2, C_BLACK, instruction_table[idx].opname);
    dtext_opt(
        DWIDTH - 9,
        10 + (y * 14) + 2,
        C_BLACK,
        C_NONE,
        DTEXT_RIGHT,
        DTEXT_TOP,
        instruction_table[idx].desc
    );
    return 0;
}

//---
// Menu interface
//---

void menu_instruction_init(IvkMenuInst *menu)
{
    int nb_entry;

    nb_entry = -1;
    while (true) {
        if (instruction_table[++nb_entry].trampoline == NULL)
            break;
    }
    memset(menu, 0x00, sizeof(IvkMenuInst));
    menu_list_init(
        &menu->list,
        menu,
        (void*)&menu_instruction_display_row,
        (void*)&menu_instruction_exec,
        nb_entry
    );
    menu->nb_entry = nb_entry;
}

void menu_instruction_display(IvkMenuInst *menu)
{
    dclear(C_WHITE);
    _title("Instruction selection >");
    menu_list_display(&menu->list);
    dupdate();
}

void menu_instruction_key(IvkMenuInst *menu, key_event_t keyev)
{
    menu_list_key(&menu->list, keyev);
}
