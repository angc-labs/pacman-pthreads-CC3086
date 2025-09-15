cmake_minimum_required(VERSION 3.25)
project(pacman-pthreads-CC3086)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# conan
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# ejecutable
file(GLOB SOURCES "src/*.cpp" "src/other/*.cpp")
add_executable(pacman ${SOURCES}
        src/Object.cpp
        headers/Object.h
        src/PowerUp.cpp
        headers/PowerUp.h
        src/Pacman.cpp
        headers/Pacman.h
        src/Punto.cpp
        headers/Punto.h
        src/Pared.cpp
        headers/Pared.h
        headers/Mapa.h
        src/GameManager.cpp
        headers/GameManager.h
)

target_link_libraries(pacman
    PRIVATE
    ${CURSES_LIBRARIES}
)
