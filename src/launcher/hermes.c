#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include "core.h"

void invocar_entidade(const char *cmd, int forcar_float) {
    if (!cmd || strlen(cmd) == 0 || strcmp(cmd, " ") == 0) return;

    pid_t pid = fork();
    if (pid < 0) return; 

    if (pid == 0) {
        // [1] Criação de Nova Sessão para o intermediário
        setsid(); 

        if (fork() == 0) {
            // [2] Criação de Nova Sessão para o Neto (Desvinculo Total)
            setsid();

            // Lista de ferramentas sincronizada
            const char *cli_tools[] = {
                "nmtui", "btop", "htop", "journalctl", "fastfetch", "tor-router", "ncpamixer", 
                "archonplayer", "zeusbrowser", "vim", "nano", "micro", "links", 
                "bmon", "glances", "mtr", "httping", "tiptop", "sudo", "su"
            };

            int precisa_terminal = 0;
            int total_tools = sizeof(cli_tools) / sizeof(char *);
            for(int i = 0; i < total_tools; i++) {
                if (strstr(cmd, cli_tools[i])) { precisa_terminal = 1; break; }
            }

            // [EXECUÇÃO]
            if (strstr(cmd, "kitty") || strstr(cmd, "bash")) {
                execlp("sh", "sh", "-c", cmd, (char *)NULL);
            } 
            else if (precisa_terminal || forcar_float) {
                if (strstr(cmd, "nano")  || strstr(cmd, "links") || 
                    strstr(cmd, "btop")  || strstr(cmd, "htop")  || 
                    strstr(cmd, "journalctl")) {
                    
                    execlp("kitty", "kitty", "--class", "ARCHON_LARGE", "--title", "ARCHON_TASK", "sh", "-c", cmd, (char *)NULL);
                } 
                else if (strstr(cmd, "fastfetch")) {
                    execlp("kitty", "kitty", "--class", "ARCHON_FLOAT", "--title", "ARCHON_INFO", "--hold", "bash", "--norc", "-c", cmd, (char *)NULL);
                }
                // --- TRATAMENTO DE ALVO INTERATIVO (MTR / HTTPING) ---
                else if (strstr(cmd, "mtr") || strstr(cmd, "httping")) {
                    char cmd_dinamico[MAX_CMD_LEN + 512]; // CORRIGIDO: Agora é um array seguro
                    
                    if (strstr(cmd, "mtr")) {
                        snprintf(cmd_dinamico, sizeof(cmd_dinamico),
                                 "HOST=$(gum input --placeholder 'Alvo para Rota (ex: google.com)'); "
                                 "if [ ! -z \"$HOST\" ]; then mtr --user-mode \"$HOST\"; "
                                 "else echo 'Nenhum host fornecido.'; fi; "
                                 "gum input --placeholder 'Pressione Enter para fechar...' --readonly");
                    } else {
                        snprintf(cmd_dinamico, sizeof(cmd_dinamico),
                                 "HOST=$(gum input --placeholder 'URL para Latencia (ex: https://google.com)'); "
                                 "if [ ! -z \"$HOST\" ]; then httping -K -g \"$HOST\"; "
                                 "else echo 'Nenhum host fornecido.'; fi; "
                                 "gum input --placeholder 'Pressione Enter para fechar...' --readonly");
                    }
                    
                    execlp("kitty", "kitty", "--class", "ARCHON_FLOAT", "--title", "ARCHON_TELEMETRIA", "sh", "-c", cmd_dinamico, (char *)NULL);
                }
                // --- RETENÇÃO PREVENTIVA PARA OUTRAS TELEMETRIAS (bmon, glances, tiptop) ---
                else if (strstr(cmd, "bmon") || strstr(cmd, "glances") || strstr(cmd, "tiptop")) {
                    char cmd_retencao[MAX_CMD_LEN + 256]; // CORRIGIDO: Agora é um array seguro
                    snprintf(cmd_retencao, sizeof(cmd_retencao), "%s; gum input --placeholder 'Retornar ao Hades (Enter)...' --readonly", cmd);
                    execlp("kitty", "kitty", "--class", "ARCHON_FLOAT", "--title", "ARCHON_TELEMETRIA", "sh", "-c", cmd_retencao, (char *)NULL);
                }
                else {
                    execlp("kitty", "kitty", "--class", "ARCHON_FLOAT", "--title", "ARCHON_ENTITY", "sh", "-c", cmd, (char *)NULL);
                }
            }
            else {
                // MODO GUI: Blindagem Absoluta contra rastro visual e processos fantasmas
                int fd = open("/dev/null", O_RDWR);
                if (fd != -1) {
                    dup2(fd, 0); dup2(fd, 1); dup2(fd, 2);
                    close(fd);
                }

                char cmd_final[MAX_CMD_LEN + 256];
                snprintf(cmd_final, sizeof(cmd_final), 
                         "export DISPLAY=:0; exec nohup %s >/dev/null 2>&1", cmd);
                
                execlp("sh", "sh", "-f", "-c", cmd_final, (char *)NULL);
            }

            _exit(1);
        }
        _exit(0);
    }

    // Retorno não bloqueante para manter os 60 FPS fluidos
    waitpid(pid, NULL, WNOHANG); 
}
