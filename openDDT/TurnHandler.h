#pragma once
#include "GameObject.h"
#include "Player.h"
#include <deque>
using std::deque;

class TurnHandler;

typedef TurnHandler TheTurnHandler;

class TurnHandler :
	public GameObject
{
public:
	virtual void draw(int angle = 0);
	virtual void update(State state);
	virtual void clean() {};
	//单例模式
	static TurnHandler* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new TurnHandler();
			return s_pInstance;
		}
		return s_pInstance;
	}
	void attachObserver(Player* p);
	void init();

	

private:
	TurnHandler() {}
	Player* getFirstPlayer();
	bool countDown();//返回时间是否用完
	void resetTime();

	bool m_bInitialized = false;
	static TurnHandler* s_pInstance;
	deque<Player*> m_waitQueue;
	Player* m_cur_player = nullptr;
	int m_count_time = 0;
	int m_turn_limit = 15000;
	int m_start_time = 0;
};