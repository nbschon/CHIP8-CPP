# CHIP8-CPP

A basic CHIP-8 emulator / interpreter written in C++ and SDL.

## About
In the next thrilling installment of "me doing something in 
C++ /SDL," I took a stab at creating a CHIP-8 emulator. 

In various programming / emulator development communities
online, this is widely considered a rite of passage before
moving on to more (relatively) advanced systems, like the
Game Boy (Color) or Nintendo Entertainment System. 

I've always been fascinated with retro gaming, emulation,
etc., so I wanted to try and tackle a more trivial 
implementation of something like this before diving
into the wild world of the 6502, NES PPU, and cartridge 
mappers.

## Usage

Simply run the program with the path to a ROM file as the
first argument

```
./Chip8 path/to/rom.ch8
```

## Features

Load ROMs into memory and run them as they would've appeared
in the 70's on something like the COSMAC VIP.

The CHIP-8 was controlled with a keypad, with the
following layout:

| `1` | `2` | `3` | `C` |
|-----|-----|-----|-----|
| `4` | `5` | `6` | `D` |
| `7` | `8` | `9` | `E` |
| `A` | `0` | `B` | `F` |

Unfortunately, not everyone owns a hexadecimal keypad,
so this is mapped onto the keyboard like so:

| `1` | `2` | `3` | `4` |
|-----|-----|-----|-----|
| `Q` | `W` | `E` | `R` |
| `A` | `S` | `D` | `F` |
| `Z` | `X` | `C` | `V` |

Beyond that, there's not too much to it; Load up a ROM,
press Escape to exit, and you're off to the races.

## Future Functionality

Some things I'd like to add in the future would be 
a way to change the FPS in real time, since different
CHIP-8 implementations ran at different speeds.

Additionally, there's also the SUPER-CHIP, which extends
the capabilities of the original interpreter with things 
like high-res mode, and display scrolling, so adding this
to enhance compatibility could be a neat thing to add.