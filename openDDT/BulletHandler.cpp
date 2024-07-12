#include "BulletHandler.h"

void BulletHandler::draw(int angle)
{
	for (Bullet& b : bullets)
	{
		b.draw(angle);
	}
}