#include <iostream>
#include <vector>
#include "Game.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "AnglePanel.h"
#include "Bullet.h"
#include "ForcePanel.h"
#include "BulletHandler.h"
#include "CollisionHandler.h"
#include "TurnHandler.h"
#include "FontManager.h"

Game* Game::s_pInstance = 0;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	// attempt to initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		int flags = 0;
		if (fullscreen)
		{
			flags = SDL_WINDOW_FULLSCREEN;
		}


		std::cout << "SDL init success\n";
		// init the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (m_pWindow != 0) // window init success
		{
			std::cout << "window creation success\n";
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			if (m_pRenderer != 0) // renderer init success
			{
				std::cout << "renderer creation success\n";
				SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
			}
			else
			{
				std::cout << "renderer init fail\n";
				return false; // renderer init fail
			}
		}
		else
		{
			std::cout << "window init fail\n";
			return false; // window init fail
		}
	}
	else
	{
		std::cout << "SDL init fail\n";
		return false; // SDL init fail
	}

	std::cout << "init success\n";
	m_bRunning = true; // everything inited successfully, start the main loop
	
	//增加人物
	if (!TheTextureManager::Instance()->load("assets/people.png", "people", m_pRenderer))
	{
		return false;
	}
	Player* character = new Player(new LoaderParams(xpos + 20, ypos + height - 300, 45, 50, "people"));
	m_gameObjects.push_back(character);
	
	//增加NPC
	if (!TheTextureManager::Instance()->load("assets/people.png", "npc", m_pRenderer))
	{
		return false;
	}
	Player* npc = new Player(new LoaderParams(xpos + width -200, ypos + height - 300, 45, 50, "npc"));
	m_gameObjects.push_back(npc);
	
	//将人物和NPC加入碰撞管理器
	TheCollisionHandler::Instance()->attachObserver(npc);
	TheCollisionHandler::Instance()->attachObserver(character);
	//将人物和NPC加入回合管理器
	TheTurnHandler::Instance()->attachObserver(character);
	TheTurnHandler::Instance()->attachObserver(npc);

	

	//增加角度盘
	if (!TheTextureManager::Instance()->load("assets/angle_panel3.jpg", "angle_panel", m_pRenderer))
	{
		return false;
	}

	m_gameObjects.push_back(new AnglePanel(new LoaderParams(xpos+70, ypos + height -200, 100, 100, "angle_panel")));

	//增加力度表
	if (!TheTextureManager::Instance()->load("assets/force_panel.png", "force_panel", m_pRenderer))
	{
		return false;
	}

	m_gameObjects.push_back(new ForcePanel(new LoaderParams(xpos + 220, ypos + height - 220, 400, 53, "force_panel")));


	//增加子弹管理类
	m_gameObjects.push_back(TheBulletHandler::Instance());

	//增加碰撞管理类
	m_gameObjects.push_back(TheCollisionHandler::Instance());

	//将回合管理器,加入游戏中
	m_gameObjects.push_back(TheTurnHandler::Instance());

	//将字体管理器加入游戏
	m_gameObjects.push_back(TheFontManager::Instance());
	return true;
}

bool Game::addObject(GameObject* object)
{
	m_gameObjects.push_back(object);
	m_bVectorChanged = true;
	return true;
}

void Game::removeObject()
{
	//删除元素
	m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
		[](const GameObject* item) { return item->needDelete; }),
		m_gameObjects.end());
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer);

	for (std::vector<GameObject*>::size_type i = 0; i != m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->draw();
	}

	SDL_RenderPresent(m_pRenderer);
}

void Game::clean()
{
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}

void Game::handleEvents()
{
	TheInputHandler::Instance()->update();
}


void Game::update()
{
	//Get Event
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
	{
		m_cur_event = _left_press;
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
	{
		m_cur_event = _right_press;
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
	{
		m_cur_event = _space_press;
	}
	else if (TheInputHandler::Instance()->isKeyRelease(SDL_SCANCODE_SPACE))
	{
		m_cur_event = _space_release;
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP))
	{
		m_cur_event = _up_press;
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN))
	{
		m_cur_event = _down_press;
	}
	else
	{
		m_cur_event = _none;
	}

	//Change state
	m_cur_state = g_transition_table[m_cur_state][m_cur_event];

	if (Exploding == m_cur_state)
	{
		TheBulletHandler::Instance()->removeBullet();
		m_cur_state = Idle;
	}

	//Notify all observers, when state changed.
	for (GameObject* go : m_gameObjects)
	{
		go->update(m_cur_state);
		if (m_bVectorChanged)
		{
			m_bVectorChanged = false;
			break;
		}
	}
	/*
	int da = 0;//角度改变量
	static int bulletNum = 0;//子弹数量
	cout << "bulletNum=" << bulletNum << endl;
	cout << "current state" << m_cur_state << endl;
	//改变状态
	switch (m_cur_state)
	{
	case Idle:
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
		{
			m_cur_state = GettingForce;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP))
		{
			m_cur_state = GettingAngle;
			da = 1;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN))
		{
			m_cur_state = GettingAngle;
			da = -1;
		}
		break;
	case GettingAngle:
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
		{
			m_cur_state = GettingForce;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP))
		{
			da = 1;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN))
		{
			da = -1;
		}
		break;
	case GettingForce:
		cout << "Getting Force" << endl;
		if (TheInputHandler::Instance()->isKeyRelease(SDL_SCANCODE_SPACE))
		{
			m_cur_state = Shooting;
		}
		break;
	case Shooting:
		cout << "Shooting.." << endl;
		break;
	default:
		break;
	}

	//执行更新
	for (std::vector<SDLGameObject*>::size_type i = 0; i != m_gameObjects.size(); i++)
	{
		std::string id = ((SDLGameObject*)m_gameObjects[i])->getTextureID();
		if (Idle == m_cur_state)
		{
			if("people" != id)
			{
				continue;
			}
		}
		if (GettingAngle == m_cur_state)
		{
			if ("angle_panel" == id)
			{
				AnglePanel* ap = ((AnglePanel*)m_gameObjects[i]);
				ap->addAngle(da);
				m_shoot_angle = ap->getAngle();
			}
			else
			{
				continue;
			}
			
		}
		if (GettingForce == m_cur_state)
		{
			if ("force_panel" == id)
			{
				ForcePanel* fp = ((ForcePanel*)m_gameObjects[i]);
				m_shoot_force = fp->getForce();
				
			}
			else
			{
				continue;
			}
		}
		if (Shooting == m_cur_state)
		{
			if ("people" == id)
			{
				cout << "shoot." << endl;
				((Player*)m_gameObjects[i])->shoot(m_shoot_angle, m_shoot_force);
				bulletNum++;
				m_cur_state = End;
			}
			else
			{
				continue;
			}
		}
		if (End == m_cur_state)
		{
			if (0 == bulletNum)
			{
				m_cur_state = Idle;
			}
			else if ("bullet" == id)
			{
				Bullet* b = (Bullet*)m_gameObjects[i];
				//cout << "b->getPosition().y=" << b->getPosition().y << endl;
				if (b->getPosition().y >= 500)
				{
					//cout << "erasing.." << endl;
					b->needDelete = true;
					//delete b;
					bulletNum--;
				}
			}
			else
			{
				continue;
			}
		}
		
		m_gameObjects[i]->update();
	}

	//删除元素
	m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
		[](const GameObject* item) { return item->needDelete; }),
		m_gameObjects.end());
		*/
}


