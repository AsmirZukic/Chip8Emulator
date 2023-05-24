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
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = registers[Vx] % VIDEO_HEIGHT;
	uint8_t yPos = registers[Vy] % VIDEO_WIDTH;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[index + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &video[(yPos + col) * VIDEO_HEIGHT + (xPos + row)];

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