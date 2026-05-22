#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

/* --- 1. BLINDAGEM DE MEMÓRIA (POKA-YOKE) --- */
// Potências de 2 para otimização de cache e alinhamento de memória
#define MAX_ITEMS    1024      // Limite para o Oracle/Zeus
#define MAX_APPS     256       // Suporta arsenais massivos (BlackArch)
#define MAX_CMD_LEN  2048      // Espaço para comandos complexos e wrappers
#define MAX_PATH_LEN 1024      // Suporte a caminhos absolutos profundos
#define HOST_MAX     64        // Padrão POSIX para nomes de rede

/* --- 2. PALETA TÁTICA (FALLBACK & ANSI) --- */
// Usados para logs fora do ncurses e mensagens de erro do terminal
#define CLR_VINHO "\033[38;5;88m"
#define CLR_CYAN  "\033[38;5;44m"
#define CLR_GOLD  "\033[38;5;220m"
#define CLR_RESET "\033[0m"

/* --- 3. MAPEAMENTO DE ENTRADA (NCURSES SYNC) --- */
#ifndef KEY_ESC
    #define KEY_ESC 27
#endif
#define KEY_TAB     9
#define KEY_ENTER_L 10

/* --- 4. ARQUITETURA DE ESTADOS E DADOS --- */

// Máquina de Estados: Determina o comportamento do Orquestrador (Expandido com MODO_SHELL)
typedef enum {
    MODO_OLIMPO,  // Foco no Lado Alpha (Domínios)
    MODO_TERRA,   // Foco no Lado Omega (Aplicativos/Oracle)
    MODO_ORACLE,  // Estado especial para navegação de arquivos Zeus
    MODO_SHELL    // NOVO: Terminal interativo assíncrono inferior
} EstadoSistema;

// DominioPai: O alicerce visual dos menus
typedef struct {
    char titulo[32];   
    char keyword[32];  
    char icone[16];    
} DominioPai;

// AppEntidade: O átomo de execução (O que Hermes dispara)
typedef struct {
    char icone[16];    // Símbolo grego ou NerdFont
    char nome[64];     // Label legível
    char comando[MAX_CMD_LEN]; // Comando real sanitizado
} AppEntidade;

/* --- 5. DNA VISUAL (MOTOR DE ANIMAÇÃO) --- */
// 'static const' garante que o spinner seja thread-safe e read-only
static const char* SPINNER[] __attribute__((unused)) = {
    "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"
};
#define SPINNER_SIZE 10

#endif /* CONSTANTS_H */
