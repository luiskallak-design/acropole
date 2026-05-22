#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "core.h"

// Estrutura para persistência do cálculo de CPU entre os frames do Main
static struct { uint64_t idle, total; } last_cpu = {0, 0};

/**
 * @brief Telemetria de Memória: Lê direto do kernel (/proc/meminfo)
 */
void ler_ram(char *buffer) {
    if (!buffer) return;
    char buf[2048];
    int fd = open("/proc/meminfo", O_RDONLY);
    if (fd < 0) { 
        strncpy(buffer, "OFFLINE", 31); // Alinhado com o tamanho [32] da struct no core.h
        return; 
    }
    
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (n <= 0) return;
    buf[n] = '\0';

    uint64_t total = 0, avail = 0;
    char *p;
    
    if ((p = strstr(buf, "MemTotal:"))) sscanf(p, "MemTotal: %lu", &total);
    if ((p = strstr(buf, "MemAvailable:"))) sscanf(p, "MemAvailable: %lu", &avail);

    // Formatação: Exibe Uso/Total em GB (Ex: 1.5G/8G)
    snprintf(buffer, 32, "%.1fG/%.0fG", 
             (double)(total - avail) / 1048576.0, 
             (double)total / 1048576.0);
}

/**
 * @brief Telemetria de CPU: Cálculo diferencial de Jiffies
 */
void ler_cpu(char *buffer) {
    if (!buffer) return;
    char buf[1024];
    int fd = open("/proc/stat", O_RDONLY);
    if (fd < 0) { 
        strncpy(buffer, "0%", 15); // Alinhado com o tamanho [16] da struct no core.h
        return; 
    }
    
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (n <= 0) return;
    buf[n] = '\0';

    uint64_t u, ni, s, id, io, irq, sirq;
    // Captura os tempos do processador
    if (sscanf(buf, "cpu %lu %lu %lu %lu %lu %lu %lu", &u, &ni, &s, &id, &io, &irq, &sirq) < 7) return;

    uint64_t cur_id = id + io;
    uint64_t cur_tot = cur_id + u + ni + s + irq + sirq;

    // Cálculo diferencial (Delta) para precisão em tempo real
    uint64_t d_tot = cur_tot - last_cpu.total;
    uint64_t d_id = cur_id - last_cpu.idle;

    last_cpu.total = cur_tot;
    last_cpu.idle = cur_id;

    // Evita divisão por zero na primeira execução
    if (d_tot > 0) {
        uint64_t percent = (d_tot - d_id) * 100 / d_tot;
        snprintf(buffer, 16, "%u%%", (unsigned int)percent); 
 // Alinhado com o tamanho [16] da struct no core.h
    } else {
        strncpy(buffer, "...", 15);
    }
}
