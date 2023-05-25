#include <iostream>
#include <fstream>
#include <ios>
#include <cstdint>
#include <chrono>

#include "../include/Chip8.h"
#include "../include/engine.hpp"

int main( int argc, char* argv[] )
{
    Chip8 emulator(argv[1]);

    int videoPitch = sizeof(emulator.video[0])* VIDEO_WIDTH;
	auto lastCycleTime = std::chrono::high_resolution_clock::now();
    auto cycleDelay = 10; 

    Engine engine;

    while( engine.isRunning() )
    {
        engine.handleInput(&engine, emulator.keyPad);

        auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();


        if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

			emulator.Cycle();

            engine.updateTexture(emulator.video, videoPitch);

			engine.render();
		}

    }

    return 0;
}
