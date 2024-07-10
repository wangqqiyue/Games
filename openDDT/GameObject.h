#pragma once
#include<string>
#include "LoaderParams.h"

class GameObject
{
public:
	virtual void draw(int angle=0) = 0;
	virtual void update() = 0;
	virtual void clean() = 0;
protected:
	GameObject(const LoaderParams* pParams) {}
	virtual ~GameObject() {}

};

