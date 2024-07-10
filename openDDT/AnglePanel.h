#pragma once
#include "SDLGameObject.h"
class AnglePanel :
    public SDLGameObject
{
public:
	AnglePanel(const LoaderParams* pParams);
	~AnglePanel() {}
	void draw(int angle=0);
	void update();
	void clean();
};

