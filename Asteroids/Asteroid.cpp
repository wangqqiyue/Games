#ifndef INCLUDE_ASTEROID
#define INCLUDE_ASTEROID
#include "Asteroid.h"
#endif 
float myRand() {
	return float((float)rand() / RAND_MAX);
}
void Asteroid::WrapCoordinates(float & x, float& y) {

	if (x <= 0.0f)	x = x + (float)ScreenWidth();
	if (x >= (float)ScreenWidth())	x = x - (float)ScreenWidth();
	if (y <= 0.0f)	y = y + (float)ScreenHeight();
	if (y >= (float)ScreenHeight()) y = y - (float)ScreenHeight();
}

bool Asteroid::KeyHit(float fElapsedTime) {
	if (GetKey(olc::UP).bHeld|| GetKey(olc::W).bHeld) {
		spaceship.dx += sinf(spaceship.angle)* fElapsedTime;
		spaceship.dy += -cosf(spaceship.angle)* fElapsedTime;
	}
	if (GetKey(olc::LEFT).bHeld||GetKey(olc::A).bHeld) {
		spaceship.angle -= 1.0f* fElapsedTime;
	}
	if (GetKey(olc::RIGHT).bHeld|| GetKey(olc::D).bHeld) {
		spaceship.angle += 1.0f* fElapsedTime;
	}

	return true;
}

void Asteroid::DrawWireFrame(SpaceObject &sObject,olc::Pixel color) {
	WrapCoordinates(sObject.x, sObject.y);
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
		DrawLine(psTranslated[i].first, psTranslated[i].second, psTranslated[next].first, psTranslated[next].second, color);
	}
}

void Asteroid::generateAsteroids() {
	for (int j = 0; j < 3; j++) {
		SpaceObject a = { (myRand() - 1.0f) * ScreenWidth(),myRand() * ScreenHeight() ,myRand() * 0.10f,myRand() * 0.10f,myRand() * 6.28f,{},6};
		int psNum = 20;
		for (int i = 0; i < psNum; i++) {
			float r = a.nSize * (0.8f + 0.4f * myRand());
			float angle = (6.28f * i / psNum);
			float x = r * sinf(angle);
			float y = r * cosf(angle);
			pair<float, float> p = make_pair(x, y);
			a.points.push_back(p);
		}
		asteroids.push_back(a);
	}
}

