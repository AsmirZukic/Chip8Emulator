#pragma once
#include "renderer.hpp"
#include "inputHandler.hpp"
#include "texture.hpp"

class Engine  
{
private:
  bool running = false;

  static Engine* iEngine;

  Engine();
  Engine( const Engine& e2 );

  ~Engine();

public:
  static Engine* getInstance();

  virtual void render( SDL_Texture* texture );

  const bool isRunning();

  void draw();
};
