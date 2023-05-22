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

    // Set up function pointer table
    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::OP_1nnn;
    table[0x2] = &Chip8::OP_2nnn;
    table[0x3] = &Chip8::OP_3xkk;
    table[0x4] = &Chip8::OP_4xkk;
    table[0x5] = &Chip8::OP_5xy0;
    table[0x6] = &Chip8::OP_6xkk;
    table[0x7] = &Chip8::OP_7xkk;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::OP_9xy0;
    table[0xA] = &Chip8::OP_Annn;
    table[0xB] = &Chip8::OP_Bnnn;
    table[0xC] = &Chip8::OP_Cxkk;
    table[0xD] = &Chip8::OP_Dxyn;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;

    for (size_t i = 0; i <= 0xE; i++)
    {
        table0[i] = &Chip8::OP_NULL;
        table8[i] = &Chip8::OP_NULL;
        tableE[i] = &Chip8::OP_NULL;
    }

    table0[0x0] = &Chip8::OP_00E0;
    table0[0xE] = &Chip8::OP_00EE;

    table8[0x0] = &Chip8::OP_8xy0;
    table8[0x1] = &Chip8::OP_8xy1;
    table8[0x2] = &Chip8::OP_8xy2;
    table8[0x3] = &Chip8::OP_8xy3;
    table8[0x4] = &Chip8::OP_8xy4;
    table8[0x5] = &Chip8::OP_8xy5;
    table8[0x6] = &Chip8::OP_8xy6;
    table8[0x7] = &Chip8::OP_8xy7;
    table8[0xE] = &Chip8::OP_8xyE;

    tableE[0x1] = &Chip8::OP_ExA1;
    tableE[0xE] = &Chip8::OP_Ex9E;

    for (size_t i = 0; i <= 0x65; i++)
    {
        tableF[i] = &Chip8::OP_NULL;
    }

    tableF[0x07] = &Chip8::OP_Fx07;
    tableF[0x0A] = &Chip8::OP_Fx0A;
    tableF[0x15] = &Chip8::OP_Fx15;
    tableF[0x18] = &Chip8::OP_Fx18;
    tableF[0x1E] = &Chip8::OP_Fx1E;
    tableF[0x29] = &Chip8::OP_Fx29;
    tableF[0x33] = &Chip8::OP_Fx33;
    tableF[0x55] = &Chip8::OP_Fx55;
    tableF[0x65] = &Chip8::OP_Fx65;

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

void Chip8::OP_9xy0()
{
    std::uint8_t registerVx = (opcode & 0x0F00) >> 8u;
    std::uint8_t registerVy = (opcode & 0x00F0) >> 4u;

    if( registers[registerVx] != registers[registerVy] )
        pc += 2;
}

void Chip8::OP_Annn()
{
    std::uint16_t address = opcode & 0x0FFFu;

    index = address;
}

void Chip8::OP_Bnnn()
{
    std::uint16_t address = opcode & 0x0FFFu;

    pc = address + registers[0];
}

void Chip8::OP_Cxkk()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    std::uint8_t byte = opcode & 0x00FFu;

    registers[registerVx] = randByte(randGen) & byte;
}

void Chip8::OP_Dxyn()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
	std::uint8_t registerVy = (opcode & 0x00F0u) >> 4u;
	std::uint8_t height = opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries
	std::uint8_t xPos = registers[registerVx] % VIDEO_WIDTH;
	std::uint8_t yPos = registers[registerVy] % VIDEO_HEIGHT;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		std::uint8_t spriteByte = memory[index + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			std::uint8_t spritePixel = spriteByte & (0x80u >> col);
			std::uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel)
			{
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					registers[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void Chip8::OP_Ex9E()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
    
    std::uint8_t key = registers[registerVx];

    if(keyPad[key])
        pc += 2;
}

void Chip8::OP_ExA1()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;

    std::uint8_t key = registers[registerVx];

    if( !keyPad[key] )
        pc += 2;
}

void Chip8::OP_Fx07()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;

    registers[registerVx] = delayTimer;
}

void Chip8::OP_Fx0A()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;

	if (keyPad[0])
	{
		registers[registerVx] = 0;
	}
	else if (keyPad[1])
	{
		registers[registerVx] = 1;
	}
	else if (keyPad[2])
	{
		registers[registerVx] = 2;
	}
	else if (keyPad[3])
	{
		registers[registerVx] = 3;
	}
	else if (keyPad[4])
	{
		registers[registerVx] = 4;
	}
	else if (keyPad[5])
	{
		registers[registerVx] = 5;
	}
	else if (keyPad[6])
	{
		registers[registerVx] = 6;
	}
	else if (keyPad[7])
	{
		registers[registerVx] = 7;
	}
	else if (keyPad[8])
	{
		registers[registerVx] = 8;
	}
	else if (keyPad[9])
	{
		registers[registerVx] = 9;
	}
	else if (keyPad[10])
	{
		registers[registerVx] = 10;
	}
	else if (keyPad[11])
	{
		registers[registerVx] = 11;
	}
	else if (keyPad[12])
	{
		registers[registerVx] = 12;
	}
	else if (keyPad[13])
	{
		registers[registerVx] = 13;
	}
	else if (keyPad[14])
	{
		registers[registerVx] = 14;
	}
	else if (keyPad[15])
	{
		registers[registerVx] = 15;
	}
	else
	{
		pc -= 2;
	}
}

void Chip8::OP_Fx15()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;

    delayTimer = registers[registerVx];
}

void Chip8::OP_Fx18()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;

    soundTImer = registers[registerVx];
}

void Chip8::OP_Fx1E()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;

    index += registers[registerVx];
}

void Chip8::OP_Fx29()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;   
    std::uint8_t digit = registers[registerVx];

    auto FontSetStartAddress = 0x50;
    auto FontSetWidth = 5;

    index = FontSetStartAddress + (FontSetWidth * digit);
}

void Chip8::OP_Fx33()
{
    std::uint8_t registerVx = (opcode & 0x0F00u) >> 8u;
	std::uint8_t value = registers[registerVx];

	// Ones-place
	memory[index + 2] = value % 10;
	value /= 10;

	// Tens-place
	memory[index + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	memory[index] = value % 10;
}

void Chip8::OP_Fx55()
{
    std::uint8_t registerVx = ( opcode & 0x0F00u) >> 8u;

    for( auto i = 0; i <= registerVx; i++)
    {
        memory[index + i] = registers[i];
    }
}

void Chip8::OP_Fx65()
{
    std::uint8_t registerVx = ( opcode & 0x0F00u) >> 8u;

    for( auto i = 0; i <= registerVx; i++)
    {
        registers[i] = memory[index + i];
    }
}

void Chip8::Cycle()
{
    // Fetch
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	// Increment the PC before we execute anything
	pc += 2;

	// Decode and Execute
	((*this).*(table[(opcode & 0xF000u) >> 12u]))();

	// Decrement the delay timer if it's been set
	if (delayTimer > 0)
	{
		--delayTimer;
	}

	// Decrement the sound timer if it's been set
	if (soundTImer > 0)
	{
		--soundTImer;
	}
}
