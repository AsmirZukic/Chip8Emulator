#pragma once

#include <SDL2/SDL.h>
#include <string>

class Texture
{
private:
    SDL_Texture* mTexture;
public:
    Texture(int textureWidth, int textureHeight);
    ~Texture();

    void update(std::uint32_t* buffer, int pitch);
    void free();
    SDL_Texture* getTexture();
};
