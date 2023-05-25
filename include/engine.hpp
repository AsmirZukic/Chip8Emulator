#pragma once
#include "inputHandler.hpp"

#include <SDL2/SDL.h>

class Engine  
{
private:
  SDL_Window* mWindow = nullptr;
  SDL_Renderer* mRenderer = nullptr;
  SDL_Texture* mTexture = nullptr;

  InputHandler mInputHandler;

  bool mIsRunning = false;

  std::uint32_t videoBuffer[64*32]{};



public:
  Engine();
  ~Engine();
  void render();
  void updateTexture( std::uint8_t* buffer, int pitch);
  void handleInput(Engine* engine, std::uint8_t* keys);

  void close();

  bool isRunning();
};
