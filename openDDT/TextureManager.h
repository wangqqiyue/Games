#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <map>
class TextureManager;

typedef TextureManager TheTextureManager;

class TextureManager
{
public:
	//����ģʽ
	static TextureManager* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new TextureManager();
			return s_pInstance;
		}
		return s_pInstance;
	}
	bool load(std::string fileName, std::string id,
		SDL_Renderer* pRenderer);
	// draw
	void draw(std::string id, int x, int y, int width, int
		height, SDL_Renderer* pRenderer, SDL_RendererFlip flip =
		SDL_FLIP_NONE);
	// drawframe
	void drawFrame(std::string id, int x, int y, int width, int
		height, int currentRow, int currentFrame, SDL_Renderer*
		pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
private:
	TextureManager() {}
	std::map<std::string, SDL_Texture*> m_textureMap;
	static TextureManager* s_pInstance;
};

