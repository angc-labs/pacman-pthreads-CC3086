# pacman-pthreads-CC3086

## Dependencias

1. Instalar conan con python 
```bash
pip install conan
```

2. Instalar cmake y ncurses 

- `Ubuntu`
```bash
mkdir build
mkdir bin
sudo apt install cmake
sudo apt install libncurses5-dev libncursesw5-dev
```

- `Fedora`
```bash
sudo dnf install cmake
sudo dnf install libncurses5-dev libncursesw5-dev
```

3. Instalar dependencias con conan
```bash
conan install . --output-folder=build --build=missing -s build_type=Release
```

4. Configuración con CMake
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

5. Compilación 

```bash
cmake --build bin
```

O instalar desde el archivo de configuracion:

```bash
chmod +x setup-ubuntu.sh
./setup-ubuntu.sh
```