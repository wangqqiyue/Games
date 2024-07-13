#pragma once
#include "GameObject.h"
#include "SDL_ttf.h"
#include <iostream>
using std::string;

class FontManager :
    public GameObject
{
public:
	virtual void draw(int angle = 0);
	virtual void update(State state);
	virtual void clean() {};
	void drawText(SDL_Renderer* renderer, const std::string& text, SDL_Color color, int x, int y);
	//µ¥ÀýÄ£Ê½
	static FontManager* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new FontManager();
			return s_pInstance;
		}
		return s_pInstance;
	}

	bool init();



private:
	FontManager() {}
	static FontManager* s_pInstance;
	TTF_Font* m_font;
	string m_font_path="fonts/times.ttf";
	int m_font_size = 18;

};

