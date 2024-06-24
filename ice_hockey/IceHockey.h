#pragma once

#include "olcPixelGameEngine.h"


class Field {
public:
	float width;
	float height;
	//olc::vf2d points[2];
	float goalWidth;
	float goalDepth;
	float border;
	float innerX, innerY, outterX, outterY;
	olc::vf2d goalLeft, goalRight;

	Field() = default;
	void InitField(float w, float h, float gw, float gp, float b,const olc::PixelGameEngine* p);
	void DrawField(olc::PixelGameEngine* p);
};

// Override base class with your custom functionality
class IceHockey : public olc::PixelGameEngine
{
public:
	Field field;
	IceHockey()
	{
		// Name your application
		sAppName = "IceHockey";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		field.InitField(ScreenWidth()*0.8f, ScreenHeight()*0.8f, ScreenHeight()*0.2f, ScreenWidth()*0.05f, 20.0f, this);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		field.DrawField(this);
		return true;
	}
};
