#include "Game.h"
int main(int argc,char* argv[])
{
	Game *game = new Game();
	game->init("BirthEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, true);
	while (game->running())
	{
		game->handleEvent();
		game->update();
		game->render();
	}
	game->clean();
	return 0;
}