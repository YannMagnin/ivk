#ifndef _UTILS_H
#define _UTILS_H 1

#include <stddef.h>
#include <stdint.h>

#include <gint/display.h>
#include <gint/keyboard.h>

/* internal short-hand for print */
#define _(str, ...) \
    dprint(3, 3+((y++)*12), C_BLACK, str __VA_OPT__(,) __VA_ARGS__)

#endif /* _UTILS_H */
