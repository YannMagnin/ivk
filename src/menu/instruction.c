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
void manual_gui_entry(struct instruction_gui_info *info)
{
    struct ivk_ctx_req req;
    struct ivk_ctx_out out;
    key_event_t ev;
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

        while((ev = pollevent()).type != KEYEV_NONE)
        {
            if (ev.type == KEYEV_UP)
                continue;
            switch (ev.key)
            {
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

//---
// Menu interface
//---

void menu_instruction_init(IvkMenuInst *menu)
{
    menu->len = -1;
    while (true) {
        if (instruction_table[++menu->len].trampoline == NULL)
            break;
    }
    menu->offset = 0;
    menu->pos = 0;
    menu->top = 1;
    menu->bottom = ROW_COUNT + 1;
}

void menu_instruction_display(IvkMenuInst *menu)
{
    int offset = menu->offset;
    int pos = menu->pos;
    int top = menu->top;
    int bottom = menu->bottom;
    int i = 0;
    int j = top;

    dclear(C_WHITE);
    _title("Instruction selection");
    while(j < bottom && instruction_table[offset+i].trampoline != NULL)
    {
        _row(
            2, j,
            instruction_table[offset+i].opname,
            instruction_table[offset+i].desc
        );
        i++;
        j++;
    }
    if(menu->len > bottom - top)
    {
        int area_x      = 391;
        int area_width  = 2;
        int area_top    = ROW_Y + ROW_H * (top - 1);
        int area_height = ROW_H * (bottom - top);

        int bar_top = (offset * area_height) / menu->len;
        int bar_height = ((bottom - top) * area_height) / menu->len;

        drect(area_x, area_top + bar_top + 3, area_x + area_width - 1,
                area_top + bar_top + bar_height + 4, C_BLACK);
    }
    int selected = top + (pos - offset);
    if(selected >= top && selected < bottom)
        _highlight(selected);
    dupdate();
}

void menu_instruction_key(IvkMenuInst *menu, key_event_t keyev)
{
    int max_offset;
    int visible;

    visible = menu->bottom - menu->top;
    max_offset = max(menu->len - visible, 0);
    switch(keyev.key)
    {
        case KEY_EXE:
            if (keyev.type == KEYEV_DOWN)
                manual_gui_entry(&instruction_table[menu->pos]);
            break;
        case KEY_UP:
            if (menu->pos > 0) {
                menu->pos--;
                menu->offset = min(menu->offset, menu->pos);
            } else {
                menu->pos = menu->len - 1;
                menu->offset = max_offset;
            }
            break;
        case KEY_DOWN:
            if (++menu->pos < menu->len) {
                if(menu->pos > menu->offset + visible - 1 &&
                        menu->offset + 1 <= max_offset) {
                    menu->offset++;
                }
            } else {
                menu->pos = 0;
                menu->offset = 0;
            }
            break;
    }
}
