#ifndef CORE_H
#define CORE_H

#include "constants.h"
#include "zeus.h"

#define TARGET_FPS  60
#define FRAME_TIME  (1000 / TARGET_FPS) 

// --- ESTRUTURAS DE CONTEXTO ---

typedef struct {
    int sel_pai;          // Índice selecionado no painel esquerdo (Domínios)
    int sel_filho;        // Índice selecionado no painel direito (Arquivos/Apps)
    int active_pane;      // Painel em foco (0: Alpha, 1: Omega, 2: Shell)
    int loop_count;       // Contador para animações de interface
    int total_filhos;     // Quantidade de itens carregados no painel direito
    int offset_omega;     // Controle de scroll vertical do painel direito
    char ram[32];         // String de status do uso de memória
    char cpu[16];         // String de status do uso de processamento
    char hostname[HOST_MAX]; 
    
    /* --- NÚCLEO PRIMORDIAL: TERMINAL INTERATIVO EMBARCADOS --- */
    char shell_input[MAX_CMD_LEN]; // Buffer tático para o prompt inferior
    int shell_cursor;              // Posição do ponteiro de escrita
} CoreContext;

typedef struct {
    CoreContext ctx;
    BrowseItem  cache_items[MAX_ITEMS]; // Cache tático de arquivos e aplicações
    int         should_exit;            
} ArchonEngine;

// --- VARIÁVEIS EXTERNAS ---
extern const char *dominios_myth[];

// --- [🛡️] PROTÓTOTIPOS DOS MÓDULOS DE ELITE ---

// Módulo: core_logic.c
void carregar_sistema(ArchonEngine *eng);

// Módulo: atlas.c (Hardware Monitor)
void ler_ram(char *buffer);
void ler_cpu(char *buffer);

// Módulo: hermes.c (Executor)
void invocar_entidade(const char *cmd, int forcar_float);

// Módulo: lethe.c (Clean/Shutdown)
void lethe_init(void);
void archon_shutdown(void);

// Módulo: portal.c / apps.c (Navegação de Domínios)
int colher_apps_por_dominio(const char *keyword);
void obter_app_submenu_completo(int idx, char *icone, char *nome, char *cmd);

// Módulo: oracle.c (Sistema de Arquivos)
int scan_folder(BrowseItem *items);

// Módulo: mythology.c (Interface/Cores)
void init_archon_colors(void);
void draw_pill(int y, int x, const char *icon, const char *label, int selected);
void alternar_tema(void);

#endif /* CORE_H */
