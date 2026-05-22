#ifndef ZEUS_H
#define ZEUS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define SPECTRUM_SIZE 256

/* BrowseItem: 256 bytes para o nome + 4 bytes para o int = 260 bytes totais.
   Isso alinha perfeitamente com o seu archonplayer.o e limpa o lixo visual. */
typedef struct {
    char name[256];           
    int is_dir;
} BrowseItem;

extern int spectrum_data[SPECTRUM_SIZE];

int  scan_folder(BrowseItem *items);
void draw_spectrum(int y_base, int x_start, int x_end);
void audio_callback(int chan, void *stream, int len, void *udata);


#endif
