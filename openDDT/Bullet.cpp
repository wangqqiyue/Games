#include "Bullet.h"
#include "SDL.h"
#include <iostream>
#include "Game.h"
using std::cout;
using std::endl;

Bullet::Bullet(const LoaderParams* pParams) : SDLGameObject(pParams) 
{ 
	
}

void Bullet::init(int angle, int force)
{
	m_angle = angle;
	//cout << "set angle=" << angle << endl;
	float radians = (float)m_angle * PI / 180.0f;//弧度制角
	float speed = MAX_SPEED * force/100;//初始速度值
	m_v.x = speed * cosf(radians);
	m_v.y = -speed * sinf(radians);
	
}


void Bullet::draw(int angle)
{
	//static SDL_Point* ps=new SDL_Point[10000];
	//static int i = 0;
	//cout << "m_width=" << m_width << endl;
	//ps[i].x = m_pos.x + m_width / 2;
	//ps[i].y = m_pos.y + m_height / 2;
	//i++;
	//cout << "bullet m_pos=" << m_pos << endl;
	//调试
	//SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 255, 0, 255);
	//SDL_SetRenderDrawBlendMode(TheGame::Instance()->getRenderer(), SDL_BLENDMODE_NONE);
	//SDL_RenderDrawPoints(TheGame::Instance()->getRenderer(), ps,i);
	//SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 0, 0, 255);
	TextureManager::Instance()->drawFrame(m_textureID, m_pos.x, m_pos.y, m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), SDL_FLIP_NONE, (90-m_angle));
}

void Bullet::update(State state)
{
	if (Flying == state)
	{
		m_v.y += 0.01;
		m_pos += m_v;

		m_currentFrame = (int)(SDL_GetTicks() / 100) % 4;

		float radians = acosf(m_v.x / m_v.mag());//求反余弦值,但只能返回[0-PI]区间的值
		m_angle = radians * 180.0f / PI;
		//需要根据速度分量的符号,判断角度
		if (m_v.y > 0)
		{
			m_angle = 2 * PI - m_angle;
		}

		if (m_pos.y > 500)
		{
			TheGame::Instance()->setState(Exploding);
			needDelete = true;
		}
	}

	
}

void Bullet::clean() {}