#include <iostream>
#include <vector>
#include "Game.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "AnglePanel.h"
#include "Bullet.h"

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
				SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
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

	if (!TheTextureManager::Instance()->load("assets/people.png", "people", m_pRenderer))
	{
		return false;
	}

	m_gameObjects.push_back(new Player(new LoaderParams(xpos + 20, ypos + height - 300, 45, 50, "people")));


	if (!TheTextureManager::Instance()->load("assets/angle_panel.png", "angle_panel", m_pRenderer))
	{
		return false;
	}

	m_gameObjects.push_back(new AnglePanel(new LoaderParams(xpos+20, ypos+height-220, 200, 144, "angle_panel")));
	return true;
}
bool Game::addObject(GameObject* object)
{

	m_gameObjects.push_back(object);

	return true;
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
	//改变状态
	switch (m_cur_state)
	{
	case Idle:
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
		{
			m_cur_state = GettingAngle;
		}
		break;
	case GettingAngle:
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN))
		{
			m_cur_state = Shooting;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			m_cur_state = Idle;
		}
		break;
	case Shooting:
		m_cur_state = Idle;
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
			if("angle_panel" == id)
			{
				continue;
			}
		}
		if (GettingAngle == m_cur_state)
		{
			if ("angle_panel" == id)
			{
				m_shoot_angle = ((AnglePanel*)m_gameObjects[i])->getAngle();
				//cout << "shoot_angle=" << m_shoot_angle << endl;
			}
			if ( "people" == id)
			{
				continue;
			}
			if ("bullet" == id)
			{
				Bullet* b = (Bullet*)m_gameObjects[i];
				//cout << "b->getPosition().y=" << b->getPosition().y << endl;
				if (b->getPosition().y >= 500)
				{
					//cout << "erasing.." << endl;
					b -> needDelete = true;
				}
			}
		}
		if (Shooting == m_cur_state)
		{

			if ("people" == id)
			{
				((Player*)m_gameObjects[i])->shoot(m_shoot_angle);
			}
			
		}
		if (End == m_cur_state)
		{
			if ("angle_panel" == id || "people" == id)
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
}


