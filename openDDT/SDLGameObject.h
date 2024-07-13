#ifndef __SDLGameObject__
#define __SDLGameObject__

#include <string>

#include "GameObject.h"
#include "LoaderParams.h"
#include "Global.h"

class SDLGameObject : public GameObject
{
public:

	SDLGameObject(const LoaderParams* pParams);

	virtual void draw(int angle = 0);
	virtual void update(State state) {};
	virtual void clean() {};
	virtual void onCollision() {};
	std::string getTextureID() { return m_textureID; }
	int getAngle() { return m_angle; }
	void setAngle(int angle) { m_angle = angle; }
	vf2d getPosition() { return m_pos; }
	vi2d getSize() { return { m_width,m_height }; }

protected:

	vf2d m_pos={0,0};//位置

	int m_angle=0;//和x正方向的夹角

	vf2d m_v={0,0};//速度

	int m_width=0;
	int m_height=0;

	int m_currentRow=0;
	int m_currentFrame=0;

	std::string m_textureID="";
};

#endif // __SDLGameObject__
