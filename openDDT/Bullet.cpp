#include "Bullet.h"
#include "SDL.h"

Bullet::Bullet(const LoaderParams* pParams) : SDLGameObject(pParams) {}

void Bullet::draw()
{
	SDLGameObject::draw();
}

void Bullet::update()
{
	m_y -= 1;
	m_currentFrame = (int)(SDL_GetTicks() / 100) % 4;
}

void Bullet::clean() {}