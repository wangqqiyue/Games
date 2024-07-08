#ifndef __Game__
#define __Game__

#include <vector>
#include <iostream>
#include "SDL.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Global.h"
using std::cout;
using std::endl;

class Game
{
public:

	~Game() {}

	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void render();
	void update();
	void handleEvents();
	void clean();

	bool running() { return m_bRunning; }
	void quit() { m_bRunning = false; }
	SDL_Renderer* getRenderer() const { return m_pRenderer; }

	static Game* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new Game();
			cout << "����s_pInstance�ɹ�" << endl;
		}
		return s_pInstance;
	}

private:

	Game() {};

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture;
	SDL_Rect m_sourceRectangle;
	SDL_Rect m_destinationRectangle;

	bool m_bRunning;
	int m_currentFrame;

	std::vector<GameObject*> m_gameObjects;

	static Game* s_pInstance;
};

typedef Game TheGame;
#endif // __Game__
