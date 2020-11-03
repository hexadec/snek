cmake_minimum_required(VERSION 3.16)
project(snek C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Werror -Wall -pedantic -Wextra -Wno-unused-function")

add_executable(snek snek.c snek.h linkedlist.c linkedlist.h)
target_link_libraries(snek ncurses)