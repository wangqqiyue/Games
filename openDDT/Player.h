#ifndef __Player__
#define __Player__

#include <string>
#include "Global.h"
#include "SDLGameObject.h"

class Player : public SDLGameObject
{
public:

	Player(const LoaderParams* pParams);

	void draw(int angle);
	void update(State state);
	void clean();
	void shoot(int angle,int force);
	void setMyTurn(bool isMyTurn);
	bool getMyTurn() {return m_myTurn;}
	void onCollision();
	void setName(string name) { m_name = name; }
private:
	Direction m_direction=RIGHT;
	int m_hp = 100;//健康值,血条
	int m_mp = 100;//魔法值,体力条
	bool m_myTurn = false;
	int m_shoot_angle = 0;
	string m_name = "";
};

#endif // __Player__
