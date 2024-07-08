#pragma once
#include "SDLGameObject.h"
class AnglePanel :
    public SDLGameObject
{
public:
	AnglePanel(const LoaderParams* pParams);
	~AnglePanel() {}
	void draw();
	void update();
	void clean();
private:
	int m_angle;
};

