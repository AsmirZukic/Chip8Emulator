#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <cstring>
#include "Chip8.h"

Chip8::Chip8(char* path):randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    pc = 0x200;

    loadFontSet();
    loadRom(path);
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U); 

}

void Chip8::loadFontSet()
{
    std::uint16_t fontSetStartAddress = 0x50;
    std::uint16_t fontsetSize = 80;

    std::uint8_t fontset[fontsetSize] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for(auto i = 0; i < fontSetStartAddress; i++)
    {
        memory[fontSetStartAddress + i] = fontset[i];
    }

}

void Chip8::loadRom(char* filename)
{
    std::fstream file(filename, std::ios::in | std::ios::binary | std::ios::ate );

    if(!file.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    const std::streampos START_ADDRESS = 200;
    std::streampos size = file.tellg() + START_ADDRESS;
    std::uint8_t* buffer = new std::uint8_t[size];

    file.seekg(0, std::ios::beg);
    file.read((char*)buffer, size);
    file.close();

    for(auto i=0; i < size; i++)
    {
        memory[(int)START_ADDRESS + i] = buffer[i];
    }

    delete[] buffer;
}

void Chip8::OP_00E0()
{
	memset(video, 0, sizeof(video));
}

void Chip8::OP_00EE()
{
	--sp;
	pc = stack[sp];
}

void Chip8::OP_1nnn()
{
    std::uint16_t address = opcode & 0x0FFFu;

    pc = address;
}

void Chip8::OP_2nnn()
{
    std::uint16_t address = opcode & 0x0FFFu;

    stack[sp] = pc;
    sp++;
    pc = address;
}

void Chip8::OP_3xkk()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t byte = opcode & 0x00FFu;

    if(registers[registerVx] == byte)
    {
        pc += 2;
    }
}

void Chip8::OP_4xkk()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t byte = opcode & 0x00FFu;

    if(registers[registerVx] != byte)
    {
        pc += 2;
    }
}

void Chip8::OP_5xy0()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t registerVy = (opcode & 0x00F0u) >> 4u;

    if(registers[registerVx] == registers[registerVy])
    {
        pc += 2;
    }
}

void Chip8::OP_6xkk()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t byte = opcode & 0x00FFu;

    registers[registerVx] = byte;
}

void Chip8::OP_7xkk()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t byte = opcode & 0x00FFu;

    registers[registerVx] += byte;
}

void Chip8::OP_8xy0()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t registerVy = (opcode & 0x00F0u) >> 4u;

    registers[registerVx] = registers[registerVy];
}

void Chip8::OP_8xy1()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t registerVy = (opcode & 0x00F0u) >> 4u;

    registers[registerVx] |= registers[registerVy]; 
}

void Chip8::OP_8xy2()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t registerVy = (opcode & 0x00F0u) >> 4u;

    registers[registerVx] &= registers[registerVy]; 
}

void Chip8::OP_8xy3()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t registerVy = (opcode & 0x00F0u) >> 4u;

    registers[registerVx] ^= registers[registerVy]; 
}

void Chip8::OP_8xy4()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t registerVy = (opcode & 0x0F00u) >> 4u;

    std::uint16_t sum = registers[registerVx] + registers[registerVy];

    if( sum > 255 )
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[registerVx] = sum & 0xFFu;
}

void Chip8::OP_8xy5()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t registerVy = (opcode & 0x0F00u) >> 4u;   

    if( registers[registerVx] < registers[registerVy])
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[registerVx] -= registers[registerVy]; 

}

void Chip8::OP_8xy6()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;

    registers[0xF] = registers[registerVx] & 0x1u;

    registers[registerVx] >>= 1;    
}

void Chip8::OP_8xy7()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t registerVy = (opcode & 0x0F00u) >> 4u;

    if( registers[registerVy] > registers[registerVx])
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[registerVx] = registers[registerVy] - registers[registerVx];    
}

void Chip8::OP_8xyE()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u; 

   // Save MSB in VF
	registers[0xF] = (registers[registerVx] & 0x80u) >> 7u;

	registers[registerVx] <<= 1;
}
