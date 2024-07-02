#pragma once

#include "olcPixelGameEngine.h"
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>


#define SPEED_MAX_INIT 60

enum Side {
	LEFT =0,
	RIGHT,
};
enum Direction {
	NW=0,//North West 西北
	NE,//东北
	SW,//西南
	SE,//东南
};
class Field {
public:
	float width;
	float height;

	float goalWidth;
	float border;
	float innerX, innerY;
	olc::vf2d goalLeft, goalRight;
	float friction = 0.01;//摩擦力系数
	olc::Pixel borderColor = olc::BLACK;
	float heightRadiusRatio =  8.0f;
	olc::PixelGameEngine* p;

	Field() = default;
	void InitField(float w, float h, float gw, float b,olc::PixelGameEngine* p);
	void DrawField();
	void DrawBarrier();
	void DrawSpot(float x, float y, float r, olc::Pixel c, bool drawCentral =true);
	void DrawZoneLine(float x1, float y1, float x2, float y2, olc::Pixel c);
	void DrawArc(olc::vf2d start, olc::vf2d end, olc::Pixel c,Direction d);
};

//冰球
class Puck {
public:
	olc::vf2d position;
	olc::vf2d velocity;
	float radius;
	olc::Pixel color;
	Field f;
	olc::PixelGameEngine* p;
	float SPEED_MAX= SPEED_MAX_INIT;
	float mass = 1.0f;
	float goalPuckRatio = 4.0f;
	LPCWSTR bound_sound_file = TEXT("sound\\knock.wav");

	Puck() = default;
	void InitPuck(const Field& f, olc::Pixel col, olc::PixelGameEngine* p);
	void DrawPuck();
	void Move();

};

//球拍
class Paddle{
public:
	olc::vf2d pos;
	float innerR,outerR;
	olc::Pixel innerCol,outerCol;
	olc::vf2d lastPos;
	olc::vf2d v;
	olc::PixelGameEngine* p;
	Field f;
	Side side;

	int score = 0;
	float mass = 1.0f;
	float SPEED_MAX = SPEED_MAX_INIT;
	float speedEasy = 2.0f;
	float speedNormal = 4.0f;
	float speedHard = 10.0f;
	float goalPaddleRatio = 4.0f;
	bool win = false;

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


class IceHockey : public olc::PixelGameEngine
{
public:
	Field field;
	Puck puck;
	Paddle paddle;
	bool holdPaddle=false;
	Paddle player1;
	AiPaddle ai1, ai2;
	
	bool reset = false;
	bool judged = false;
	bool aiPlay = true;
	int FPS_MAX_INIT = 20;
	int maxFPS = FPS_MAX_INIT;
	int SPEED_MAX = SPEED_MAX_INIT;

	LPCWSTR whistle_sound_file = TEXT("sound\\whistle.wav");
	LPCWSTR bound_sound_file = TEXT("sound\\knock.wav");
	LPCWSTR win_sound_file = TEXT("sound\\win.wav");
	LPCWSTR lose_sound_file = TEXT("sound\\lose.wav");
	std::string  ring_img_file = "img\\olympic-ring.jpg";
	std::string  bg_img_file = "img\\beijing-2022.jpg";
	std::string  logo_img_file = "img\\logo.jpg";
	std::string  china_img_file = "img\\china-flag.png";
	std::string  france_img_file = "img\\france-flag.png";
	std::unique_ptr<olc::Sprite> bgSprite;
	std::unique_ptr<olc::Decal> bgDecal;
	std::unique_ptr<olc::Sprite> ringSprite;
	std::unique_ptr<olc::Decal> ringDecal;
	std::unique_ptr<olc::Sprite> logoSprite;
	std::unique_ptr<olc::Decal> logoDecal;
	std::unique_ptr<olc::Sprite> chinaSprite;
	std::unique_ptr<olc::Decal> chinaDecal;
	std::unique_ptr<olc::Sprite> franceSprite;
	std::unique_ptr<olc::Decal> franceDecal;
	

	void MouseOperate(Paddle& paddle);
	void CollisionResponse(Paddle& paddle);
	void AiResponse(AiPaddle& paddle);
	void AiResponseStrong(AiPaddle& paddle);
	void Rendering();
	bool PuckInGoal();
	void GameReset();
	void DrawScore(int s1,int s2);
	void DrawWin(const Paddle& p1, const Paddle& p2);
	void KeyOperation();
	void DrawSpeed();
	void DrawFPS();
	void DrawTime(int x, int y, int scale);

public:
	IceHockey()
	{
		sAppName = "IceHockey";
	}

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		field.InitField(ScreenWidth()*0.7f, ScreenHeight()*0.7f, ScreenHeight()*0.2f,  5.0f, this);
		bgSprite= std::make_unique<olc::Sprite>(bg_img_file);
		bgDecal = std::make_unique<olc::Decal>(bgSprite.get());
		ringSprite = std::make_unique<olc::Sprite>(ring_img_file);
		ringDecal = std::make_unique<olc::Decal>(ringSprite.get());
		logoSprite = std::make_unique<olc::Sprite>(logo_img_file);
		logoDecal = std::make_unique<olc::Decal>(logoSprite.get());
		chinaSprite = std::make_unique<olc::Sprite>(china_img_file);
		chinaDecal = std::make_unique<olc::Decal>(chinaSprite.get());
		franceSprite = std::make_unique<olc::Sprite>(france_img_file);
		franceDecal = std::make_unique<olc::Decal>(franceSprite.get());
		GameReset();
		SetMaxFPS(FPS_MAX_INIT);
		aiPlay = false;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		static std::chrono::time_point<std::chrono::system_clock>  lastTime;
		Clear(olc::WHITE);
		if (reset) {
			auto now = std::chrono::system_clock::now();
			std::chrono::duration<float> duration = (now - lastTime);
			if (aiPlay) {
				DrawWin(ai1, ai2);
			}
			else {
				DrawWin(player1, ai2);
			}
			if (duration.count() > 1.0f)
			{
				GameReset();
				//播放开始比赛的哨声
				PlaySound(whistle_sound_file, NULL, SND_FILENAME | SND_ASYNC);
				judged = false;
				reset = false;
			}
			
		}
		else {
			KeyOperation();
			
			puck.Move();
			if (!aiPlay) {
				MouseOperate(player1);
				CollisionResponse(player1);
			}
			else {
				AiResponseStrong(ai1);
				//AiResponse(ai1);
				ai1.Move();
				CollisionResponse(ai1);
			}
			
			AiResponseStrong(ai2);
			//AiResponse(ai2);
			ai2.Move();
			CollisionResponse(ai2);
		}

		Rendering();
		
		if (PuckInGoal()) {
			reset = true;
			lastTime= std::chrono::system_clock::now();
		}

		return true;
	}
};
