#pragma once
#include "olcPixelGameEngine.h"
#include <vector>
#include <utility>
#include <iostream>
#include <string>
using std::cout;
using std::endl;

using std::vector;
using std::pair;
using std::make_pair;
using std::string;
using std::to_string;

float myRand();

class SpaceObject {
public:
	float x, y, dx, dy, angle;
	int nSize;

	vector<pair<float, float>> points;
	SpaceObject() = default;
	SpaceObject(float x, float y, float dx, float dy, float angle, vector<pair<float, float>>ps,int size) {
		this->x = x;
		this->y = y;
		this->dx = dx;
		this->dy = dy;
		this->angle = angle;
		this->points = ps;
		this->nSize = size;
	}
	//拷贝构造函数, 当对象作为函数参数或返回值时,调用
	SpaceObject(const SpaceObject& s) {
		x = s.x;
		y = s.y;
		dx = s.dx;
		dy = s.dy;
		angle = s.angle;
		points = s.points;
		nSize = s.nSize;
	}
	
	void move() {
		x += dx;
		y += dy;
	}

	void print() {
		cout << "x,y,dx,dy,angle,psNum,nSize=" << x << "," << y 
			<< "," << dx << "," << dy << "," << angle << "," 
			<< points.size() << "," << nSize << endl;
	}
};

float getDistance(float x1, float y1, float x2, float y2);
bool checkCollision(SpaceObject &s1, SpaceObject& s2);

class Asteroid:public olc::PixelGameEngine
{
	
public:
	SpaceObject spaceship;
	vector<SpaceObject> asteroids;
	vector<SpaceObject> bullets;
	int score=0;
	int hp=10;
	int bulletsTotal=0;
	void generateAsteroids();
	void generateBullets();

	Asteroid() {
		sAppName = "Asteroid";
	}
	bool OnUserCreate()override {
		spaceship = { ScreenWidth()/2.0f,ScreenHeight()/2.0f,0.0f,0.0f,0.0f,{make_pair(0,-5),make_pair(-2.5,2.5),make_pair(2.5,2.5)},5};
		generateAsteroids();
		return true;
	};

	bool OnUserUpdate(float fElapsedTime)override{
		Clear(olc::BLACK);

		/*
		if (0 >= hp) {
			gameOver();
		}	
		//检测是否胜利
		else if (0 == asteroids.size()) {
			gamePrompt();
			return true;
		}
		*/

		//绘制分数
		string scoreStr = "score=";
		scoreStr += to_string(score);
		DrawString( 0,0 , scoreStr, olc::GREEN);
		//绘制生命值
		string hpStr = "hp";
		hpStr += to_string(hp);
		DrawString(ScreenWidth() -hpStr.length()*8-50, 0, hpStr, (hp > 3 ? olc::GREEN : olc::RED));
		FillRect(ScreenWidth()-45, 3, hp*40/10, 5, (hp > 3 ? olc::GREEN : olc::RED));

		KeyHit(fElapsedTime);


		spaceship.move();
		DrawWireFrame(spaceship);

		for (auto& i : bullets) {
			for (auto j = asteroids.begin(); j != asteroids.end(); j++) {
				if (getDistance(i.x,i.y,j->x,j->y)<i.nSize+j->nSize) {
					j = asteroids.erase(j);
					j--;
					score++;
				}
			}
			i.move();
			removeBullets();
			Draw( i.x,i.y);
		}

		for (auto& i : asteroids) {
			i.move();
			//陨石与飞船碰撞,减少血量
			if (checkCollision(i, spaceship)) {
				hp--;
			}
			DrawWireFrame(i, olc::YELLOW);
		}
		return true;
	};
	virtual bool Draw(int32_t x, int32_t y, olc::Pixel p = olc::WHITE) {
		float fx, fy;
		fx = x;
		fy = y;
		
		WrapCoordinates(fx, fy);

		return PixelGameEngine::Draw(fx, fy,p);
	}

	void WrapCoordinates(float &x, float &y);
	bool KeyHit(float fElapsedTime);
	void DrawWireFrame(SpaceObject &sObject,olc::Pixel color=olc::WHITE);
	void removeBullets();
	void gamePrompt();
	void gameOver();
};

void translatePoints(const SpaceObject& sObject, vector<pair<float, float>>& psTranslated);