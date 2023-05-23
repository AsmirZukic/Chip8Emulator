#include "../include/texture.hpp"
#include "../include/renderer.hpp"


Texture::Texture(int textureWidth, int textureHeight)
{
    mTexture = SDL_CreateTexture( Renderer::getInstance()->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}

Texture::~Texture()
{
    free();
}

void Texture::free()
{
    if( mTexture )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = nullptr;
    }
}

SDL_Texture* Texture::getTexture()
{
    return mTexture;
}

void Texture::update( std::uint32_t* buffer, int pitch)
{
    SDL_UpdateTexture( mTexture, nullptr, buffer, pitch);
}

