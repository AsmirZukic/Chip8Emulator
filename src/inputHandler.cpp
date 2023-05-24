#include "../include/inputHandler.hpp"
#include "../include/engine.hpp"

InputHandler::InputHandler(){}

void InputHandler::update( Engine* engine, std::uint8_t* keys)
{
	SDL_Event event;

	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
		case SDL_QUIT:
			engine->close();
			break;

		case SDL_KEYDOWN:
			onKeyDown( engine, &event, keys );
			break;

		case SDL_KEYUP:
			onKeyUp( &event, keys);
			break;

		default:
			break;
		}
	}
}

void InputHandler::onKeyDown(Engine* engine, SDL_Event* event, std::uint8_t* keys)
{
	switch (event->key.keysym.sym)
	{
		case SDLK_ESCAPE:
		{
			engine->close();
		} break;

		case SDLK_x:
		{
			keys[0] = 1;
		} break;

		case SDLK_1:
		{
			keys[1] = 1;
		} break;

		case SDLK_2:
		{
			keys[2] = 1;
		} break;

		case SDLK_3:
		{
			keys[3] = 1;
		} break;

		case SDLK_q:
		{
			keys[4] = 1;
		} break;

		case SDLK_w:
		{
			keys[5] = 1;
		} break;

		case SDLK_e:
		{
			keys[6] = 1;
		} break;

		case SDLK_a:
		{
			keys[7] = 1;
		} break;

		case SDLK_s:
		{
			keys[8] = 1;
		} break;

		case SDLK_d:
		{
			keys[9] = 1;
		} break;

		case SDLK_z:
		{
			keys[0xA] = 1;
		} break;

		case SDLK_c:
		{
			keys[0xB] = 1;
		} break;

		case SDLK_4:
		{
			keys[0xC] = 1;
		} break;

		case SDLK_r:
		{
			keys[0xD] = 1;
		} break;

		case SDLK_f:
		{
			keys[0xE] = 1;
		} break;

		case SDLK_v:
		{
			keys[0xF] = 1;
		} break;
	}
}

void InputHandler::onKeyUp(SDL_Event* event, std::uint8_t* keys)
{
	switch (event->key.keysym.sym)
	{
		case SDLK_x:
		{
			keys[0] = 0;
		} break;

		case SDLK_1:
		{
			keys[1] = 0;
		} break;

		case SDLK_2:
		{
			keys[2] = 0;
		} break;

		case SDLK_3:
		{
			keys[3] = 0;
		} break;

		case SDLK_q:
		{
			keys[4] = 0;
		} break;

		case SDLK_w:
		{
			keys[5] = 0;
		} break;

		case SDLK_e:
		{
			keys[6] = 0;
		} break;

		case SDLK_a:
		{
			keys[7] = 0;
		} break;

		case SDLK_s:
		{
			keys[8] = 0;
		} break;

		case SDLK_d:
		{
			keys[9] = 0;
		} break;

		case SDLK_z:
		{
			keys[0xA] = 0;
		} break;

		case SDLK_c:
		{
			keys[0xB] = 0;
		} break;

		case SDLK_4:
		{
			keys[0xC] = 0;
		} break;

		case SDLK_r:
		{
			keys[0xD] = 0;
		} break;

		case SDLK_f:
		{
			keys[0xE] = 0;
		} break;

		case SDLK_v:
		{
			keys[0xF] = 0;
		} break;
	}
}

bool InputHandler::isKeyDown( SDL_Scancode key )
{
	if ( keystates != 0 )
	{
		if ( keystates[key] == 1 )
			return true;
		else
			return false;
	}
	return false;
}
