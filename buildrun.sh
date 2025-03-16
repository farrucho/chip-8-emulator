#!/bin/bash

rm ./chip8
g++ *.cpp -lSDL2 -lSDL2_ttf -o chip8
./chip8