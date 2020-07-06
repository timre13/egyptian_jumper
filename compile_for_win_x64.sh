#!/bin/sh

x86_64-w64-mingw32-g++ *.cpp -std=c++17 -o3 -I .h -lSDL2 -o egyptian_jumper.exe
