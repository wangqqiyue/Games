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
	vector<pair<float, float>> psTranslated;
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
		psTranslated = s.psTranslated;
	}
	void Translate();
	void move() {
		x += dx;
		y += dy;
		Translate();
	}

	void print() {
		cout << "x,y,dx,dy,angle,psNum,nSize=" << x << "," << y 
			<< "," << dx << "," << dy << "," << angle << "," 
			<< points.size() << "," << nSize << endl;
	}
};


float GetDistance(float x1, float y1, float x2, float y2);
bool checkCollision(SpaceObject &s1, SpaceObject& s2);
//对角线法检测碰撞
bool checkCollisionDiag(SpaceObject& s1, SpaceObject& s2);

class Asteroid:public olc::PixelGameEngine
{
	
public:
	SpaceObject spaceship;
	vector<SpaceObject> asteroids;
	vector<SpaceObject> bullets;
	int score=0;
	int hp=10;
	int bulletsTotal=0;
	bool start = false;

	Asteroid() {
		sAppName = "Asteroid";
	}
	bool OnUserCreate()override {
		spaceship = { ScreenWidth() / 2.0f,ScreenHeight() / 2.0f,myRand() ,myRand(),myRand()*6.28f,{make_pair(0,-5),make_pair(-2.5,2.5),make_pair(0,1),make_pair(2.5,2.5)},5};
		generateAsteroids();
		return true;
	};

	bool OnUserUpdate(float fElapsedTime)override{
		Clear(olc::BLACK);
		
		if (start) {
			if (0 >= hp) {
				gameOver();
			}
			//检测是否胜利
			else if (0 == asteroids.size()) {
				gamePrompt();
				return true;
			}

			//绘制分数
			string scoreStr = "score=";
			scoreStr += to_string(score);
			DrawString(0, 0, scoreStr, olc::GREEN);
			//绘制生命值
			string hpStr = "hp";
			hpStr += to_string(hp);
			DrawString(ScreenWidth() - hpStr.length() * 8 - 50, 0, hpStr, (hp > 3 ? olc::GREEN : olc::RED));
			FillRect(ScreenWidth() - 45, 3, hp * 40 / 10, 5, (hp > 3 ? olc::GREEN : olc::RED));
			KeyHit(fElapsedTime);
		}else {
			StartGame();
		}

		// 预先准备一个vector来存储需要添加的元素
		std::vector<SpaceObject> newAsteroids;
		for (auto& b : bullets) {
			//检测子弹和陨石之间是否发生碰撞
			for (auto i = asteroids.begin(); i != asteroids.end(); i++) {
				
				if (checkCollisionDiag(*i, b)) {
					//陨石分裂成2块
					if (i->nSize > 2) {
						SpaceObject child1;
						SpaceObject child2;
						GetAsteroid(child1, i->x, i->y, i->nSize / 2);
						GetAsteroid(child2, i->x, i->y, i->nSize / 2);
						newAsteroids.push_back(child1);
						newAsteroids.push_back(child2);
					}
					i = asteroids.erase(i);
					i--;
					score++;
					b.x = -1100;
				}
				
			}
		}
		removeBullets();
		// 在循环外添加新生成的小行星
		asteroids.insert(asteroids.end(), newAsteroids.begin(), newAsteroids.end()); // 添加新元素
		newAsteroids.clear(); // 清空新元素容器
		for (auto &a: asteroids) {
			a.move();
			DrawWireFrame(a, olc::YELLOW);
			if (checkCollisionDiag(a, spaceship)) {
				spaceship.dx *= 0.8;
				spaceship.dy *= 0.8;
				hp--;
			}
		}

		spaceship.move();
		DrawWireFrame(spaceship);
		for (auto& b : bullets) {
			b.move();
			DrawCircle(b.x, b.y, b.nSize);
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
	void ResetGame();
	void StartGame();
	void GetAsteroid(SpaceObject& a, float x, float y, int nSize);
	void generateAsteroids();
	void generateBullets();
};
