#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include "core.h"

/**
 * @brief Camada Lethe: Protocolo de Purificação e Encerramento.
 * Garante a restauração atômica dos descritores de hardware e terminal.
 * Purificado 100% de dependências legadas da SDL2.
 */

// Flag estática para evitar reentrância (Double Shutdown)
static int ja_encerrado = 0;

void archon_shutdown(void) {
    if (ja_encerrado) return;
    ja_encerrado = 1;

    /* 1. Protocolo Lethe: Restauração de TTY (Ncurses) */
    if (!isendwin()) {
        echo();
        nocbreak();
        curs_set(1); // Restaura o cursor visível nativo para o shell do Linux
        endwin();
    }

    /* 2. Limpeza Atômica de Buffer */
    // Usamos write direto para garantir que o terminal limpe mesmo se o stdout estiver bufferizado
    const char *cleanup = "\033[H\033[2J\033[?25h"; 
    (void)write(STDOUT_FILENO, cleanup, 10);
}

/**
 * @brief Handler de Interrupções de Sistema.
 * Intercepta sinais de terminação para evitar "Panic Exit".
 */
void handle_signal(int sig) {
    (void)sig; 
    archon_shutdown();
    // Usamos _exit para evitar chamadas redundantes de atexit()
    _exit(EXIT_SUCCESS);
}

/**
 * @brief Inicialização do Módulo de Limpeza.
 * Configura os guardiões de integridade da Acrópole.
 */
void lethe_init(void) {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Reinicia syscalls interrompidas

    sigaction(SIGINT,  &sa, NULL); // Ctrl+C
    sigaction(SIGTERM, &sa, NULL); // Kill externo
    sigaction(SIGHUP,  &sa, NULL); // Fechamento de Terminal
}
