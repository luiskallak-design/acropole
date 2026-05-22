#include <string.h>
#include <stdint.h>
#include "constants.h"
#include "mythology.h"
#include "zeus.h" 
#include "core.h"

/* --- [🏛️] TABELAS DE DESPACHO RECALIBRADAS (NÚCLEO OPERACIONAL) --- */

// [1] ZEUS-PORTAL: Portais de Acesso Web e Recursos de Mídia Archon
// Removidos os prefixos 'kitty' redundantes para o Hermes gerenciar o spawn gráfico
// ===============================================================================
// [1] PORTAL-HERMES: Navegação, Rede e Media Gateways
// ===============================================================================
static const AppEntidade DB_PORTAL[] = {
    {"Ω", "TOR-ROUTER",   "sudo tor-router start"}, 
    {"Ϙ", "Transmission", "transmission-gtk &"},
    {"Ξ", "Chromium",     "chromium --no-first-run &"}, 
    {"Ψ", "Tor Browser",  "torbrowser-launcher &"},
    {"⚡", "ZeusBrowser",  "zeusbrowser"},
    {"🔊", "ArchonPlayer", "archonplayer"}, 
    {"λ", "Links (TUI)",  "links https://google.com"},
    {"μ", "YouTube",      "chromium --app=https://youtube.com &"},
    {"ν", "Gmail",        "chromium --app=https://google.com &"}
};

// ===============================================================================
// [2] ZEUS-ORACLE: Painel de Controle e Oráculo do Sistema
// ===============================================================================
static const AppEntidade DB_ORACLE[] = {
    {"β", "Appearance",  "lxappearance &"},
    {"γ", "GParted",     "sudo gparted"}, 
    {"δ", "BTop Status", "btop"}, 
    {"ε", "Log-System",  "journalctl -xf"}
};

// ===============================================================================
// [3] ARES-DEV: Forja de Código e Editores
// ===============================================================================
static const AppEntidade DB_ARES[] = {
    {"Γ", "Geany IDE",   "geany &"},
    {"Σ", "SUDO-Geany",  "kitty sudo geany"}, 
    {"Μ", "Micro Edit",  "micro"},
    {"Δ", "Vim Ultra",   "vim"},
    {"Ν", "Nano Forja",  "nano"},
    {"Σ", "ROOT-Nano",   "sudo nano"},
    {"Π", "FileManager", "pcmanfm &"},
    {"Θ", "Gimp Edit",   "gimp &"},
    {"Ω", "VLC Player",  "vlc &"},
    {"Φ", "Flameshot",   "flameshot gui &"}
};

// ===============================================================================
// [4] HADES-VOID: Painel de Telemetria e Análise Visual (Modo Leitura)
// ===============================================================================
static const AppEntidade DB_HADES[] = {
    {"📈", "Bmon Rede",       "bmon"},
    {"📊", "Glances Monitor", "glances"},
    {"📡", "MTR Rota",        "mtr --user-mode"}, 
    {"🌐", "HTTPing Latency", "httping -K -g"},  
    {"📉", "TipTop Thread",   "tiptop"}
};

// ===============================================================================
// [5] FORJA-SYS: Hefesto - Engrenagens e Telemetria
// ===============================================================================
static const AppEntidade DB_HEFESTO[] = {
    {"⌬", "Btop",        "btop"}, 
    {"⚙", "Htop",        "htop"},
    {"Ξ", "Xarchiver",   "xarchiver &"},
    {"Ψ", "Zathura",     "zathura &"},             
    {"Α", "AbiWord",     "abiword &"},            
    {"Γ", "Gnumeric",    "gnumeric &"},           
    {"Η", "Fastfetch",   "fastfetch"} 
};


/* --- [🧠] GERENCIADOR DE DESPACHO --- */

static const AppEntidade *active_db = NULL;
static uint32_t active_db_size = 0;

const char *dominios_myth[] = { 
    "ZEUS-PORTAL", "ZEUS-ORACLE", "ARES-DEV", "FORJA-SYS", "HADES-VOID" 
};

int colher_apps_por_dominio(const char *keyword) {
    if (!keyword) return 0;

    if (strstr(keyword, "PORTAL")) {
        active_db = DB_PORTAL;
        active_db_size = sizeof(DB_PORTAL) / sizeof(AppEntidade);
    } 
    else if (strstr(keyword, "ORACLE")) {
        active_db = DB_ORACLE;
        active_db_size = sizeof(DB_ORACLE) / sizeof(AppEntidade);
    }
    else if (strstr(keyword, "ARES")) {
        active_db = DB_ARES;
        active_db_size = sizeof(DB_ARES) / sizeof(AppEntidade);
    }
    else if (strstr(keyword, "VOID") || strstr(keyword, "HADES")) {
        active_db = DB_HADES;
        active_db_size = sizeof(DB_HADES) / sizeof(AppEntidade);
    }
    else if (strstr(keyword, "SYS") || strstr(keyword, "FORJA")) {
        active_db = DB_HEFESTO;
        active_db_size = sizeof(DB_HEFESTO) / sizeof(AppEntidade);
    }

    return (int)active_db_size;
}

void obter_app_submenu_completo(int idx, char *icone, char *nome, char *cmd) {
    if (!icone || !nome || !cmd) return;

    if (active_db && idx >= 0 && (uint32_t)idx < active_db_size) {
        // [🛡️] CALIBRAGEM MITOLÓGICA: Cópia segura de strings estáticas fixas
        // Remove os testes de ponteiro NULL, pois vetores fixos sempre possuem endereço válido
        
        // 1. Copia o Ícone com limite estrito de segurança (16 - 1)
        strncpy(icone, active_db[idx].icone, 15);
        icone[15] = '\0';

        // 2. Copia o Nome com limite estrito de segurança (64 - 1)
        strncpy(nome, active_db[idx].nome, 63);
        nome[63] = '\0';

        // 3. Copia o Comando mapeando direto para o membro .comando do seu constants.h
        strncpy(cmd, active_db[idx].comando, MAX_CMD_LEN - 1);
        cmd[MAX_CMD_LEN - 1] = '\0';
        
    } else {
        // Fallback de segurança para posições vazias ou fora de escopo
        strcpy(icone, " "); 
        strcpy(nome, " "); 
        strcpy(cmd, " ");
    }
}
