#include <ncurses.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "core.h"
#include "mythology.h"

#ifndef FRAME_TIME
#define FRAME_TIME 100 
#endif

/**
 * @brief Orquestrador Core v5.5: O Despertar da Acrópole.
 * Inicializa os descritores de hardware, blinda o fluxo de sinais POSIX 
 * e prepara a matriz gráfica do terminal para receber a central de controle.
 * 
 * [🛡️] NOTA DE ENGENHARIA: Purificado 100% de dependências legadas da SDL2.
 */
void carregar_sistema(ArchonEngine *eng) {
    // Sincroniza codificação universal (Suporte a glifos e NerdFonts)
    setlocale(LC_ALL, ""); 
    
    // [⚡] BLINDAGEM DE SINAIS: Proteção contra suspensão e controle de jobs de background
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGCHLD, SIG_IGN); 

    // [🎨] MATRIZ GRÁFICA: Inicialização atômica do motor da Ncurses
    initscr();
    start_color();
    use_default_colors();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Oculta o cursor por padrão durante a navegação superior
    timeout(FRAME_TIME); 

    // Forja a paleta tática de cores mitológicas da cidadela
    init_archon_colors();

    // [🧠] BOOT DA ASTRUTURA CENTRAL: Alocação estrita e limpeza de buffers
    if (eng) {
        memset(eng, 0, sizeof(ArchonEngine));
        gethostname(eng->ctx.hostname, 64);
        
        // Zera o buffer estático do Shell interativo para evitar lixo de memória
        memset(eng->ctx.shell_input, 0, MAX_CMD_LEN);
        eng->ctx.shell_cursor = 0;
    }
}
