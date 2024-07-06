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
			SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
			cout << "Renderer created!" << endl;
			int result = IMG_Init(IMG_INIT_AVIF|IMG_INIT_PNG|IMG_INIT_JPG);
			cout << "IMG_Init result=" << result << endl;
			SDL_Surface* tmpSurface = IMG_Load("assets/france-flag.png");
			//SDL_Surface* tmpSurface = SDL_LoadBMP("assets/svg64.bmp");
			
			if (!tmpSurface) {
				cout << "tmpSurface = NULL" << endl;
			}
			playerTex = SDL_CreateTextureFromSurface(ren, tmpSurface);
			SDL_FreeSurface(tmpSurface);
			srcR = new SDL_Rect;
			dstR = new SDL_Rect;
			srcR->w = 30;
			srcR->h = 30;
			dstR->w = 30;
			dstR->h = 30;
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
{
	dstR->x = 100;
	dstR->y = 100;
}
void Game::render()
{
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, playerTex, NULL, dstR);
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