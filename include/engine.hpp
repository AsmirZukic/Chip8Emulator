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

public:
  Engine();
  ~Engine();
  void render();
  void updateTexture( std::uint32_t* buffer);
  void handleInput(Engine* engine, std::uint8_t* keys);

  void close();

  bool isRunning();
};
