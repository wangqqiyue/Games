#include "TurnHandler.h"
#include "SDL.h"
#include <iostream>
#include <random>
#include "FontManager.h"
#include "Game.h"
using std::cout;
using std::endl;

TurnHandler* TurnHandler::s_pInstance = 0;

void TurnHandler::draw(int angle)
{
	//画倒计时数字
	std::string time_str = std::to_string(m_count_time / 1000);
	//cout << "Time Left: " << m_count_time / 1000 << endl;
	TheFontManager::Instance()->drawText(TheGame::Instance()->getRenderer(),
		time_str, { 255,0,0,255 }, 450, 100);
}

void TurnHandler::update(State state)
{
	if (!m_bInitialized)
	{
		init();
	}
	if (countDown() || !m_cur_player->getMyTurn())
	{
		m_cur_player->setMyTurn(false);
		m_waitQueue.push_back(m_cur_player);
		m_cur_player = m_waitQueue.front();
		m_waitQueue.pop_front();
		m_cur_player->setMyTurn(true);
		resetTime();
	}
}

/*根据随机性原则,随机获取一个玩家作为首发*/
Player* TurnHandler::getFirstPlayer()
{
	Player* player = nullptr;
	if (!m_waitQueue.empty())
	{
		// 创建一个随机数生成器
		std::random_device rd;
		std::mt19937 gen(rd()); // Mersenne Twister 19937 generator
		std::uniform_int_distribution<> dis(0, m_waitQueue.size() - 1);

		// 生成一个随机索引
		size_t randomIndex = dis(gen);
		player = m_waitQueue[randomIndex];
		auto it = m_waitQueue.begin();
		std::advance(it, randomIndex);
		m_waitQueue.erase(it);//从队列删除元素,且不影响其他元素顺序
	}
	else
	{
		//TODO 报错
	}

	return player;
}

void TurnHandler::init()
{
	if (!m_waitQueue.empty())
	{
		m_cur_player = getFirstPlayer();
		if (!m_cur_player)
		{
			//todo 报错
			return;
		}
		m_cur_player->setMyTurn(true);
		
		for (Player* p : m_waitQueue)
		{
			p->setMyTurn(false);
		}
		resetTime();
		m_bInitialized = true;
	}
	
}

void TurnHandler::attachObserver(Player *p)
{
	m_waitQueue.push_back(p);
}

bool TurnHandler::countDown()
{
	int time_passed = SDL_GetTicks() - m_start_time;
	bool bTimeUp = false;
	m_count_time = m_turn_limit - time_passed;
	if (m_count_time <= 0)
	{
		bTimeUp = true;
	}
	

	return bTimeUp;
}

void TurnHandler::resetTime()
{
	m_count_time = m_turn_limit;
	m_start_time = SDL_GetTicks();
}