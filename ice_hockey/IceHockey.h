#pragma once

#include "olcPixelGameEngine.h"
#include <iostream>
using std::cout;
using std::endl;

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

//冰球
class Puck {
public:
	olc::vf2d position;
	olc::vf2d velocity;
	float radius;
	olc::Pixel color;

	Puck() = default;
	void InitPuck(float x, float y, float dx, float dy, float r, olc::Pixel col);
	void DrawPuck(olc::PixelGameEngine* p);
	void Move(const Field& f);

};

//球拍
class Paddle{
public:
	olc::vf2d pos;
	float innerR,outerR;
	olc::Pixel innerCol,outerCol;
	void InitPaddle(float x, float y, float inR, float outR, olc::Pixel inCol, olc::Pixel outCol);
	void DrawPaddle(olc::PixelGameEngine* p);
};

// Override base class with your custom functionality
class IceHockey : public olc::PixelGameEngine
{
public:
	Field field;
	Puck puck;
	Paddle paddle;
	bool holdPaddle=false;
	IceHockey()
	{
		// Name your application
		sAppName = "IceHockey";
	}
	void MouseOperate();

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		field.InitField(ScreenWidth()*0.8f, ScreenHeight()*0.8f, ScreenHeight()*0.2f, ScreenWidth()*0.05f, 20.0f, this);
		puck.InitPuck(field.innerX+150.0f, field.innerY+100.0f, 4.0f, -3.0f,30.0f,olc::MAGENTA);
		paddle.InitPaddle(field.innerX + 30.0f, ScreenHeight() / 2.0f, 20.0f,35.0f, olc::RED, olc::DARK_RED);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		MouseOperate();
		//先绘制场地
		field.DrawField(this);
		//再绘制冰球
		puck.DrawPuck(this);
		puck.Move(field);
		paddle.DrawPaddle(this);
		return true;
	}
};
