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
	bool addObject(GameObject* object);
	void removeObject();

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
			cout << "创建s_pInstance成功" << endl;
		}
		return s_pInstance;
	}
	int getShootAngle() { return m_shoot_angle; }
	int getShootForce() { return m_shoot_force; }
	void setShootAngle(int angle) { m_shoot_angle=angle; }
	void setShootForce(int force) { m_shoot_force=force; }
	void setState(State state) { m_cur_state = state; }

private:

	Game() { m_cur_state = Idle; };

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture;
	SDL_Rect m_sourceRectangle;
	SDL_Rect m_destinationRectangle;

	bool m_bRunning;
	bool m_bVectorChanged=false;
	int m_currentFrame;
	int m_shoot_angle = 0;
	int m_shoot_force = 0;
	std::vector<GameObject*> m_gameObjects;

	static Game* s_pInstance;
	State m_cur_state = Idle;
	Event m_cur_event = _event_total;
};

typedef Game TheGame;
#endif // __Game__
