#include "BulletHandler.h"
#include "CollisionHandler.h"
#include "Game.h"

BulletHandler* BulletHandler::s_pInstance = 0;
void BulletHandler::draw(int angle)
{
	for (Bullet* b : m_bullets)
	{
		b->draw(angle);
	}
}

void  BulletHandler::update(State state)
{
	if (Flying == state)
	{
		for (Bullet* b : m_bullets)
		{
			b->update(state);
			if (TheCollisionHandler::Instance()->checkCollisionAll(b))
			{
				TheGame::Instance()->setState(Exploding);
			}
		}
	}

}

void BulletHandler::addBullet(Bullet* b)
{
	m_bullets.push_back(b);
}

void BulletHandler::removeBullet()
{
	//É¾³ýÔªËØ
	m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
		[](const GameObject* item) { return item->needDelete; }),
		m_bullets.end());
}