#pragma once
#include "SDLGameObject.h"


class Bullet :
    public SDLGameObject
{
public:

	Bullet(const LoaderParams* pParams);
	~Bullet() {}
	void draw(int angle=0);
	void update();
	void clean();
	void setAngle(int angle);
};

