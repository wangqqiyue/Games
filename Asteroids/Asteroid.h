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
	//�������캯��, ��������Ϊ���������򷵻�ֵʱ,����
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
//�Խ��߷������ײ
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
			//����Ƿ�ʤ��
			else if (0 == asteroids.size()) {
				gamePrompt();
				return true;
			}

			//���Ʒ���
			string scoreStr = "score=";
			scoreStr += to_string(score);
			DrawString(0, 0, scoreStr, olc::GREEN);
			//��������ֵ
			string hpStr = "hp";
			hpStr += to_string(hp);
			DrawString(ScreenWidth() - hpStr.length() * 8 - 50, 0, hpStr, (hp > 3 ? olc::GREEN : olc::RED));
			FillRect(ScreenWidth() - 45, 3, hp * 40 / 10, 5, (hp > 3 ? olc::GREEN : olc::RED));
			KeyHit(fElapsedTime);
		}else {
			StartGame();
		}

		// Ԥ��׼��һ��vector���洢��Ҫ��ӵ�Ԫ��
		std::vector<SpaceObject> newAsteroids;
		for (auto& b : bullets) {
			//����ӵ�����ʯ֮���Ƿ�����ײ
			for (auto i = asteroids.begin(); i != asteroids.end(); i++) {
				
				if (checkCollisionDiag(*i, b)) {
					//��ʯ���ѳ�2��
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
		// ��ѭ������������ɵ�С����
		asteroids.insert(asteroids.end(), newAsteroids.begin(), newAsteroids.end()); // �����Ԫ��
		newAsteroids.clear(); // �����Ԫ������
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
