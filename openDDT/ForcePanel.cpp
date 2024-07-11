#include "ForcePanel.h"
#include "Game.h"
#include "SDL.h"

ForcePanel::ForcePanel(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	m_currentFrame = 0;
	m_angle = 0;
}

void ForcePanel::draw(int angle)
{
	SDLGameObject::draw();

	int startX, startY, endX, endY;
	startX = m_pos.x + m_width * m_force / 100;
	startY = m_pos.y;
	endX = startX;
	endY = startY + m_height;

	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 255, 0, 255);
	SDL_SetRenderDrawBlendMode(TheGame::Instance()->getRenderer(), SDL_BLENDMODE_NONE);
	SDL_RenderDrawLine(TheGame::Instance()->getRenderer(), startX, startY, endX, endY);
	//SDL_RenderFillRect
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 255, 255, 255, 255);
}

void ForcePanel::update()
{
	static int df = 1;
	if (m_force > 100)
	{
		df = -1;
	}
	if (m_force < 0)
	{
		df = 1;
	}
	m_force += df;
	cout << "force=" << m_force << endl;
}

void ForcePanel::clean() {}