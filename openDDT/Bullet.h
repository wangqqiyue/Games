#pragma once
#include "SDLGameObject.h"


class Bullet :
    public SDLGameObject
{
public:

	Bullet(const LoaderParams* pParams);
	~Bullet() {}
	void draw(int angle=0);
	void update(State state);
	void clean();
	void init(int angle,int force);
};

