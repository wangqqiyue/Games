#ifndef __Player__
#define __Player__

#include <string>
#include "Global.h"
#include "SDLGameObject.h"

class Player : public SDLGameObject
{
public:

	Player(const LoaderParams* pParams);

	void draw();
	void update();
	void clean();
	void turn(Direction d);
private:
	Direction m_direction=RIGHT;
};

#endif // __Player__