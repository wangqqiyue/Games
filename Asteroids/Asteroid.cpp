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
	if (GetKey(olc::DOWN).bHeld || GetKey(olc::S).bHeld) {
		spaceship.dx -= sinf(spaceship.angle) * fElapsedTime;
		spaceship.dy -= -cosf(spaceship.angle) * fElapsedTime;
	}
	if (GetKey(olc::LEFT).bHeld||GetKey(olc::A).bHeld) {
		spaceship.angle -= 1.0f* fElapsedTime;
	}
	if (GetKey(olc::RIGHT).bHeld|| GetKey(olc::D).bHeld) {
		spaceship.angle += 1.0f* fElapsedTime;
	}
	if (GetKey(olc::SPACE).bReleased) {
		generateBullets();
	}

	return true;
}

void Asteroid::DrawWireFrame(SpaceObject &sObject,olc::Pixel color) {
	WrapCoordinates(sObject.x, sObject.y);
	vector<pair<float, float>> psTranslated=sObject.points;
	int psNumber = sObject.points.size();

	translatePoints(sObject,psTranslated);
	
	for (int i = 0; i < psNumber; i++) {
		int next = (i + 1) % psNumber;
		DrawLine(psTranslated[i].first, psTranslated[i].second, psTranslated[next].first, psTranslated[next].second, color);
	}
}
void Asteroid::removeBullets() {
	for (auto i = bullets.begin(); i != bullets.end(); ) {
		if (i->x<=0 || i->x>=ScreenWidth() || i->y<=0|| i->y>=ScreenHeight()) {
			i = bullets.erase(i++);
			bulletsTotal--;
			/*
			cout << "删除子弹" << bulletsTotal << endl;
			*/
			continue;
		}
		i++;
	}
}

void Asteroid::generateBullets() {
	SpaceObject b(spaceship);
	float speed = 3.0f;
	b.nSize = 2;
	b.points.clear();
	b.x += spaceship.points[0].first * cosf(b.angle) - spaceship.points[0].second * sinf(b.angle);
	b.y += spaceship.points[0].first * sinf(b.angle) + spaceship.points[0].second * cosf(b.angle);
	b.dx = speed * sinf(b.angle);
	b.dy = -speed * cosf(b.angle);
	b.points.push_back(make_pair(b.x, b.y));
	bullets.push_back(b);
	/*
	cout << "创建子弹" <<bulletsTotal++<< endl;
	cout << "spaceship.x,y=" << spaceship.x << "," << spaceship.y << endl;
	cout << "x,y=" << b.x << "," << b.y << endl;
	*/
}

void Asteroid::generateAsteroids() {
	for (int j = 0; j < 3; j++) {
		SpaceObject a;
		a.x = myRand()* ScreenWidth();
		a.y = myRand() * ScreenHeight();
		a.dx = myRand() * 0.10f;
		a.dy = myRand() * 0.10f;
		a.angle = myRand() * 6.28f;
		a.nSize = 6;

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
		/*
		cout << "创建成功" << endl;
		a.print();
		*/

	}
}
void Asteroid::gamePrompt() {
	string win = "You Win!";
	string playAgain = "Play Again?y/n";
	DrawString((ScreenWidth()-win.size()* GetScreenPixelSize().x)/2, ScreenHeight() / 2- GetScreenPixelSize().y, win);
	DrawString((ScreenWidth()-playAgain.size() * GetScreenPixelSize().x) / 2, ScreenHeight() / 2+ GetScreenPixelSize().y, playAgain);
	if (GetKey(olc::Y).bHeld) {
		generateAsteroids();
		score = 0;
		hp = 0;
	}
}
void Asteroid::gameOver() {
	string die = "You Die!";
	string playAgain = "Play Again?y/n";
	asteroids.clear();
	DrawString((ScreenWidth() - die.size() * GetScreenPixelSize().x) / 2, ScreenHeight() / 2 - GetScreenPixelSize().y, die);
	DrawString((ScreenWidth() - playAgain.size() * GetScreenPixelSize().x) / 2, ScreenHeight() / 2 + GetScreenPixelSize().y, playAgain);
	if (GetKey(olc::Y).bHeld) {
		generateAsteroids();
		score = 0;
		hp = 10;
	}
}

float getDistance(float x1,float y1,float x2,float y2) {
	float dx = (x1 - x2) * (x1 - x2);
	float dy = (y1 - y2) * (y1 - y2);
	return sqrtf(dx + dy);
}

bool checkCollision(SpaceObject &s1, SpaceObject &s2) {
	int detected=0;
	float x[3];
	float y[3];
	SpaceObject sTriangle = (s1.points.size() == 3) ? s1 : s2;
	SpaceObject sCircle = (s1.points.size() == 3) ? s2 : s1;
	vector<pair<float, float>> psTranslated;
	psTranslated.push_back(make_pair(0, 0));
	psTranslated.push_back(make_pair(0, 0));
	psTranslated.push_back(make_pair(0, 0));
	translatePoints(sTriangle, psTranslated);

	for (int i = 0; i < 3; i++) {
		if (getDistance(psTranslated[i].first, psTranslated[i].second, sCircle.x, sCircle.y) <= s1.nSize + s2.nSize) {
			detected++;
		}
	}
	
	if (detected>0) {
		//物体互相远离
		float dx, dy;
		if (s1.nSize < s1.nSize) {
			s1.dx = s2.dx;
			s1.dy = s2.dy;
		}
		else {
			s1.dx = s2.dx;
			s1.dy = s2.dy;
		}
		return true;
	}
		
	//cout << "距离是" << sqrtf(dx + dy) << endl;
	return false;
}

void translatePoints(const SpaceObject& sObject, vector<pair<float, float>>& psTranslated) {
	int psNumber = sObject.points.size();
	float angle = sObject.angle;

	//rotate
	for (int i = 0; i < psNumber; i++) {
		psTranslated[i].first = sObject.points[i].first * cosf(angle) - sObject.points[i].second * sinf(angle);
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
}