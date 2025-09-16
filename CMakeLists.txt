cmake_minimum_required(VERSION 3.25)
project(pacman-pthreads-CC3086)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# conan
find_package(Curses REQUIRED)

# ejecutable
file(GLOB SOURCES "src/*.cpp" "src/other/*.cpp")
add_executable(pacman ${SOURCES}
        src/main.cpp
        src/Object.cpp
        src/PowerUp.cpp
        src/Pacman.cpp
        src/ui.cpp
        src/highscore.cpp
)

target_link_libraries(pacman
    PRIVATE
    ${CURSES_LIBRARIES}
)

target_include_directories(pacman
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)