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
#include "NetworkManager.h"
#include <sstream>
using std::istringstream;
using std::getline;

Game* Game::s_pInstance = 0;

void Game::showUserName() {
	TheFontManager::Instance()->drawText(TheGame::Instance()->getRenderer(), "Please Input your user name", {0,0,0,255},
		300, 200);
	if (m_username.length()) {
		TheFontManager::Instance()->drawText(TheGame::Instance()->getRenderer(), m_username, { 0,0,0,255 },
			300, 220);
	}
}

void Game::editUserName(string str) {
	m_username += str;
}

bool Game::addPlayer(string name) {
	//增加人物
	if (!TheTextureManager::Instance()->load("assets/people.png", name, m_pRenderer))
	{
		return false;
	}
	Player* character = new Player(new LoaderParams(m_pos.x + 20, m_pos.y + m_size.y - 300, 45, 50, name));
	character->setName(name);
	m_gameObjects.push_back(character);
	//将人物加入碰撞管理器
	TheCollisionHandler::Instance()->attachObserver(character);
	//将人物加入回合管理器
	TheTurnHandler::Instance()->attachObserver(character);
	
	
	return true;
}


void Game::recvMsg(const char* msg, int len) {
	string type, content;
	istringstream iss(msg);
	getline(iss, type);
	getline(iss, content);

	if (type == "add") {
		addPlayer(content);
	}
}


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
		m_pos.x = xpos;
		m_pos.y = ypos;
		m_size.x = width;
		m_size.y = height;
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
	


	//增加角度盘
	if (!TheTextureManager::Instance()->load("assets/angle_panel3.jpg", "angle_panel", m_pRenderer))
	{
		return false;
	}
	TheAnglePanel::Instance(new LoaderParams(xpos + 70, ypos + height - 200, 100, 100, "angle_panel"));
	//m_gameObjects.push_back(TheAnglePanel::Instance(new LoaderParams(xpos + 70, ypos + height - 200, 100, 100, "angle_panel")));

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

	TheNetworkManager::Instance();

	m_cur_state = Typing;
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
	if (Typing == m_cur_state) {
		showUserName();
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
	else if(TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN))
	{
		m_cur_event = _enter_press;
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

	//增加新角色
	if (Adding == m_cur_state) {
		//向服务器发送新增角色的指令
		string msg;
		msg += "add\n";
		msg += m_username;
		TheNetworkManager::Instance()->send(msg.c_str());
		addPlayer(m_username);
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
	
	
}


