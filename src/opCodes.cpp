#include <cstdint>
#include <cstring>

#include "../include/Chip8.h"


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
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t byte = opcode & 0x00FFu;

    if(registers[x] == byte)
    {
        pc += 2;
    }
}

void Chip8::OP_4xkk()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t byte = opcode & 0x00FFu;

    if(registers[x] != byte)
    {
        pc += 2;
    }
}

void Chip8::OP_5xy0()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t y = (opcode & 0x00F0u) >> 4u;

    if(registers[x] == registers[y])
    {
        pc += 2;
    }
}

void Chip8::OP_6xkk()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t byte = opcode & 0x00FFu;

    registers[x] = byte;
}

void Chip8::OP_7xkk()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t byte = opcode & 0x00FFu;

    registers[x] += byte;
}

void Chip8::OP_8xy0()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t y = (opcode & 0x00F0u) >> 4u;

    registers[x] = registers[y];
}

void Chip8::OP_8xy1()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t y = (opcode & 0x00F0u) >> 4u;

    registers[x] |= registers[y]; 
}

void Chip8::OP_8xy2()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t y = (opcode & 0x00F0u) >> 4u;

    registers[x] &= registers[y]; 
}

void Chip8::OP_8xy3()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t y = (opcode & 0x00F0u) >> 4u;

    registers[x] ^= registers[y]; 
}

void Chip8::OP_8xy4()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t y = (opcode & 0x0F00u) >> 4u;

    std::uint16_t sum = registers[x] + registers[y];

    if( sum > 255 )
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[x] = sum & 0xFFu;
}

void Chip8::OP_8xy5()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t y = (opcode & 0x0F00u) >> 4u;   

    int temp = 0;

    if( registers[x] < registers[y])
        temp = 1;
    else
        temp = 0;

    registers[x] -= registers[y]; 

    registers[0xF] = temp;

}

void Chip8::OP_8xy6()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;

    std::uint8_t temp = registers[x] & 0x1u;

    registers[x] >>= 1;    

    registers[0xF] = temp;
}

void Chip8::OP_8xy7()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t y = (opcode & 0x0F00u) >> 4u;

    int temp = 0;

    if( registers[y] > registers[x])
        temp = 1;
    else
        temp = 0;

    registers[x] = registers[y] - registers[x];   

    registers[0xF] = temp; 
}

void Chip8::OP_8xyE()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;

    std::uint8_t temp = registers[x] >> 7u;

	registers[x] <<= 1;

    registers[0xF] = temp;
}

void Chip8::OP_9xy0()
{
    std::uint8_t x = (opcode & 0x0F00) >> 8u;
    std::uint8_t y = (opcode & 0x00F0) >> 4u;

    if( registers[x] != registers[y] )
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
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    std::uint8_t byte = opcode & 0x00FFu;

    registers[x] = randByte(randGen) & byte;
}


void Chip8::OP_Dxyn()
{
	std::uint8_t height = opcode & 0x000Fu;
	std::uint8_t x = (opcode & 0x0F00u) >> 8u;
	std::uint8_t y = (opcode & 0x00F0u) >> 4u;

    std::uint8_t xPos = registers[x] % 64;
    std::uint8_t yPos = registers[y] % 32; 	
	
    registers[0xF] = 0;

    for ( int row = 0; row < height; row++)
	{
        std::uint8_t spriteByte = memory[index + row];

        //Start from MSB
        int spritebit=7;

        for (int col = xPos; col < ( xPos + 8 ) && col < 64; col++)
        {
            int getRow = col / 8;     
            int getByte = col % 8;     
            
            std::uint8_t srcbit = (spriteByte >> spritebit) & 0x1;
            
            if (srcbit)
            {
                std::uint8_t *videoByte_p = &video[ (row +yPos) * (64/8) + getRow];
                std::uint8_t videoByte = *videoByte_p;
                std::uint8_t bitMask = (0x80 >> getByte);
                std::uint8_t videoBit = videoByte & bitMask;

                srcbit <<= (7-getByte);
                
                if (srcbit & videoBit)
                    registers[0xF] = 1;
                
                videoBit ^= srcbit;
                
                videoByte = (videoByte & ~bitMask) | videoBit;

                *videoByte_p = videoByte;
            }
            spritebit--;
        }
    }
}


void Chip8::OP_Ex9E()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
    
    std::uint8_t key = registers[x];

    if(keyPad[key])
        pc += 2;
}

void Chip8::OP_ExA1()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;

    std::uint8_t key = registers[x];

    if( !keyPad[key] )
        pc += 2;
}

void Chip8::OP_Fx07()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;

    registers[x] = delayTimer;
}

void Chip8::OP_Fx0A()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;

    bool keyFound = false;

    for( auto i = 0; i < 15; i++)
    {
        if( keyPad[i])
        {
            registers[x] = i;
            break;
        }

    }
	if(!keyFound)
        pc -= 2;
}

void Chip8::OP_Fx15()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;

    delayTimer = registers[x];
}

void Chip8::OP_Fx18()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;

    soundTImer = registers[x];
}

void Chip8::OP_Fx1E()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;

    index += registers[x];
}

void Chip8::OP_Fx29()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;   
    std::uint8_t digit = registers[x];

    auto FontSetStartAddress = 0x50;
    auto FontSetWidth = 5;

    index = FontSetStartAddress + (FontSetWidth * digit);
}

void Chip8::OP_Fx33()
{
    std::uint8_t x = (opcode & 0x0F00u) >> 8u;
	std::uint8_t value = registers[x];

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
    std::uint8_t x = ( opcode & 0x0F00u) >> 8u;

    for( auto i = 0; i <= x; i++)
    {
        memory[index + i] = registers[i];
    }
}

void Chip8::OP_Fx65()
{
    std::uint8_t x = ( opcode & 0x0F00u) >> 8u;

    for( auto i = 0; i <= x; i++)
    {
        registers[i] = memory[index + i];
    }
}