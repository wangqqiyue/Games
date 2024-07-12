#pragma once
#include "GameObject.h"
#include <vector>
using std::vector;

class CollisionHandler;

typedef CollisionHandler TheCollisionHandler;

class CollisionHandler :
	public GameObject
{
public:
	virtual void draw(int angle = 0);
	virtual void update(State state);
	virtual void clean() {};
	//µ¥ÀýÄ£Ê½
	static CollisionHandler* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new CollisionHandler();
			return s_pInstance;
		}
		return s_pInstance;
	}
	void attachObserver(GameObject* go);
	void detachObserver(GameObject* go);
private:
	CollisionHandler() {}
	static CollisionHandler* s_pInstance;
	vector<GameObject*> m_gameObjects;
};