#pragma once
#include "SDL.h"
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
};

