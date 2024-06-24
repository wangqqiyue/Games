#include "IceHockey.h"

void Field::InitField(float w, float h, float gw, float gp, float b,const olc::PixelGameEngine* p) {
	width = w;
	height = h;
	goalWidth = gw;
	goalDepth = gp;
	border = b;

	innerX = (p->ScreenWidth() - width) / 2.0f;
	innerY = (p->ScreenHeight() - height) / 2.0f;

	outterX = innerX - border;
	outterY = innerY - border;

	goalLeft.x = innerX;
	goalLeft.y = (p->ScreenHeight() - goalWidth) / 2.0f;

	goalRight.x = (p->ScreenWidth() + width) / 2.0f - goalDepth;
	goalRight.y = goalLeft.y;
}
void Field::DrawField(olc::PixelGameEngine* p) {
	olc::Pixel borderColor= olc::DARK_YELLOW;
	float radius = height/5.0f;

	//绘制外圈
	p->FillRect(outterX, outterY, width + border * 2.0f, height + border * 2.0f, borderColor);

	//绘制内圈
	p->FillRect(innerX, innerY, width, height);

	//绘制中线
	p->DrawLine(p->ScreenWidth() / 2.0f, innerY, p->ScreenWidth() / 2.0f, innerY + height, borderColor);

	//绘制中央圆
	p->DrawCircle(p->ScreenWidth() / 2.0f, p->ScreenHeight() / 2.0f, radius,olc::BLACK);

	//绘制球门
	p->FillRect(goalLeft.x, goalLeft.y, goalDepth, goalWidth, olc::GREEN);
	p->FillRect(goalRight.x, goalRight.y, goalDepth, goalWidth, olc::GREEN);


}

void Puck::InitPuck(float x, float y, float dx, float dy) {
	position.x = x;
	position.y = y;
	velocity.x = dx;
	velocity.y = dy;
	radius = float(puckSprite->width / 2.0f);
	cout << "width=" << puckSprite->width << endl;
	cout << "height=" << puckSprite->height << endl;
}


void Puck::DrawPuck(olc::PixelGameEngine* p) {
	p->DrawSprite(position.x-radius, position.y - radius, puckSprite.get(),1);
	p->DrawCircle(position.x, position.y, radius, olc::RED);
}

void Puck::Move(const Field& f) {
	float nextX, nextY;
	nextX = position.x ;
	nextY = position.y;

	if (nextX - radius < f.innerX || nextX +radius>f.innerX+f.width) {
		velocity.x = -velocity.x;
	}
	if (nextY - radius < f.innerY || nextY + radius > f.innerY + f.height) {
		velocity.y = -velocity.y;
	}
	position.x += velocity.x;
	position.y += velocity.y;
}