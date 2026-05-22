/* --- BIBLIOTECAS PADRÃO E POSIX --- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>

/* --- CONTROLE DE PROCESSOS E SINAIS --- */
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <dirent.h>

/* --- INTERFACE DE USUÁRIO (TUI) --- */
#include <ncurses.h>
#include "core.h"
#include "mythology.h"

// [🎨] VARIÁVEIS DE ESTADO DO NOVO SHELL ASSÍNCRONO
static char shell_buffer[MAX_CMD_LEN] = {0};
static int shell_cursor_pos = 0;

// --- [ PROTÓTIPOS: ENTIDADES EXTERNAS ] ---
void invocar_entidade(const char *cmd, int precisa_terminal);
void sync_pywal(void);

// --- [ PROTÓTIPOS: LÓGICA LOCAL ] ---
static void launch_terminal_app(const char *full_path);
static void draw_footer_keys(int my);
static int identificar_tipo_arquivo(const char *path);
static void atualizar_conteudo_omega(ArchonEngine *eng);
static void engine_render(ArchonEngine *eng);

static void launch_terminal_app(const char *full_path) {
    invocar_entidade(full_path, 1);
}

static void draw_footer_keys(int my) {
    const char *keys[] = {
        "F1", "PROTOGNUM", "F2", "ARCHONPLAYER", 
        "F3", "ZEUSBROWSER", "F4", "NANO", 
        "F5", "CORES", "F10", "EXIT"
    };
    attron(COLOR_PAIR(CP_ACCENT));
    mvprintw(my - 1, 0, " ⌬ ACRÓPOLE_OS: ");
    attroff(COLOR_PAIR(CP_ACCENT));
    for (int i = 0; i < 12; i += 2) {
        attron(COLOR_PAIR(CP_DIMMED) | A_REVERSE);
        printw(" %s ", keys[i]);
        attroff(A_REVERSE);
        attron(COLOR_PAIR(CP_ACCENT));
        printw(" %s ", keys[i+1]);
        attroff(COLOR_PAIR(CP_ACCENT));
        printw(" ");
    }
}

static int identificar_tipo_arquivo(const char *path) {
    const char *ext = strrchr(path, '.');
    if (!ext) return 3; 
    if (strcasecmp(ext, ".mp3") == 0 || strcasecmp(ext, ".wav") == 0 || strcasecmp(ext, ".ogg") == 0) return 1;
    if (strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".png") == 0 || strcasecmp(ext, ".jpeg") == 0) return 2;
    return 3; 
}

static void atualizar_conteudo_omega(ArchonEngine *eng) {
    if (!eng) return;
    eng->ctx.sel_filho = 0; 
    eng->ctx.offset_omega = 0;
    if (eng->ctx.sel_pai == 1) eng->ctx.total_filhos = scan_folder(eng->cache_items);
    else eng->ctx.total_filhos = colher_apps_por_dominio(dominios_myth[eng->ctx.sel_pai]);
}

static void engine_render(ArchonEngine *eng) {
    int my, mx; getmaxyx(stdscr, my, mx);
    int h_action = 8; 
    int action_y = my - h_action - 3; 
    int mid_v = mx / 2;
    int h_panels = action_y - 2;

    erase(); 
    attron(COLOR_PAIR(CP_ACCENT));
    box(stdscr, 0, 0); 
    mvprintw(0, (mx / 2) - 14, "┤ 🏛️  ACRÓPOLE: CORE_ORCHESTRATOR ├"); 
    mvhline(action_y, 1, ACS_HLINE, mx - 2);
    mvaddch(action_y, 0, ACS_LTEE);
    mvaddch(action_y, mx - 1, ACS_RTEE);
    mvvline(2, mid_v, ACS_VLINE, h_panels);
    mvaddch(action_y, mid_v, ACS_BTEE);
    attroff(COLOR_PAIR(CP_ACCENT));

    // Renderização do Painel Esquerdo (Alpha)
    for(int i=0; i<5; i++) {
        int is_sel = (eng->ctx.active_pane == 0 && i == eng->ctx.sel_pai);
        draw_pill(2+(i*2), 2, "⌬", dominios_myth[i], is_sel);
    }

    // Renderização do Painel Direito (Omega)
    for (int i = 0; i < h_panels; i++) {
        int idx = i + eng->ctx.offset_omega;
        if (idx >= eng->ctx.total_filhos) break;
        int is_sel = (eng->ctx.active_pane == 1 && idx == eng->ctx.sel_filho);
        attron(COLOR_PAIR(is_sel ? CP_PILL_SEL : CP_DIMMED));
        if (eng->ctx.sel_pai == 1) {
            mvprintw(2+i, mid_v+2, " %s %-30.30s ", eng->cache_items[idx].is_dir ? "DIR" : "FIL", eng->cache_items[idx].name);
        } else {
            char ic_render[16], nm_render[64], cmd_render[MAX_CMD_LEN];
            obter_app_submenu_completo(idx, ic_render, nm_render, cmd_render);
            mvprintw(2+i, mid_v+2, " %s %-30.30s ", ic_render, nm_render);
        }
        attroff(COLOR_PAIR(is_sel ? CP_PILL_SEL : CP_DIMMED));
    }

      /* --- [💻] RENDERIZAÇÃO ESTÁVEL DO PROMPT (FIX DE CURSOR) --- */
    attron(COLOR_PAIR(CP_ACCENT));
    mvprintw(action_y + 1, 2, "⌬ DASH_SHELL: PROMPT_INTERACT");
    attroff(COLOR_PAIR(CP_ACCENT));
    
    int shell_focado = (eng->ctx.active_pane == 2);
    if (shell_focado) {
        attron(COLOR_PAIR(CP_PILL_SEL));
    } else {
        attron(COLOR_PAIR(CP_DIMMED));
    }
    
    // 1. Desenha o texto do prompt e o buffer de digitação
    mvprintw(action_y + 4, 2, "┌─╼ $ ");
    printw("%s", shell_buffer); 
    
    if (shell_focado) {
        attroff(COLOR_PAIR(CP_PILL_SEL));
    } else {
        attroff(COLOR_PAIR(CP_DIMMED));
    }

    // 2. DESENHA O RODAPÉ PRIMEIRO (MUDANÇA DE ORDEM CRUCIAL)
    draw_footer_keys(my); 

    // 3. SE O SHELL ESTIVER FOCADO, POSICIONA O CURSOR POR ÚLTIMO
    // Isso garante que nenhuma outra função mova o cursor de lugar antes do refresh
    if (shell_focado) {
        curs_set(1); // Força ativação física do cursor do Linux
        move(action_y + 4, 8 + shell_cursor_pos); // Fixa o ponteiro no local exato do texto
    } else {
        curs_set(0); // Oculta o cursor se estiver navegando nos painéis superiores
    }
    
    refresh(); 
}


int main(void) {
    setlocale(LC_ALL, ""); 
    ArchonEngine engine = {0};
    signal(SIGCHLD, SIG_IGN); 
    carregar_sistema(&engine); 
    init_archon_colors();      
    chdir(getenv("HOME"));
    atualizar_conteudo_omega(&engine);
    timeout(100); 

    while(!engine.should_exit) {
        int my, mx; getmaxyx(stdscr, my, mx);
        (void)mx;
        int h_panels = (my - 8 - 3) - 2;
        engine_render(&engine);

        int ch = getch();
        
        // Se a Ncurses retornar nulo no ciclo de timeout, pula mantendo o estado
        if (ch == ERR) continue;

        /* --- [⌨️] CAPTURA ATÔMICA DA DIGITAÇÃO DO PROMPT INFERIOR --- */
        if (engine.ctx.active_pane == 2) {
            if (ch == KEY_F(10)) {
                engine.should_exit = 1;
                continue;
            }
            if (ch == '\t') {
                engine.ctx.active_pane = 0;
                continue;
            }
            if (ch == KEY_UP) {
                engine.ctx.active_pane = 1;
                continue;
            }
                                            // EXECUÇÃO DO ENTER NO PROMPT (Mecânica Inteligente Anti-Piscão)
            else if (ch == 10 || ch == KEY_ENTER || ch == KEY_ENTER_L) { 
                if (shell_cursor_pos > 0) {
                    shell_buffer[shell_cursor_pos] = '\0';
                    
                    char cmd_ajustado[MAX_CMD_LEN + 64];
                    
                    // Checa se o comando digitado pertence ao grupo interativo longo
                    if (strstr(shell_buffer, "nano")   || strstr(shell_buffer, "vim")   ||
                        strstr(shell_buffer, "micro")  || strstr(shell_buffer, "btop")  ||
                        strstr(shell_buffer, "htop")   || strstr(shell_buffer, "nmtui") ||
                        strstr(shell_buffer, "links")  || strstr(shell_buffer, "msfconsole")) {
                        
                        // Envia o comando puro para ferramentas interativas
                        snprintf(cmd_ajustado, sizeof(cmd_ajustado), "%s", shell_buffer);
                    } else {
                        // Comandos instantâneos (lsblk, uname) recebem uma trava de leitura automática.
                        // O 'echo' adiciona uma quebra de linha estética e o 'read' segura o Kitty na tela.
                        snprintf(cmd_ajustado, sizeof(cmd_ajustado), "%s; echo \"\"; echo \"[Pressione ENTER para fechar a entidade]\"; read", shell_buffer);
                    }
                    
                    // Dispara o comando processado pelo barramento assíncrono do hermes.c
                    invocar_entidade(cmd_ajustado, 1);
                    
                    // Purga os buffers de forma atômica para a próxima escrita
                    memset(shell_buffer, 0, sizeof(shell_buffer));
                    shell_cursor_pos = 0;
                }
                
                // Redesenha a matriz da TUI de forma limpa
                clearok(stdscr, TRUE);
                refresh();
                continue;
            }



            // Mecânica do BACKSPACE
            if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) { 
                if (shell_cursor_pos > 0) {
                    shell_cursor_pos--;
                    shell_buffer[shell_cursor_pos] = '\0';
                }
                continue;
            }
            // Filtro de Injeção de caracteres imprimíveis
            if (ch >= 32 && ch <= 126) { 
                if (shell_cursor_pos < MAX_CMD_LEN - 1) {
                    shell_buffer[shell_cursor_pos] = (char)ch;
                    shell_cursor_pos++;
                    shell_buffer[shell_cursor_pos] = '\0';
                }
                continue;
            }
        }

        /* --- MECÂNICA DE FLUXO DOS MENUS ORIGINAIS (ALPHA / OMEGA) --- */
        switch(ch) {
            case 'q': case KEY_F(10): engine.should_exit = 1; break;
            
            case KEY_F(1): launch_terminal_app("/opt/archia/projects/protognum/bin/protognum"); break;
            case KEY_F(2): launch_terminal_app("/opt/archia/projects/archonplayer/bin/archonplayer"); break;
            case KEY_F(3): launch_terminal_app("/opt/archia/projects/zeusbrowser-tui/bin/zeusbrowser"); break;
            case KEY_F(4): launch_terminal_app("nano"); break;
            case KEY_F(5): sync_pywal(); break; 
            
            case '\t': 
                engine.ctx.active_pane = (engine.ctx.active_pane + 1) % 3;
                break;
                
            case KEY_UP:
                if (engine.ctx.active_pane == 0) {
                    if (engine.ctx.sel_pai > 0) engine.ctx.sel_pai--;
                    atualizar_conteudo_omega(&engine);
                } else if (engine.ctx.active_pane == 1) {
                    if (engine.ctx.sel_filho > 0) {
                        engine.ctx.sel_filho--;
                        if (engine.ctx.sel_filho < engine.ctx.offset_omega) engine.ctx.offset_omega = engine.ctx.sel_filho;
                    }
                }
                break;
                
            case KEY_DOWN:
                if (engine.ctx.active_pane == 0) {
                    if (engine.ctx.sel_pai < 4) engine.ctx.sel_pai++;
                    atualizar_conteudo_omega(&engine);
                } else if (engine.ctx.active_pane == 1) {
                    if (engine.ctx.sel_filho < (int)engine.ctx.total_filhos - 1) {
                        engine.ctx.sel_filho++;
                        if (engine.ctx.sel_filho >= engine.ctx.offset_omega + h_panels) engine.ctx.offset_omega++;
                    }
                }
                break;
                
            case 10: // ENTER EXECUTADO NOS MENUS SUPERIORES
                if (engine.ctx.active_pane == 0) {
                    engine.ctx.active_pane = 1;
                }
                else if (engine.ctx.active_pane == 1) {
                    if (engine.ctx.sel_pai == 1) {
                        char path_abs[MAX_PATH_LEN];
                        if (realpath(engine.cache_items[engine.ctx.sel_filho].name, path_abs)) {
                            if (engine.cache_items[engine.ctx.sel_filho].is_dir) {
                                chdir(path_abs); 
                                atualizar_conteudo_omega(&engine);
                            } else {
                                int tipo = identificar_tipo_arquivo(path_abs);
                                char cmd_local[MAX_CMD_LEN + 256]; 
                                
                                                                if (tipo == 1) {
                                    // [ÁUDIO]: Invoca o seu player externo com double fork assíncrono
                                      snprintf(cmd_local, sizeof(cmd_local), "/usr/local/bin/archonplayer \"%s\"", path_abs);
                                    invocar_entidade(cmd_local, 1);
                                } 
                                else if (tipo == 2) {
                                    // [IMAGEM]: Abre o feh de forma limpa e centralizada com double fork gráfico
                                    snprintf(cmd_local, sizeof(cmd_local), "feh --start-at \"%s\"", path_abs);
                                    invocar_entidade(cmd_local, 0);
                                } 
                                else {
                                    // [CÓDIGOS / TEXTOS / SH]: Abre o nano clonando a sessão em terminal flutuante
                                    snprintf(cmd_local, sizeof(cmd_local), "nano \"%s\"", path_abs);
                                    invocar_entidade(cmd_local, 1);
                                

                                }
                            }
                        }
                    } else {
                        char ic[16], nm[64], cmd_app[MAX_CMD_LEN];
                        obter_app_submenu_completo(engine.ctx.sel_filho, ic, nm, cmd_app);
                        if (cmd_app[0] != '\0') invocar_entidade(cmd_app, 0);
                    }
                    clearok(stdscr, TRUE); 
                }
                break;
        }
    }
    archon_shutdown(); 
    return 0;
}
