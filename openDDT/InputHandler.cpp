#include <iostream>
#include <vector>

#include "InputHandler.h"
#include "Game.h"

InputHandler* InputHandler::s_pInstance = 0;

void InputHandler::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;

		case SDL_KEYDOWN:
			m_keyDown = true;
			onKeyDown();
			break;
		case SDL_KEYUP:
			m_keyDown = false;
			onKeyUp();
			break;

		default:
			break;
		}
	}
}

void InputHandler::onKeyDown()
{
	m_keystates = SDL_GetKeyboardState(0);
}

void InputHandler::onKeyUp()
{
	m_keystates = SDL_GetKeyboardState(0);
}

bool InputHandler::isKeyDown(SDL_Scancode key)
{
	if (m_keystates != 0)
	{
		if (m_keystates[key] == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool InputHandler::isKeyPress(SDL_Scancode key)
{
	if (m_keystates != 0)
	{
		if (m_keyDown && m_keystates[key])
		{
			return true;
		}
	}

	return false;
}
bool InputHandler::isKeyRelease(SDL_Scancode key)
{
	if (m_keystates != 0)
	{
		if (!m_keyDown && !m_keystates[key])
		{
			return true;
		}
	}

	return false;
}