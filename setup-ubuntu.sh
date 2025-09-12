#!/bin/bash

set -e
echo "PROYECTO 1-PACMAN-SETUP"

sudo apt install -y python3-pip
sudo apt install -y python3.12-venv

# virtual env
python3 -m venv venv
source ./venv/bin/activate
mkdir -p build
mkdir -p bin

# Instalar Conan si no está disponible
if ! command -v conan &> /dev/null; then
    echo "INSTALANDO CONAN"
    pip install conan
fi

# dev profile for conan
if ! conan profile list | grep -q "^dev$"; then
    echo "CREANDO PROFILE dev..."
    conan profile detect --name dev
fi

echo "INSTALANDO CMAKE Y NCURSES..."
sudo apt install -y cmake libncurses5-dev libncursesw5-dev

echo "INSTALANDO DEPENDENCIAS..."
conan install . --output-folder=build --build=missing -s build_type=Release --profile:build=dev

echo "CONFIGURACION CON CMAKE"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=bin

echo "COMPILACION..."
cmake --build build

echo "LISTO"
