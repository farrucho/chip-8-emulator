#!/bin/bash

rm ./a.out
g++ *.cpp -lSDL2 -lSDL2_ttf
./a.out