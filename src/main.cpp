#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <SDL2/SDL.h>

class Chip8 {
public:
    Chip8();
    void LoadROM(const std::string& filename);
    void EmulateCycle();
    void HandleInput();
    void UpdateTimers();
    void Render();
    bool IsRunning() const { return isRunning; }

private:
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint16_t opcode;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint16_t stack[16];
    uint16_t sp;
    uint8_t key[16];
    uint32_t gfx[64 * 32];
    uint8_t fontset[80];

    typedef void (Chip8::*OpcodeFunc)();
    OpcodeFunc opcodeTable[16];

    bool isRunning;
    std::ifstream file;

    void Initialize();
    void FetchOpcode();
    void DecodeOpcode();
    void ExecuteOpcode();

    // Opcode functions
    void Opcode0NNN();
    void Opcode00E0();
    void Opcode00EE();
    void Opcode1NNN();
    void Opcode2NNN();
    void Opcode3XNN();
    void Opcode4XNN();
    void Opcode5XY0();
    void Opcode6XNN();
    void Opcode7XNN();
    void Opcode8XY0();
    void Opcode8XY1();
    void Opcode8XY2();
    void Opcode8XY3();
    void Opcode8XY4();
    void Opcode8XY5();
    void Opcode8XY6();
    void Opcode8XY7();
    void Opcode8XYE();
    void Opcode9XY0();
    void OpcodeANNN();
    void OpcodeBNNN();
    void OpcodeCXNN();
    void OpcodeDXYN();
    void OpcodeEX9E();
    void OpcodeEXA1();
    void OpcodeFX07();
    void OpcodeFX0A();
    void OpcodeFX15();
    void OpcodeFX18();
    void OpcodeFX1E();
    void OpcodeFX29();
    void OpcodeFX33();
    void OpcodeFX55();
    void OpcodeFX65();

    friend class Chip8Display;
    void RenderPixel(int x, int y, uint8_t value);
    void ClearScreen();
};

class Chip8Display {
public:
    Chip8Display();
    ~Chip8Display();

    void Initialize();
    void Render(const Chip8& chip8);
    void HandleInput(Chip8& chip8);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};


Chip8Display::Chip8Display() : window(nullptr), renderer(nullptr) {}

Chip8Display::~Chip8Display() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void Chip8Display::Initialize() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, 64, 32);
}

void Chip8Display::Render(const Chip8& chip8) {
    SDL_RenderClear(renderer);

    // Render graphics
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            if (chip8.gfx[x + y * 64] != 0) {
                SDL_Rect rect;
                rect.x = x * 10;
                rect.y = y * 10;
                rect.w = 10;
                rect.h = 10;

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Chip8Display::HandleInput(Chip8& chip8) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            chip8.isRunning = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                // Handle key events and update the Chip8 key states accordingly
            }
        }
        else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                // Handle key events and update the Chip8 key states accordingly
            }
        }
    }
}

Chip8::Chip8() {
    Initialize();
    opcodeTable[0x0] = &Chip8::Opcode0NNN;
    opcodeTable[0x1] = &Chip8::Opcode1NNN;
    opcodeTable[0x2] = &Chip8::Opcode2NNN;
    opcodeTable[0x3] = &Chip8::Opcode3XNN;
    opcodeTable[0x4] = &Chip8::Opcode4XNN;
    opcodeTable[0x5] = &Chip8::Opcode5XY0;
    opcodeTable[0x6] = &Chip8::Opcode6XNN;
    opcodeTable[0x7] = &Chip8::Opcode7XNN;
    opcodeTable[0x8] = &Chip8::Opcode8XY0;
    opcodeTable[0x9] = &Chip8::Opcode9XY0;
    opcodeTable[0xA] = &Chip8::OpcodeANNN;
    opcodeTable[0xB] = &Chip8::OpcodeBNNN;
    opcodeTable[0xC] = &Chip8::OpcodeCXNN;
    opcodeTable[0xD] = &Chip8::OpcodeDXYN;
    opcodeTable[0xE] = &Chip8::OpcodeEX9E;
    opcodeTable[0xF] = &Chip8::OpcodeFX07;
}

void Chip8::Initialize() {
    pc = 0x200;  // Program counter starts at 0x200
    opcode = 0;
    I = 0;
    sp = 0;

    // Clear display
    memset(gfx, 0, sizeof(gfx));

    // Clear stack, registers, and memory
    memset(stack, 0, sizeof(stack));
    memset(V, 0, sizeof(V));
    memset(memory, 0, sizeof(memory));

    // Load fontset
    for (int i = 0; i < 80; ++i) {
        memory[i] = fontset[i];
    }

    // Reset timers
    delayTimer = 0;
    soundTimer = 0;

    // Seed random number generator
    std::srand(std::time(nullptr));

    isRunning = true;
}

void Chip8::LoadROM(const std::string& filename) {
    file.open(filename, std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streampos size = file.tellg();
        char* buffer = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (int i = 0; i < size; ++i) {
            memory[0x200 + i] = buffer[i];
        }

        delete[] buffer;
    }
}

void Chip8::EmulateCycle() {
    FetchOpcode();
    DecodeOpcode();
    ExecuteOpcode();
    UpdateTimers();
    Render();
    HandleInput();
}

void Chip8::FetchOpcode() {
    opcode = memory[pc] << 8 | memory[pc + 1];
}

void Chip8::DecodeOpcode() {
    (this->*opcodeTable[(opcode & 0xF000) >> 12])();
}

void Chip8::ExecuteOpcode() {
    pc += 2;
}

void Chip8::HandleInput() {
    // Handle input events
}

void Chip8::UpdateTimers() {
    if (delayTimer > 0) {
        --delayTimer;
    }

    if (soundTimer > 0) {
        if (soundTimer == 1) {
            // Beep sound
        }
        --soundTimer;
    }
}

void Chip8::Render() {
    // Render graphics
}

void Chip8::Opcode0NNN() {
    // Ignored
}

void Chip8::Opcode1NNN() {
    uint16_t address = opcode & 0x0FFF;
    pc = address;
}

void Chip8::Opcode2NNN() {
    stack[sp] = pc;
    ++sp;
    uint16_t address = opcode & 0x0FFF;
    pc = address;
}

void Chip8::Opcode3XNN() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t value = opcode & 0x00FF;
    if (V[x] == value) {
        pc += 2;
    }
}

void Chip8::Opcode4XNN() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t value = opcode & 0x00FF;
    if (V[x] != value) {
        pc += 2;
    }
}

void Chip8::Opcode5XY0() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (V[x] == V[y]) {
        pc += 2;
    }
}

void Chip8::Opcode6XNN() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t value = opcode & 0x00FF;
    V[x] = value;
}

void Chip8::Opcode7XNN() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t value = opcode & 0x00FF;
    V[x] += value;
}

void Chip8::Opcode8XY0() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] = V[y];
}

void Chip8::Opcode8XY1() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] |= V[y];
}

void Chip8::Opcode8XY2() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] &= V[y];
}

void Chip8::Opcode8XY3() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] ^= V[y];
}

void Chip8::Opcode8XY4() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint16_t sum = V[x] + V[y];
    V[0xF] = sum > 0xFF ? 1 : 0;
    V[x] = sum & 0xFF;
}

void Chip8::Opcode8XY5() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[0xF] = V[x] > V[y] ? 1 : 0;
    V[x] -= V[y];
}

void Chip8::Opcode8XY6() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[0xF] = V[x] & 0x1;
    V[x] >>= 1;
}

void Chip8::Opcode8XY7() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[0xF] = V[y] > V[x] ? 1 : 0;
    V[x] = V[y] - V[x];
}

void Chip8::Opcode8XYE() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[0xF] = V[x] >> 7;
    V[x] <<= 1;
}

void Chip8::Opcode9XY0() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (V[x] != V[y]) {
        pc += 2;
    }
}

void Chip8::OpcodeANNN() {
    uint16_t address = opcode & 0x0FFF;
    I = address;
}

void Chip8::OpcodeBNNN() {
    uint16_t address = opcode & 0x0FFF;
    pc = V[0] + address;
}

void Chip8::OpcodeCXNN() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t value = opcode & 0x00FF;
    V[x] = std::rand() & value;
}

void Chip8::RenderPixel(int x, int y, uint8_t value) {
    if (gfx[x + y * 64] == 1 && value == 1) {
        V[0xF] = 1;
    }
    gfx[x + y * 64] ^= value;
}

void Chip8::OpcodeDXYN() {
    uint8_t x = V[(opcode & 0x0F00) >> 8];
    uint8_t y = V[(opcode & 0x00F0) >> 4];
    uint8_t height = opcode & 0x000F;
    uint8_t pixel;

    V[0xF] = 0;
    for (int yline = 0; yline < height; ++yline) {
        pixel = memory[I + yline];
        for (int xline = 0; xline < 8; ++xline) {
            if ((pixel & (0x80 >> xline)) != 0) {
                RenderPixel(x + xline, y + yline, 1);
            }
        }
    }
}

void Chip8::OpcodeEX9E() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    if (key[V[x]]) {
        pc += 2;
    }
}

void Chip8::OpcodeEXA1() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    if (!key[V[x]]) {
        pc += 2;
    }
}

void Chip8::OpcodeFX07() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[x] = delayTimer;
}

void Chip8::OpcodeFX0A() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    bool keyPress = false;
    for (int i = 0; i < 16; i++) {
        if (key[i]) {
            V[x] = i;
            keyPress = true;
        }
    }
    if (!keyPress) {
        pc -= 2;
    }
}

void Chip8::OpcodeFX15() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    delayTimer = V[x];
}

void Chip8::OpcodeFX18() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    soundTimer = V[x];
}

void Chip8::OpcodeFX1E() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    I += V[x];
}

void Chip8::OpcodeFX29() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    I = V[x] * 0x5;
}

void Chip8::OpcodeFX33() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    memory[I] = V[x] / 100;
    memory[I + 1] = (V[x] / 10) % 10;
    memory[I + 2] = (V[x] % 100) % 10;
}

void Chip8::OpcodeFX55() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (int i = 0; i <= x; ++i) {
        memory[I + i] = V[i];
    }
}

void Chip8::OpcodeFX65() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (int i = 0; i <= x; ++i) {
        V[i] = memory[I + i];
    }
}

void Chip8::ClearScreen() {
    memset(gfx, 0, sizeof(gfx));
}

int main() {
    Chip8 chip8;
    Chip8Display display;

    chip8.LoadROM("ROMS/test_opcode.ch8");
    display.Initialize();

    while (chip8.IsRunning()) {
        chip8.EmulateCycle();
        display.Render(chip8);
        display.HandleInput(chip8);
    }

    return 0;
}

