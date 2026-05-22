#ifndef MYTHOLOGY_H
#define MYTHOLOGY_H

#include "constants.h"

#define PILL_LEFT  ""
#define PILL_RIGHT ""

#define CP_ACCENT    1  
#define CP_DIMMED    2  
#define CP_ALERT     3  
#define CP_PILL_SEL  4  

void init_archon_colors();
void draw_pill(int y, int x, const char *icon, const char *label, int selected);
void alternar_tema();
void sync_pywal();

#endif
