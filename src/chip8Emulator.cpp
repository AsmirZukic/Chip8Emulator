#include <iostream>
#include <fstream>
#include <ios>
#include <cstdint>
#include <chrono>

#include "../include/Chip8.h"
#include "../include/engine.hpp"
#include "../include/texture.hpp"

int main( int argc, char* argv[] )
{
    Chip8 emulator(argv[1]);

    int videoPitch = sizeof(emulator.video[0])*VIDEO_WIDTH;
	auto lastCycleTime = std::chrono::high_resolution_clock::now();
    auto cycleDelay = 10; 

    while( Engine::getInstance()->isRunning() )
    {
        Texture texture(64, 32);

        InputHandler::getInstance()->update(emulator.keyPad);

        auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

			emulator.Cycle();

            texture.update(emulator.video,videoPitch);
			Engine::getInstance()->render(texture.getTexture());
            Engine::getInstance()->draw();
		}



    }

    return 0;
}
