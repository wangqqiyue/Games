#pragma once

#include "olcPixelGameEngine.h"
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <iostream>
using std::cout;
using std::endl;

enum Side {
	LEFT =0,
	RIGHT,
};

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
	float friction = 0.01;//Ä¦²ÁÁ¦ÏµÊý
	olc::Pixel borderColor = olc::DARK_YELLOW;
	float heightRadiusRatio =  5.0f;
	olc::PixelGameEngine* p;

	Field() = default;
	void InitField(float w, float h, float gw, float gp, float b,olc::PixelGameEngine* p);
	void DrawField();
	void DrawBarrier();
};

//±ùÇò
class Puck {
public:
	olc::vf2d position;
	olc::vf2d velocity;
	float mass = 1.0f;
	float radius;
	olc::Pixel color;
	float goalPuckRatio = 4.0f;
	LPCWSTR bound_sound_file = TEXT("sound\\knock.wav"); 
	Field f;
	olc::PixelGameEngine* p;

	Puck() = default;
	void InitPuck(const Field& f, olc::Pixel col, olc::PixelGameEngine* p);
	void DrawPuck();
	void Move();

};

//ÇòÅÄ
class Paddle{
public:
	olc::vf2d pos;
	float innerR,outerR;
	olc::Pixel innerCol,outerCol;
	olc::vf2d lastPos;
	olc::vf2d v;
	float mass = 1.0f;
	float speedEasy = 10.0f;
	float goalPaddleRatio = 4.0f;
	olc::PixelGameEngine* p;
	Field f;
	Side side;

	void InitPaddle(const Field& f, Side side, olc::Pixel inCol, olc::Pixel outCol, olc::PixelGameEngine*p);
	void DrawPaddle();
	void Move();

};

class AiPaddle :public Paddle {
public:
	void InitPaddle(const Field& f, Side side, olc::Pixel inCol, olc::Pixel outCol, olc::PixelGameEngine* p,const Paddle* enemy);
	olc::vf2d posGoal;
	olc::vf2d posEnemyGoal;
	const Paddle* enemy;
};


// Override base class with your custom functionality
class IceHockey : public olc::PixelGameEngine
{
public:
	Field field;
	Puck puck;
	Paddle paddle;
	bool holdPaddle=false;
	Paddle player;
	AiPaddle ai1, ai2;
	LPCWSTR whistle_sound_file = TEXT("sound\\whistle.wav");

	IceHockey()
	{
		// Name your application
		sAppName = "IceHockey";
	}
	void MouseOperate(Paddle& paddle);
	void CollisionResponse(Paddle& paddle,float fElapsedTime);
	void AiResponse(AiPaddle& paddle);
	void AiResponseStrong(AiPaddle& paddle);
	void Rendering();
	bool PuckInGoal();
	void GameReset();
	LPCWSTR bound_sound_file = TEXT("sound\\knock.wav");
public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		field.InitField(ScreenWidth()*0.8f, ScreenHeight()*0.8f, ScreenHeight()*0.2f, ScreenWidth()*0.05f, 20.0f, this);
		GameReset();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		Rendering();
		//MouseOperate();
		AiResponse(ai1);
		AiResponseStrong(ai2);
		puck.Move();
		//CollisionResponse(player, fElapsedTime);
		CollisionResponse(ai1, fElapsedTime);
		CollisionResponse(ai2, fElapsedTime);
		if (PuckInGoal()) {
			GameReset();
		}
		return true;
	}
};
