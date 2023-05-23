#pragma once
#include <stdint.h>
#include <SDL2/SDL.h>
#include <cstdint>

class InputHandler
{
private:
  InputHandler();
  InputHandler( const InputHandler& h2 );
  ~InputHandler(){}

  static InputHandler* iHandler;

  // Keyboard //
  const uint8_t* keystates = SDL_GetKeyboardState( 0 );

  void onKeyDown(SDL_Event* event, std::uint8_t* keys);
  void onKeyUp(SDL_Event* event, std::uint8_t* keys);

public:

	static InputHandler* getInstance();

	void update(std::uint8_t* keys);

	// Keyboard //
	bool isKeyDown( SDL_Scancode key );
};
