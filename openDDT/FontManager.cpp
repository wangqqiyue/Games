#include "FontManager.h"

FontManager* FontManager::s_pInstance = 0;
void FontManager::draw(int angle)
{

}

void FontManager::update(State state)
{

}

bool FontManager::init()
{
	//��ʼ��SDL_ttf
	if (TTF_Init() == -1)
	{
		return false;
	}
	//���ֿ� 
	m_font = TTF_OpenFont(m_font_path.c_str(), m_font_size);
	if (!m_font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
}

void FontManager::drawText(SDL_Renderer* renderer, const std::string& text, SDL_Color color, int x, int y) {
	SDL_Surface* tmpSurface = TTF_RenderText_Solid(m_font, text.c_str(), color); 
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	SDL_Rect dstRect = { x, y, tmpSurface->w, tmpSurface->h };
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);
}