#include <string>
#include "SDL.h"
#include "Game.h"
#include "InputHandler.h"
#include "Player.h"
#include "TextureManager.h"
#include "Bullet.h"

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
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
	{

		TheTextureManager::Instance()->load("assets/bullet.png", "bullet", TheGame::Instance()->getRenderer());

		Bullet * b = new Bullet(new LoaderParams(m_x, m_y-50, 64, 128, "bullet"));

		
		TheGame::Instance()->addObject(b);
		
	}
	

}

void Player::clean() {}
