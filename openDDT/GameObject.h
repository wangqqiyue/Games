#pragma once
#include<string>
#include "LoaderParams.h"
#include "Global.h"

class GameObject
{
public:
	virtual void draw(int angle=0) = 0;
	virtual void update(State state) = 0;
	virtual void clean() = 0;
	bool needDelete = false;
protected:
	GameObject() {}
	GameObject(const LoaderParams* pParams) {}
	virtual ~GameObject() {}
	
};

