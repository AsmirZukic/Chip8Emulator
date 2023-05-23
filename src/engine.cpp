#include "../include/engine.hpp"
#include <iostream>
#include <stdexcept>

Engine* Engine::iEngine = nullptr;

Engine::Engine()
{
  try
  {
    //If the window is equal to nullptr
    if( !( Window::getInstance()->getWindow() ) )
    {
      throw "Could not create window";
    }
    else
    {
      //If the renderer is equal to nullptr
      if( !( Renderer::getInstance()->getRenderer() ) )
        throw "Could not create Renderer";

      //Else the engine is running
      running = true;
    }
  } catch ( const char* msg )
  {
    std::cerr << msg << '\n';
    running = false;
  }
}

Engine::~Engine()
{
  running = false;

  Window::getInstance()->close();
  Renderer::getInstance()->close();
}


void Engine::render(SDL_Texture* texture)
{
  SDL_RenderClear( Renderer::getInstance()->getRenderer() );
  SDL_RenderCopy(Renderer::getInstance()->getRenderer(), texture, nullptr, nullptr);
}

const bool Engine::isRunning()
{
  return Window::getInstance()->isOpen() && Renderer::getInstance()->isCreated() && running;
}

void Engine::draw()
{
  SDL_RenderPresent( Renderer::getInstance()->getRenderer() );
}

Engine* Engine::getInstance()
{
  if( !iEngine )
    iEngine = new Engine();

  return iEngine;
}
