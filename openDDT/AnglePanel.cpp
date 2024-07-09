#include "AnglePanel.h"
#include "Game.h"
#include "SDL.h"

AnglePanel::AnglePanel(const LoaderParams* pParams) : SDLGameObject(pParams) 
{
	m_currentFrame = 0;
	m_angle = 0;
}

void AnglePanel::draw()
{
	SDLGameObject::draw();
	/* Select the color for drawing. It is set to red here. */
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 255, 0, 255);
	int startX, startY, endX, endY;
	startX = m_x + m_width / 2;
	startY = m_y + m_height;
	float radians = (float)m_angle * 3.1415f / 180.0f;//ª°∂»÷∆Ω«
	endX = startX + m_width/2 * cosf(radians);
	endY = startY - m_width/2 * sinf(radians);
	//cout << "m_angle=" << m_angle << endl;
	
	SDL_SetRenderDrawBlendMode(TheGame::Instance()->getRenderer(), SDL_BLENDMODE_NONE);
	SDL_RenderDrawLine(TheGame::Instance()->getRenderer(),startX,startY, endX,endY);
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 0, 0, 255);
}

void AnglePanel::update()
{
	m_angle++;
	if (m_angle >= 180) {
		m_angle = 0;
	}
}

void AnglePanel::clean() {}