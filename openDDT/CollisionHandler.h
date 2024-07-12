#pragma once
#include "GameObject.h"
#include "SDLGameObject.h"
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
	void attachObserver(SDLGameObject* go);
	void detachObserver(SDLGameObject* go);
	bool checkCollision(SDLGameObject* g1, SDLGameObject* g2);
	bool checkCollisionAll(SDLGameObject* g1);
private:
	CollisionHandler() {}
	static CollisionHandler* s_pInstance;
	vector<SDLGameObject*> m_gameObjects;
};