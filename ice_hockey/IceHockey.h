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
	float friction = 0.01;//摩擦力系数
	olc::Pixel borderColor = olc::DARK_YELLOW;
	float heightRadiusRatio =  5.0f;
	olc::PixelGameEngine* p;

	Field() = default;
	void InitField(float w, float h, float gw, float gp, float b,olc::PixelGameEngine* p);
	void DrawField();
	void DrawBarrier();
};

//冰球
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
	void Move(float fElapsedTime);

};

//球拍
class Paddle{
public:
	int score = 0;
	olc::vf2d pos;
	float innerR,outerR;
	olc::Pixel innerCol,outerCol;
	olc::vf2d lastPos;
	olc::vf2d v;
	float mass = 1.0f;
	float speedEasy = 2.0f;
	float speedNormal = 4.0f;
	float speedHard = 10.0f;
	float goalPaddleRatio = 4.0f;
	olc::PixelGameEngine* p;
	Field f;
	Side side;
	bool win = false;

	void InitPaddle(const Field& f, Side side, olc::Pixel inCol, olc::Pixel outCol, olc::PixelGameEngine*p);
	void DrawPaddle();
	void Move(float fElapsedTime);

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
	Paddle player1;
	AiPaddle ai1, ai2;
	LPCWSTR whistle_sound_file = TEXT("sound\\whistle.wav");
	LPCWSTR bound_sound_file = TEXT("sound\\knock.wav");
	LPCWSTR win_sound_file = TEXT("sound\\win.wav");
	LPCWSTR lose_sound_file = TEXT("sound\\lose.wav");
	bool reset = false;


	void MouseOperate(Paddle& paddle);
	void CollisionResponse(Paddle& paddle,float fElapsedTime);
	void AiResponse(AiPaddle& paddle,float fElapsedTime);
	void AiResponseStrong(AiPaddle& paddle,float fElapsedTime);
	void Rendering();
	bool PuckInGoal();
	void GameReset();
	void DrawScore(int s1,int s2);
	void DrawWin(const Paddle& p1, const Paddle& p2);
public:
	IceHockey()
	{
		// Name your application
		sAppName = "IceHockey";
	}

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		field.InitField(ScreenWidth()*0.8f, ScreenHeight()*0.6f, ScreenHeight()*0.3f, ScreenWidth()*0.05f, 20.0f, this);
		GameReset();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		static std::chrono::time_point<std::chrono::system_clock>  lastTime;
		Clear(olc::WHITE);
		if (reset) {
			auto now = std::chrono::system_clock::now();
			std::chrono::duration<float> duration = (now - lastTime);
			DrawWin(player1,ai2);
			if (duration.count() > 1.0f)
			{
				//播放开始比赛的哨声
				PlaySound(whistle_sound_file, NULL, SND_FILENAME | SND_ASYNC);
				reset = false;
			}
			
		}
		else {
			MouseOperate(player1);
			//AiResponseStrong(ai1);
			AiResponseStrong(ai2, fElapsedTime);
			puck.Move(fElapsedTime);
			//CollisionResponse(player, fElapsedTime);
			//CollisionResponse(ai1, fElapsedTime);
			CollisionResponse(player1, fElapsedTime);
			CollisionResponse(ai2, fElapsedTime);
		}

	
		Rendering();
		
		if (PuckInGoal()) {
			GameReset();
			reset = true;
			lastTime= std::chrono::system_clock::now();
		}

		return true;
	}
};
