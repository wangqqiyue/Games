#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Game.h"

SDLGameObject::SDLGameObject(const LoaderParams* pParams) : GameObject(pParams)
{
	m_pos.x = pParams->getX();
	m_pos.y = pParams->getY();
	m_width = pParams->getWidth();
	m_height = pParams->getHeight();
	m_textureID = pParams->getTextureID();

	m_currentRow = 1;
	m_currentFrame = 1;
}

void SDLGameObject::draw(int angle)
{
	TextureManager::Instance()->drawFrame(m_textureID, m_pos.x, m_pos.y, m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), SDL_FLIP_NONE, angle);
}
