#include "AnglePanel.h"
#include "Game.h"
#include "SDL.h"

AnglePanel* AnglePanel::s_pInstance = 0;

AnglePanel::AnglePanel(const LoaderParams* pParams) : SDLGameObject(pParams) 
{
	m_currentFrame = 0;
	m_angle = 0;
}

void AnglePanel::draw(int angle)
{
	SDLGameObject::draw();
	/* Select the color for drawing. It is set to red here. */
	int startX, startY, endX, endY;
	startX = m_pos.x + m_width / 2;
	startY = m_pos.y + m_height/2;
	float radians = (float)angle * 3.1415f / 180.0f;//ª°∂»÷∆Ω«
	endX = startX + m_width/2 * cosf(radians);
	endY = startY - m_width/2 * sinf(radians);
//	cout << "m_angle=" << m_angle << endl;
	
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 255, 0, 255);
	SDL_SetRenderDrawBlendMode(TheGame::Instance()->getRenderer(), SDL_BLENDMODE_NONE);
	SDL_RenderDrawLine(TheGame::Instance()->getRenderer(),startX,startY, endX,endY);
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 255, 255, 255, 255);
}

void AnglePanel::update(State state)
{
	
}

void AnglePanel::clean() {}