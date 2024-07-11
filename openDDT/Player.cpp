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

void Player::update(State state)
{
	if(GoingLeft == state)
	{
		m_currentFrame = 0;
		m_direction = LEFT;
		m_pos.x -= 1;
	}
	if (GoingRight == state)
	{
		m_currentFrame = 1;
		m_direction = RIGHT;
		m_pos.x += 1;
	}

}

void Player::clean() {}

void Player::shoot(int angle,int force)
{
	TheTextureManager::Instance()->load("assets/bullet.png", "bullet", TheGame::Instance()->getRenderer());

	Bullet* b = new Bullet(new LoaderParams(m_pos.x, m_pos.y , 64, 128, "bullet"));
	b->init(angle, force);
	//cout << "shoot m_pos=" << m_pos << endl;
	TheGame::Instance()->addObject(b);
}