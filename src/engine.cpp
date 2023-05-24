#include "../include/engine.hpp"
#include <iostream>
#include <stdexcept>

Engine::Engine()
{
  mIsRunning = true;
  if( !(SDL_INIT_VIDEO < 0) )
  {
    mWindow = SDL_CreateWindow( "Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_RESIZABLE );
    if( !mWindow )
      close();

    else
    {
      mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
      if( !mRenderer )
        close();
    }
  }

  mTexture = SDL_CreateTexture( mRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

Engine::~Engine()
{
  close();
}

void Engine::close()
{
    SDL_DestroyWindow( mWindow );
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyTexture(mTexture);

    mWindow = nullptr;
    mRenderer = nullptr;
    mTexture = nullptr;

    mIsRunning = false;

    SDL_Quit();
}

void Engine::render()
{
  SDL_RenderClear( mRenderer );
  SDL_RenderCopy(mRenderer, mTexture, nullptr, nullptr);
  SDL_RenderPresent( mRenderer);
}

void Engine::updateTexture( std::uint32_t* buffer, int pitch)
{
    SDL_UpdateTexture( mTexture, nullptr, buffer, pitch);
}

void Engine::handleInput(Engine* engine, std::uint8_t* keys)
{
    mInputHandler.update(engine, keys);
}

bool Engine::isRunning()
{
    return mIsRunning;
}
