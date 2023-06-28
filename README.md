# Chip-8 Emulator

A Chip-8 emulator implemented in C++ using SDL2 for graphics and input handling.

## Introduction

This project aims to emulate the Chip-8 virtual machine, allowing users to run and play Chip-8 games on their modern systems. The emulator utilizes the SDL2 library for handling graphics and input, providing a platform-independent solution.

## Features

- Emulation of the Chip-8 virtual machine
- Support for loading Chip-8 ROMs
- Rendering graphics using a 64x32 pixel display
- Handling keyboard input for game controls
- Sound timer emulation
- Collision detection for sprite drawing
- Interactive user interface

## Requirements

- C++ compiler with C++11 support
- SDL2 library

## Usage

1. Clone the repository:

   ```shell
   git clone <repository_url>
   ```

2. Compile the source code:

   ```shell
   make all
   ```

3. Run the emulator:

   ```shell
   ./chip8 /path/to/rom.ch8
   ```

4. Enjoy playing Chip-8 games!

## Controls

The Chip-8 keypad is emulated using the following keys:

```
Chip-8 Keypad      Keyboard
+-+-+-+-+-+       +-+-+-+-+
|1|2|3|C|       |1|2|3|4|
+-+-+-+-+-+  =>   +-+-+-+-+
|4|5|6|D|       |Q|W|E|R|
+-+-+-+-+-+       +-+-+-+-+
|7|8|9|E|       |A|S|D|F|
+-+-+-+-+-+       +-+-+-+-+
|A|0|B|F|       |Z|X|C|V|
+-+-+-+-+-+       +-+-+-+-+
```

## Contributions

Contributions to this project are welcome. If you find any issues or have ideas for improvements, feel free to open an issue or submit a pull request.


## Acknowledgments

- [John Earnest](https://github.com/JohnEarnest) for his comprehensive Chip-8 technical reference and test ROMs.
- The creators and contributors of the SDL2 library for providing a powerful and cross-platform multimedia library.