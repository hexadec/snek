cmake_minimum_required(VERSION 3.13)
project(snek C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Werror -Wall -pedantic -Wextra -O3")

add_executable(snek snek.c snek.h linkedlist.c linkedlist.h screen.c screen.h debugmalloc.h fileio.c fileio.h)
target_link_libraries(snek ncursesw)