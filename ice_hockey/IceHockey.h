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

//����
class Puck {
public:
	olc::vf2d position;
	olc::vf2d velocity;
	float radius;
	//you can't have an olc::Decal without an olc::Sprite.
	std::unique_ptr<olc::Sprite> puckSprite = std::make_unique<olc::Sprite>
		("C:\\Users\\17964\\Desktop\\game_learning\\ice_hockey\\hockey_puck3.png");
	//std::unique_ptr<olc::Decal> puckDecal= std::make_unique<olc::Decal>(puckSprite.get());

	Puck() = default;
	void InitPuck(float x, float y, float dx, float dy);
	void DrawPuck(olc::PixelGameEngine* p);
	void Move(const Field& f);

};

// Override base class with your custom functionality
class IceHockey : public olc::PixelGameEngine
{
public:
	Field field;
	Puck puck;
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
		puck.InitPuck(field.innerX+150.0f, field.innerY+100.0f, 4.0f, -3.0f);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		//�Ȼ��Ƴ���
		field.DrawField(this);
		//�ٻ��Ʊ���
		puck.DrawPuck(this);
		puck.Move(field);
		return true;
	}
};
