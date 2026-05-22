#!/bin/bash

# [🔨] DETECTOR DE DISTRO E INSTALADOR UNIVERSAL
echo -e "\033[1;33m[!] Iniciando Forja Universal DashCBash...\033[0m"

# Detectar Gerenciador de Pacotes
if [ -f /etc/arch-release ]; then
    PM="sudo pacman -Sy --needed --noconfirm"
    DEPS="sdl2 sdl2_mixer ncurses kitty"
elif [ -f /etc/debian_version ]; then
    PM="sudo apt-get update && sudo apt-get install -y"
    DEPS="libsdl2-dev libsdl2-mixer-dev libncurses5-dev kitty"
elif [ -f /etc/fedora-release ]; then
    PM="sudo dnf install -y"
    DEPS="SDL2-devel SDL2_mixer-devel ncurses-devel kitty"
else
    echo "Distro não suportada automaticamente. Instale SDL2 e Ncurses manualmente."
    exit 1
fi

echo -e "\033[1;32m[+] Instalando dependências base...\033[0m"
$PM $DEPS

# Compilação
echo -e "\033[1;32m[+] Compilando DashCBash...\033[0m"
make

echo -e "\033[1;34m[!] AVISO: Ferramentas do 'Hades-Void' (Nmap, Metasploit, etc) 
devem ser instaladas manualmente ou via repositórios específicos da sua distro.\033[0m"
