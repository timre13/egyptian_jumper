#!/bin/sh

i686-w64-mingw32-c++ *.cpp -std=c++17 -I ~/SDL2_win_x86/include -L ~/SDL2_win_x86/lib -lSDL2 -lSDL2main -static-libgcc -static-libstdc++ -o egyptian_jumper.exe

cp ~/SDL2_win_x86/bin/SDL2.dll .
