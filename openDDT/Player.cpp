#include <string>
#include "SDL.h"
#include "Game.h"
#include "InputHandler.h"
#include "Player.h"
#include "TextureManager.h"
#include "Bullet.h"
#include "BulletHandler.h"

Player::Player(const LoaderParams* pParams) : SDLGameObject(pParams) {}

void Player::turn(Direction d)
{
	m_direction = d;
}
void Player::draw(int angle)
{
	SDLGameObject::draw();
	//绘制血条和体力条
	SDL_Rect rect;
	rect.w = m_width*m_hp/100;
	rect.h = m_width / 6;
	rect.x = m_pos.x;
	rect.y = m_pos.y - rect.h;
	
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(),   // 渲染器
		255,     // 红
		0,     // 绿
		0,     // 蓝
		255);     // 透明值
	SDL_SetRenderDrawBlendMode(TheGame::Instance()->getRenderer(), SDL_BLENDMODE_NONE);
	SDL_RenderFillRect(TheGame::Instance()->getRenderer(),&rect);
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(),255,255,255,255);
	
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
	if (Shooting == state)
	{
		shoot(TheGame::Instance()->getShootAngle(), TheGame::Instance()->getShootForce());
		TheGame::Instance()->setState(Flying);
	}

}

void Player::clean() {}

void Player::shoot(int angle,int force)
{
	TheTextureManager::Instance()->load("assets/bullet.png", "bullet", TheGame::Instance()->getRenderer());

	Bullet* b = new Bullet(new LoaderParams(m_pos.x, m_pos.y , 64, 128, "bullet"));
	b->init(angle, force);
	cout << "shoot angle=" << angle << endl;
	cout << "shoot force=" << force << endl;
	TheBulletHandler::Instance()->addBullet(b);
}