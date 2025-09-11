#!/bin/bash

set -e
echo "PROYECTO 1-PACMAN-SETUP"

sudo apt install python3-pip

if ! command -v conan &> /dev/null; then
    echo "INSTALANDO CONAN"
    pip install --user conan
fi
echo "INSTALANDO CMAKE Y NCURSES..."
sudo apt install -y cmake libncurses5-dev libncursesw5-dev

echo "INSTALANDO DEPENDENCIAS..."
conan install . --output-folder=build --build=missing -s build_type=Release

echo "CONFIGURACION CON CMAKE"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

echo "COMPILACION..."
mkdir bin
cmake --build bin

echo "LISTO"
