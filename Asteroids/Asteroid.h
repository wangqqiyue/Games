#pragma once
#include "olcPixelGameEngine.h"
#include <vector>
#include <utility>
#include <iostream>
using std::cout;
using std::endl;

using std::vector;
using std::pair;
using std::make_pair;

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
	//�������캯��, ��������Ϊ���������򷵻�ֵʱ,����
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

bool checkCllision(SpaceObject s1, SpaceObject s2);

class Asteroid:public olc::PixelGameEngine
{
	
public:
	SpaceObject spaceship;
	vector<SpaceObject> asteroids;
	vector<SpaceObject> bullets;
	int score;
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
		KeyHit(fElapsedTime);
		spaceship.move();
		for (auto& i : asteroids) {
			i.move();
			DrawWireFrame(i,olc::YELLOW);
		}
		DrawWireFrame(spaceship);
		for (auto& i : bullets) {
			i.move();
			removeBullets();
			Draw(i.x, i.y);
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
};

