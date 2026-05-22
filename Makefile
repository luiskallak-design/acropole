
# --- NÚCLEO DO ORQUESTRADOR ACÓPOLE OS ---
TARGET   = bin/acropole
CC       = gcc

# URLs Oficiais do Olimpo (Seus Repositórios)
PROTOGNUM_GIT   = https://github.com/luiskallak-design/protognum
ZEUSBROWSER_GIT = https://github.com/luiskallak-design/zeusbrowser

# Caminhos do Sistema
PREFIX  ?= /usr/local
BINDIR  = $(PREFIX)/bin

# Matriz Cromática (Estética Acrópole Emerald)
GREEN   = \033[0;32m
CYAN    = \033[0;36m
WHITE   = \033[1;37m
BOLD    = \033[1m
RESET   = \033[0m

# Flags do Orquestrador (Com geração automática de dependências táticas .d)
CFLAGS  = -Iinclude -Wall -O3 -MMD -MP
LIBS    = -lncursesw

# Código Local do Acrópole (Mapeamento explícito mantendo toda a árvore de subpastas)
SRC     = src/main.c \
          $(wildcard src/clean/*.c) \
          $(wildcard src/launcher/*.c) \
          $(wildcard src/logic/*.c) \
          $(wildcard src/monitor/*.c)

# Espelhamento cirúrgico dos objetos e arquivos de dependência dentro de build/
OBJ     = $(patsubst src/%.c, build/%.o, $(SRC))
DEPS    = $(OBJ:.o=.d)

.PHONY: all setup modules clean install uninstall

# Compila o Acrópole local e puxa as dependências se não existirem
all: setup modules $(TARGET)
	@echo -e "\n$(GREEN)$(BOLD)⌬ [ACRÓPOLE OS] ORQUESTRADOR CONSOLIDADO COM SUCESSO!$(RESET)"
	@echo -e "$(CYAN)❱❱ Central Operacional pronta em: $(TARGET)$(RESET)\n"

# Prepara os diretórios fundamentais na raiz do projeto
setup:
	@mkdir -p bin build

# Forja automatizada da cambada usando os seus caminhos exatos de subpastas
modules:
	@echo -e "$(CYAN)⌬ RECONHECENDO MÓDULOS DO ECOSSISTEMA...$(RESET)"
	
	@if ! command -v protognum >/dev/null 2>&1; then \
		echo -e "$(WHITE)⌬ [ALERTA] Protognum ausente. Iniciando forja...$(RESET)"; \
		git clone $(PROTOGNUM_GIT) /tmp/protognum-repo && \
		cd /tmp/protognum-repo/protognum && make && sudo make install && \
		cd /tmp && rm -rf /tmp/protognum-repo; \
		echo -e "$(GREEN)⌬ [SUCESSO] Protognum integrado ao sistema!$(RESET)\n"; \
	else \
		echo -e "$(GREEN)⌬ [OK] Protognum já está ativo no sistema.$(RESET)"; \
	fi
	
	@if ! command -v zeusbrowser >/dev/null 2>&1; then \
		echo -e "$(WHITE)⌬ [ALERTA] ZeusBrowser ausente. Iniciando forja...$(RESET)"; \
		git clone $(ZEUSBROWSER_GIT) /tmp/zeusbrowser-repo && \
		cd /tmp/zeusbrowser-repo/zeusbrowser-tui && make && sudo make install && \
		cd /tmp && rm -rf /tmp/zeusbrowser-repo; \
		echo -e "$(GREEN)⌬ [SUCESSO] ZeusBrowser integrado ao sistema!$(RESET)\n"; \
	else \
		echo -e "$(GREEN)⌬ [OK] ZeusBrowser já está ativo no sistema.$(RESET)"; \
	fi

# Linkagem do Executável do Acrópole (Consolida TODOS os módulos da pasta build/)
$(TARGET): $(OBJ)
	@echo -e "$(GREEN)⌬ CONSOLIDANDO NÚCLEO OPERACIONAL: $(RESET)$@"
	@$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Compilação Individual (Garante a criação física das subpastas em build/ antes do GCC rodar)
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo -e "$(CYAN)⌬ PROCESSANDO COMPONENTE: $(RESET)$<"
	@$(CC) -c $< -o $@ $(CFLAGS)

# Inclui dinamicamente os cabeçalhos de dependência (.d) para evitar compilações desnecessárias
-include $(DEPS)

# Limpeza profunda de resquícios de compilação
clean:
	@rm -rf bin build
	@echo -e "$(GREEN)⌬ ACRÓPOLE: Código local limpo.$(RESET)"

# Instalação Global do Sistema Integrado
install: all
	@echo -e "$(CYAN)⌬ LINKANDO ORQUESTRADOR CENTRAL NO SISTEMA...$(RESET)"
	@mkdir -p $(DESTDIR)$(BINDIR)
	@install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/acropole
	@echo -e "\n$(GREEN)$(BOLD)⌬ [SUCESSO] Acrópole OS está online! Digite 'acropole' no terminal.$(RESET)"

# Desinstalação limpa de todo o ecossistema
uninstall:
	@rm -f $(DESTDIR)$(BINDIR)/acropole
	@rm -f $(DESTDIR)$(BINDIR)/protognum
	@rm -f $(DESTDIR)$(BINDIR)/zeusbrowser
	@rm -f $(DESTDIR)$(BINDIR)/archonplayer
	@echo -e "$(WHITE)⌬ Ecossistema Archon completamente removido do sistema.$(RESET)"
