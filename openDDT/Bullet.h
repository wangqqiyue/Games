#pragma once
#include "SDLGameObject.h"


class Bullet :
    public SDLGameObject
{
public:

	Bullet(const LoaderParams* pParams);
	~Bullet() {}
	void draw();
	void update();
	void clean();
};

