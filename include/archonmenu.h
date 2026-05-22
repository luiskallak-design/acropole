#ifndef ARCHONMENU_H
#define ARCHONMENU_H

#include <ncurses.h>

/* --- CONFIGURAÇÕES DE DESIGN CARONTE --- */
#define PILL_LEFT  ""
#define PILL_RIGHT ""
#define SEP_SOLID  "█"

/* --- CORES PYWAL --- */
#define CP_ACCENT    1
#define CP_DIMMED    2
#define CP_CRITICAL  3
#define CP_PILL_SEL  4

/* --- ESTRUTURAS --- */
typedef struct {
    char *label;
    char *icon;
    char *exec_cmd;
} MenuStack;

/* --- PROTÓTIPOS --- */
void init_archon_colors();
void sync_pywal();
void draw_pill_label(int y, int x, char *icon, char *label, int selected);

#endif
