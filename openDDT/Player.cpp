#include <string>
#include "SDL.h"
#include "InputHandler.h"
#include "Player.h"

Player::Player(const LoaderParams* pParams) : SDLGameObject(pParams) {}

void Player::turn(Direction d)
{
	m_direction = d;
}
void Player::draw()
{
	SDLGameObject::draw();
}

void Player::update()
{
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
	{
		m_currentFrame = 0;
		m_direction = LEFT;
		m_x -= 1;
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
	{
		m_currentFrame = 1;
		m_direction = RIGHT;
		m_x += 1;
	}
	

}

void Player::clean() {}
