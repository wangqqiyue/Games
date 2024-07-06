#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
using std::cout;
using std::endl;

class Game
{
public:
	Game();
	~Game();
	void init(const char* title, int posx, int posy, int width, int height, bool fullscreen);
	void handleEvent();
	void update();
	void render();
	void clean();

	bool running();

private:
	bool isRunning;
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Texture* playerTex;
	SDL_Rect* srcR;
	SDL_Rect* dstR;
};

