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
	void move() {
		x += dx;
		y += dy;
	}
};

class Asteroid:public olc::PixelGameEngine
{
	
public:
	SpaceObject spaceship;
	vector<SpaceObject> asteroids;
	vector<SpaceObject> bulletes;
	int score;

	Asteroid() {
		sAppName = "Asteroid";
	}
	bool OnUserCreate()override {
		spaceship = { ScreenWidth()/2.0f,ScreenHeight()/2.0f,0.0f,0.0f,0.0f,{make_pair(0,-5),make_pair(-2.5,2.5),make_pair(2.5,2.5)},5};
		return true;
	};

	bool OnUserUpdate(float fElapsedTime)override{
		Clear(olc::BLACK);
		KeyHit(fElapsedTime);
		spaceship.move();
		
		DrawWireFrame(spaceship);
		return true;
	};

	void WrapCoordinates(float ix, float iy, float& ox, float& oy);
	bool KeyHit(float fElapsedTime);
	void DrawWireFrame(SpaceObject &sObject);
};

