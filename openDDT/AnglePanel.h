#pragma once
#include "SDLGameObject.h"

class AnglePanel;

typedef AnglePanel TheAnglePanel;

class AnglePanel :
    public SDLGameObject
{
public:
	
	~AnglePanel() {}
	void draw(int angle=0);
	void update(State state);
	void clean();
	void addAngle(int da) { m_angle += da; }
	//µ¥ÀýÄ£Ê½
	static AnglePanel* Instance(const LoaderParams* pParams)
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new AnglePanel(pParams);
		}
		return s_pInstance;
	}

private:
	AnglePanel(const LoaderParams* pParams);
	static AnglePanel* s_pInstance;
};

