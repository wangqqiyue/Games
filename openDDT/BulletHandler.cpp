#include "BulletHandler.h"

BulletHandler* BulletHandler::s_pInstance = 0;
void BulletHandler::draw(int angle)
{
	for (Bullet* b : bullets)
	{
		b->draw(angle);
	}
}

void  BulletHandler::update(State state)
{
	for (Bullet* b : bullets)
	{
		b->update(state);
	}
}

void BulletHandler::addBullet(Bullet* b)
{
	bullets.push_back(b);
}