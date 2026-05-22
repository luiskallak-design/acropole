#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "mythology.h"
#include "core.h"

static int esquema_atual = 0;

// NOTA: 'spectrum_data' e 'draw_spectrum' NÃO são definidos aqui.
// Eles são importados do archonplayer.o via core.h/zeus.h.

void init_archon_colors() {
    start_color();
    use_default_colors();
    
    switch (esquema_atual) {
        case 0: // EMERALD (Pip-Boy Classic)
            init_pair(CP_ACCENT, 48, -1);
            init_pair(CP_DIMMED, 34, -1);
            init_pair(CP_PILL_SEL, 0, 48); 
            init_pair(CP_ALERT, 196, -1);   
            break;
        case 1: // BLOOD
            init_pair(CP_ACCENT, 160, -1);
            init_pair(CP_DIMMED, 88, -1);
            init_pair(CP_PILL_SEL, 15, 160); 
            init_pair(CP_ALERT, 226, -1);   
            break;
        case 2: // CYBER
            init_pair(CP_ACCENT, 51, -1);
            init_pair(CP_DIMMED, 33, -1);
            init_pair(CP_PILL_SEL, 0, 51); 
            init_pair(CP_ALERT, 213, -1);   
            break;
        case 3: // GOLDEN
            init_pair(CP_ACCENT, 220, -1);
            init_pair(CP_DIMMED, 136, -1);
            init_pair(CP_PILL_SEL, 0, 220); 
            init_pair(CP_ALERT, 15, -1);    
            break;
    }
}

void draw_pill(int y, int x, const char *icon, const char *label, int selected) {
    if (selected) {
        attron(COLOR_PAIR(CP_PILL_SEL));
        mvprintw(y, x, "[ %s %-18.18s ]", icon, label);
        attroff(COLOR_PAIR(CP_PILL_SEL));
    } else {
        attron(COLOR_PAIR(CP_DIMMED));
        mvprintw(y, x, "  %s %-18.18s  ", icon, label);
        attroff(COLOR_PAIR(CP_DIMMED));
    }
}

void alternar_tema() {
    esquema_atual = (esquema_atual + 1) % 4; 
    init_archon_colors();
    clear(); 
}

void sync_pywal() {
    alternar_tema();
    // No futuro, descomente para integrar:
    // system("wal -r"); 
}
