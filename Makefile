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

# Flags do Orquestrador
CFLAGS  = -Iinclude -Wall -O3
LIBS    = -lncursesw

# Código Local do Acrópole
SRC     = $(wildcard src/*.c)
OBJ     = $(patsubst src/%.c, obj/%.o, $(SRC))

.PHONY: all setup modules clean install uninstall

# Compila o Acrópole local e puxa as dependências se não existirem
all: setup modules $(TARGET)
	@echo -e "\n$(GREEN)$(BOLD)⌬ [ACRÓPOLE OS] ORQUESTRADOR CONSOLIDADO COM SUCESSO!$(RESET)"
	@echo -e "$(CYAN)❱❱ Central Operacional pronta em: $(TARGET)$(RESET)\n"

setup:
	@mkdir -p bin obj

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

# Linkagem do Executável do Acrópole
$(TARGET): $(OBJ)
	@$(CC) $(OBJ) -o $(TARGET) $(LIBS)

obj/%.o: src/%.c
	@$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@rm -rf bin obj
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
