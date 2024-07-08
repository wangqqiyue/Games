#include <string>
#include "SDL.h"

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
	m_currentFrame = 0;
}

void Player::clean() {}
