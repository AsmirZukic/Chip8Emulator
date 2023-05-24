#pragma once
#include <stdint.h>
#include <SDL2/SDL.h>
#include <cstdint>

class Engine;

class InputHandler
{
private:
  const uint8_t* keystates = SDL_GetKeyboardState( 0 );

  void onKeyDown(Engine* engine, SDL_Event* event, std::uint8_t* keys);
  void onKeyUp(SDL_Event* event, std::uint8_t* keys);

public:
  InputHandler();
  ~InputHandler(){}

	void update(Engine* engine, std::uint8_t* keys);
	bool isKeyDown( SDL_Scancode key );
};
