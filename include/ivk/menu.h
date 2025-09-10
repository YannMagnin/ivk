#ifndef IVK_MENU_H
#define IVK_MENU_H 1

#include <gint/keyboard.h>
#include <gint/display.h>

#include "ivk/utils.h"

//---
// Instruction menu
//---

/* internal menu information */
struct IvkMenuInst {
    i8 len;
    i8 offset;
    i8 pos;
    i8 top;
    i8 bottom;
};
typedef struct IvkMenuInst IvkMenuInst;

/* menu init */
extern void menu_instruction_init(IvkMenuInst *menu);

/* menu display */
extern void menu_instruction_display(IvkMenuInst *menu);

/* menu key event handling */
extern void menu_instruction_key(IvkMenuInst *menu, key_event_t keyev);

#endif /* IVK_MENU_H */
