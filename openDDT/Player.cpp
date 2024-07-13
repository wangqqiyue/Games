#include <string>
#include "SDL.h"
#include "Game.h"
#include "InputHandler.h"
#include "Player.h"
#include "TextureManager.h"
#include "Bullet.h"
#include "BulletHandler.h"
#include "FontManager.h"
#include "AnglePanel.h"

Player::Player(const LoaderParams* pParams) : SDLGameObject(pParams) {}

void Player::setMyTurn(bool isMyTurn)
{
	m_myTurn = isMyTurn;
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
	//绘制血条
	SDL_RenderFillRect(TheGame::Instance()->getRenderer(),&rect);
	//绘制红三角
	if (m_myTurn)
	{
		SDL_Point triangle[4];
		triangle[0].x = m_pos.x + m_width / 2;
		triangle[0].y = m_pos.y - 40;
		triangle[1].x = triangle[0].x - 15;
		triangle[1].y = triangle[0].y - 30;
		triangle[2].x = triangle[0].x + 15;
		triangle[2].y = triangle[0].y - 30;
		triangle[3] = triangle[0];

		SDL_RenderDrawLines(TheGame::Instance()->getRenderer(), triangle, 4);
	}
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(),255,255,255,255);
	
	std::string hp_str = std::to_string(m_hp);
	//绘制血条文字
	TheFontManager::Instance()->drawText(TheGame::Instance()->getRenderer(), hp_str, {255,0,0,255},
		m_pos.x, m_pos.y - rect.h * 4);

	if (m_myTurn)
	{
		//绘制角度盘
		TheAnglePanel::Instance()->draw(m_angle + m_shoot_angle);
	}

	
}

void Player::update(State state)
{
	if (m_myTurn)
	{
		if (GoingLeft == state)
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
		if (TurningUp == state)
		{
			m_shoot_angle++;
		}
		if (TurningDown == state)
		{
			m_shoot_angle--;
		}
		if (Shooting == state)
		{
			shoot(m_angle+m_shoot_angle, TheGame::Instance()->getShootForce());
			TheGame::Instance()->setState(Flying);
		}
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
	m_myTurn = false;
}

void Player::onCollision()
{
	m_hp -= 10;
}