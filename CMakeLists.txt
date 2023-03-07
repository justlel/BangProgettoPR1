cmake_minimum_required(VERSION 3.25)
project(BangProgettoPR1 C)

set(CMAKE_C_STANDARD 99)

add_executable(BangProgettoPR1 main.c partita.c mazzo.h giocatore.h carta.h mazzo.c savegame.c savegame.h)
