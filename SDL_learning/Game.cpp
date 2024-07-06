#include "Game.h"
Game::Game()
{}
Game::~Game()
{}
void Game::init(const char* title, int posx, int posy, int width, int height, bool fullscreen)
{
	int flag = 0;
	if (fullscreen)
	{
		flag += SDL_WINDOW_FULLSCREEN;
	}
	if (0 == SDL_Init(SDL_INIT_EVERYTHING))
	{
		win = SDL_CreateWindow(title, posx, posy,
			width, height, flag);
		if (win)
		{
			cout << "Window created!" << endl;
		}
		ren = SDL_CreateRenderer(win, -1, 0);
		if (ren)
		{
			SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
			cout << "Renderer created!" << endl;
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

}
void Game::handleEvent()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}
void Game::update()
{}
void Game::render()
{
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);
}
void Game::clean()
{
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
	cout << "Game cleaning!" << endl;
}

bool Game::running()
{
	return isRunning;
}