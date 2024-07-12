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
private:
	Direction m_direction=RIGHT;
	int m_hp = 100;//����ֵ,Ѫ��
	int m_mp = 100;//ħ��ֵ,������
	bool m_myTurn = false;
};

#endif // __Player__
