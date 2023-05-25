#include <cstdint>
#include <chrono>
#include <random>

const int VIDEO_HEIGHT = 64;
const int VIDEO_WIDTH = 32;

class Chip8
{
    public:
        std::uint8_t keyPad[16]{};
        std::uint8_t video[64*32]{};
       
        Chip8(char* path);
        void Cycle();

    private:
        std::uint8_t registers[16];
        std::uint8_t memory[4096];
        std::uint16_t index{};
        std::uint16_t pc = 0x200;
        std::uint16_t stack[16]{};
        std::uint8_t sp{};
        std::uint8_t delayTimer{};
        std::uint8_t soundTImer{};
        std::uint16_t opcode;

        std::default_random_engine randGen;
	    std::uniform_int_distribution<uint8_t> randByte;

        void loadRom(char* fileName);
        void loadFontSet();

        void OP_00E0();
        void OP_00EE();

        void OP_1nnn();
        void OP_2nnn();
        void OP_3xkk();
        void OP_4xkk();
        void OP_5xy0();
        void OP_6xkk();
        void OP_7xkk();

        void OP_8xy0();
        void OP_8xy1();
        void OP_8xy2();
        void OP_8xy3();
        void OP_8xy4();
        void OP_8xy5();
        void OP_8xy6();
        void OP_8xy7();
        void OP_8xyE();

        void OP_9xy0();

        void OP_Annn();
        void OP_Bnnn();
        void OP_Cxkk();
        void OP_Dxyn();

        void OP_Ex9E();
        void OP_ExA1();
        void OP_Fx07();
        void OP_Fx0A();
        void OP_Fx15();
        void OP_Fx18();
        void OP_Fx1E();
        void OP_Fx29();
        void OP_Fx33();
        void OP_Fx55();
        void OP_Fx65();

        void Table0();
        void Table8();
        void TableE();
        void TableF();
        void OP_NULL();

        typedef void (Chip8::*Chip8Func)();
        Chip8Func table[0xF + 1];
        Chip8Func table0[0xE + 1];
        Chip8Func table8[0xE + 1];
        Chip8Func tableE[0xE + 1];
        Chip8Func tableF[0x65 + 1];

};