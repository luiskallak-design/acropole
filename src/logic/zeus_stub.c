#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "core.h"
#include "zeus.h"

/**
 * @brief Oráculo Zeus: Motor Dinâmico de Varredura de Diretórios para a Acrópole.
 * Vasculha o diretório de trabalho atual em tempo real e popula a matriz Omega.
 * 
 * [🛡️] NOTA DE ENGENHARIA: Auto-detectável e portável, ideal para o GitHub.
 */
int scan_folder(BrowseItem *items) {
    if (!items) return 0;

    // Abre o diretório de trabalho atual (.) modificado pelo chdir() do main
    DIR *d = opendir(".");
    if (!d) return 0;

    struct dirent *dir;
    int count = 0;

    while ((dir = readdir(d)) != NULL) {
        // Ignora o diretório oculto atual "." para limpar o visual da TUI,
        // mas mantém o ".." para o usuário conseguir voltar de pasta ao clicar
        if (strcmp(dir->d_name, ".") == 0) {
            continue;
        }

        // Blindagem estrita contra estouro de cache da estrutura do Oráculo
        if (count >= MAX_ITEMS) break;

        // Copia com segurança o nome do arquivo/pasta para o buffer ncurses
        strncpy(items[count].name, dir->d_name, 63);
        items[count].name[63] = '\0';

        // Coleta metadados de hardware para identificar se é um diretório legítimo
        struct stat st;
        if (stat(dir->d_name, &st) == 0) {
            items[count].is_dir = S_ISDIR(st.st_mode);
        } else {
            // Fallback baseado no descritor de tipo do sistema de arquivos
            items[count].is_dir = (dir->d_type == DT_DIR);
        }

        count++;
    }
    closedir(d);

    // Retorna a contagem exata de entidades reais encontradas na pasta atual
    return count; 
}
