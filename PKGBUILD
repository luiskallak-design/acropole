# ⌬ [ACRÓPOLE-CORE] - SUPREMO ORQUESTRADOR DO ECOSSISTEMA ARCHON
# Maintainer: Archon Elite <luiskallak-design@gmail.com>

pkgname=acropole-tui-git
pkgver=5.0.0
pkgrel=1
pkgdesc="Acrópole v5.0 - Orquestrador Supremo TUI focado em Telemetria e Gestão de Sistema"
arch=('x86_64')
url="https://github.com"
license=('GPL3')

# --- DEPENDÊNCIAS DE EXECUÇÃO (ARSENAL UNIFICADO) ---
_deps_base=('ncurses' 'nsxiv' 'kitty' 'qterminal' 'fastfetch')
_deps_portal=('tor-router' 'transmission-gtk' 'chromium' 'torbrowser-launcher' 'links' 'zeusbrowser-git' 'archonplayer-git')
_deps_oracle=('lxappearance' 'gparted' 'btop')
_deps_ares=('geany' 'micro' 'vim' 'nano' 'pcmanfm' 'gimp' 'vlc' 'flameshot')

# Hades convertido: Removido BlackArch / Inserida Telemetria Visual
# Nota: 'tiptop' pode requerer AUR (tiptop-git) dependendo do repositório
_deps_hades=('bmon' 'glances' 'mtr' 'httping' 'tiptop') 

# Hefesto limpo: Removidas as duplicatas que já existem no Ares e Portal
_deps_hefesto=('htop' 'xarchiver' 'zathura' 'abiword' 'gnumeric')

depends=("${_deps_base[@]}" "${_deps_portal[@]}" "${_deps_oracle[@]}" 
         "${_deps_ares[@]}" "${_deps_hades[@]}" "${_deps_hefesto[@]}")

makedepends=('git' 'gcc' 'make')
provides=('acropole')
conflicts=('acropole')

# Puxa direto do seu repositório oficial do GitHub
source=("git+${url}.git")
sha256sums=('SKIP')

build() {
  cd "acropole-tui"
  echo -e "\033[0;35m⌬ INICIANDO COMPILAÇÃO DO CORE SUPREMO...\033[0m"
  make
}

package() {
  cd "acropole-tui"
  echo -e "\033[0;36m⌬ INSTALANDO ORQUESTRADOR SUPREMO NO SISTEMA...\033[0m"
  
  # Corrigido de build/ para bin/ de acordo com a saída do seu Makefile!
  install -Dm755 bin/acropole "$pkgdir/usr/bin/acropole"
}
