#ifndef INCLUDE_ASTEROID
#define INCLUDE_ASTEROID
#include "Asteroid.h"
#endif 

void Asteroid::WrapCoordinates(float ix, float iy, float& ox, float& oy) {
	ox = ix;
	oy = iy;
	if (ix < 0.0f)	ox = ix + (float)ScreenWidth();
	if (ix >= (float)ScreenWidth())	ox = ix - (float)ScreenWidth();
	if (iy < 0.0f)	oy = iy + (float)ScreenHeight();
	if (iy >= (float)ScreenHeight()) oy = iy - (float)ScreenHeight();
}

bool Asteroid::KeyHit(float fElapsedTime) {
	if (GetKey(olc::UP).bHeld) {
		spaceship.dx += sinf(spaceship.angle)* fElapsedTime;
		spaceship.dy += -cosf(spaceship.angle)* fElapsedTime;
	}
	if (GetKey(olc::DOWN).bHeld) {
		spaceship.dx -= sinf(spaceship.angle) * fElapsedTime;
		spaceship.dy -= -cosf(spaceship.angle) * fElapsedTime;
	}
	if (GetKey(olc::LEFT).bHeld) {
		spaceship.angle -= 1.0f* fElapsedTime;
	}
	if (GetKey(olc::RIGHT).bHeld) {
		spaceship.angle += 1.0f* fElapsedTime;
	}

	return true;
}

void Asteroid::DrawWireFrame(SpaceObject &sObject) {
	WrapCoordinates(sObject.x, sObject.y, sObject.x, sObject.y);
	vector<pair<float, float>> psTranslated=sObject.points;
	int psNumber = sObject.points.size();
	float angle = sObject.angle;
	//rotate
	for (int i = 0; i < psNumber; i++) {
		psTranslated[i].first = sObject.points[i].first * cosf(angle)-sObject.points[i].second*sinf(angle);
		psTranslated[i].second = sObject.points[i].first * sinf(angle) + sObject.points[i].second * cosf(angle);
	}


	//scale
	for (int i = 0; i < psNumber; i++) {
		psTranslated[i].first *= sObject.nSize;
		psTranslated[i].second *= sObject.nSize;
	}

	//translated to Screen World
	for (int i = 0; i < psNumber; i++) {
		psTranslated[i].first += sObject.x;
		psTranslated[i].second += sObject.y;
	}

	for (int i = 0; i < psNumber; i++) {
		int next = (i + 1) % psNumber;
		DrawLine(psTranslated[i].first, psTranslated[i].second, psTranslated[next].first, psTranslated[next].second);
	}
}