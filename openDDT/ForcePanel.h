#pragma once
#include "SDLGameObject.h"
class ForcePanel :
    public SDLGameObject
{
public:
	ForcePanel(const LoaderParams* pParams);
	~ForcePanel() {}
	void draw(int angle = 0);
	void update();
	void clean();
	int getForce() { return m_force; }
private:
	int m_force = 0;
};

