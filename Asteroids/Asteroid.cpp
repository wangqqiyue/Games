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
	int psNumber = sObject.points.size();
	
	for (int i = 0; i < psNumber; i++) {
		int next = (i + 1) % psNumber;
		
		DrawLine(sObject.psTranslated[i].first, sObject.psTranslated[i].second, sObject.psTranslated[next].first, sObject.psTranslated[next].second, color);
		
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
	return checkCollisionDiag(s1, s2);
	SpaceObject* a = &s1;
	SpaceObject* b = &s2;
	if (s1.psTranslated.size() == 0) {
		s1.Translate();
	}
	if (s2.psTranslated.size() == 0) {
		s2.Translate();
	}
	for (int i = 0; i < 2; i++) {
		//调换顺序
		if (i == 1) {
			a = &s2;
			b = &s1;
		}
		for (int j = 0; j < a->psTranslated.size(); j++) {
			int next = (j + 1) % a->psTranslated.size();
			pair<float, float> axisProj = make_pair(-(a->psTranslated[next].second-a->psTranslated[j].second), a->psTranslated[next].first - a->psTranslated[j].first);
			float max_a = -9999;
			float min_a = 9999;
			float max_b = -9999;
			float min_b = 9999;
			
			for (int k = 0; k < a->psTranslated.size(); k++) {
				float q = a->psTranslated[k].first * axisProj.first + a->psTranslated[k].second * axisProj.second;
				if (q >= max_a) {
					max_a = q;
				}
				if (q <= min_a) {
					min_a = q;
				}
			}
			for (int k = 0; k < b->psTranslated.size(); k++) {
				float q = b->psTranslated[k].first * axisProj.first + b->psTranslated[k].second * axisProj.second;
				if (q >= max_b) {
					max_b = q;
				}
				if (q <= min_b) {
					min_b = q;
				}
			}

			if (!(max_b >= min_a && max_a >= min_b)) {
				return false;
			}
			
		}

	}

	return true;
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in the floats i_x and i_y.
char get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
	float p2_x, float p2_y, float p3_x, float p3_y, float* i_x, float* i_y)
{
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	float s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		if (i_x != NULL)
			*i_x = p0_x + (t * s1_x);
		if (i_y != NULL)
			*i_y = p0_y + (t * s1_y);
		return 1;
	}

	return 0; // No collision
}

//对角线法检测碰撞
bool checkCollisionDiag(SpaceObject& s1, SpaceObject& s2) {
	SpaceObject* a = &s1;
	SpaceObject* b = &s2;
	int detected = 0;
	if (s1.psTranslated.size() == 0) {
		s1.Translate();
	}
	if (s2.psTranslated.size() == 0) {
		s2.Translate();
	}
	for (int i = 0; i < 2; i++) {
		//调换顺序
		if (i == 1) {
			a = &s2;
			b = &s1;
		}
		for (int j = 0; j < a->psTranslated.size(); j++) {
			pair<float, float> line1_start;
			pair<float, float> line1_end;
			line1_start.first = a->x;
			line1_start.second = a->y;
			line1_end.first = a->psTranslated[j].first;
			line1_end.second = a->psTranslated[j].second;
			for (int k = 0; k < b->psTranslated.size(); k++) {
				int next = (k + 1) % b->psTranslated.size();
				pair<float, float> line2_start;
				pair<float, float> line2_end;
				line2_start.first = b->psTranslated[k].first;
				line2_start.second = b->psTranslated[k].second;
				line2_end.first = b->psTranslated[next].first;
				line2_end.second = b->psTranslated[next].second;

				detected = get_line_intersection(line1_start.first, line1_start.second,
					line1_end.first, line1_end.second,
					line2_start.first, line2_start.second,
					line2_end.first, line2_end.second,
					NULL, NULL);
				if (detected) {
					return true;
				}
			}


		}

	}

	return false;
}


void SpaceObject::Translate() {
	int psNumber = points.size();
	float angle = this->angle;
	if (psTranslated.size() == 0) {
		for (int i = 0; i < psNumber; i++) {
			psTranslated.push_back(make_pair(0, 0));
		}
	}
	
	//rotate
	for (int i = 0; i < psNumber; i++) {
		psTranslated[i].first = points[i].first * cosf(angle) - points[i].second * sinf(angle);
		psTranslated[i].second = points[i].first * sinf(angle) +points[i].second * cosf(angle);
	}


	//scale
	for (int i = 0; i < psNumber; i++) {
		psTranslated[i].first *= nSize;
		psTranslated[i].second *= nSize;
	}

	//translated to Screen World
	for (int i = 0; i < psNumber; i++) {
		psTranslated[i].first += x;
		psTranslated[i].second += y;
	}
}

bool IsPointInsideCircle(float cx, float cy, float radius, float x, float y)
{
	return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius;
}