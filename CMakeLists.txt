cmake_minimum_required(VERSION 3.25)
project(pacman-pthreads-CC3086)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# conan
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# ejecutable
add_executable(pacman src/main.cpp)

target_link_libraries(pacman
    PRIVATE
    ${CURSES_LIBRARIES}
)