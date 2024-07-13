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
	void setAngle(int angle) { m_angle = angle; }
	int getAngle() { return m_angle; }
	//µ¥ÀýÄ£Ê½
	static AnglePanel* Instance(const LoaderParams* pParams=nullptr)
	{
		if (s_pInstance == 0)
		{
			if (pParams)
			{
				s_pInstance = new AnglePanel(pParams);
			}
			else
			{
				printf("nullptr as parameter\n");
			}
		}
		return s_pInstance;
	}

private:
	AnglePanel(const LoaderParams* pParams);
	static AnglePanel* s_pInstance;
};

