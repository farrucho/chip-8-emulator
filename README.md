# CHIP-8 Emulator

In the 90s, CHIP-8 interpreters began emerging for graphing calculators, with notable examples like CHIP-48 and SCHIP for the HP-48. I successfully developed a fully optimized CHIP-8 emulator in C++, capable of
running and debugging any given ROM file. This project passes with success chip8-test-suite and references can be find below this file.


## Gallery

![tetris](https://github.com/user-attachments/assets/1b3110ff-c0a1-44fb-a920-ec25f59ff535)

![pong](https://github.com/user-attachments/assets/cee4babc-06bd-4a0d-a4cb-73c92d719798)

## KeyBoard Input

```text
1 2 3 C        1 2 3 4
4 5 6 D   ->   Q W E R
7 8 9 E        A S D F
A 0 B F        Z X C V
```


## Build

Requires SDL2 and SDL2_ttf. 

`sh buildrun.sh`



## References: 
    - http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
    - https://austinmorlan.com/posts/chip8_emulator/
    - https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
    - https://github.com/kripod/chip8-roms
    - https://github.com/Timendus/chip8-test-suite
    - Emulator Development Discord Server

## License:
This project is licensed under the MIT license    

