#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include <vector>
using std::vector;

class BulletHandler;

typedef BulletHandler TheBulletHandler;

class BulletHandler :
    public GameObject
{
public:
	virtual void draw(int angle = 0);
	virtual void update(State state);
	virtual void clean() {};
	//µ¥ÀýÄ£Ê½
	static BulletHandler* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new BulletHandler();
			return s_pInstance;
		}
		return s_pInstance;
	}
	void addBullet(Bullet* b);
private:
	BulletHandler() {}
	static BulletHandler* s_pInstance;
	vector<Bullet*> bullets;
};


