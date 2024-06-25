#include "IceHockey.h"

float GetDistance(olc::vf2d p1, olc::vf2d p2){
	return sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

void IceHockey::MouseOperate() {
	olc::vi2d mPos=GetMousePos();
	if (!GetMouse(0).bHeld) {
		holdPaddle = false;
	}
	if (!holdPaddle) {
		if (GetDistance(mPos, paddle.pos) <= paddle.outerR && GetMouse(0).bHeld) {
			holdPaddle = true;
		}
	}
	if (holdPaddle) {
		paddle.lastPos = paddle.pos;
		paddle.pos.x = mPos.x;
		paddle.pos.y = mPos.y;
		if (paddle.pos.x >= ScreenWidth() / 2.0f-paddle.outerR) {
			paddle.pos.x = ScreenWidth() / 2.0f - paddle.outerR;
		}
		if (paddle.pos.x <= field.innerX+paddle.outerR) {
			paddle.pos.x = field.innerX + paddle.outerR;
		}
		if (paddle.pos.y >= field.innerY+field.height - paddle.outerR) {
			paddle.pos.y = field.innerY + field.height - paddle.outerR;
		}
		if (paddle.pos.y <= field.innerY + paddle.outerR) {
			paddle.pos.y = field.innerY + paddle.outerR;
		}
		paddle.v = paddle.GetVelocity();
	}


}

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

void Puck::InitPuck(float x, float y, float dx, float dy,float r,olc::Pixel col) {
	position.x = x;
	position.y = y;
	velocity.x = dx;
	velocity.y = dy;
	radius = r;
	color = col;
}


void Puck::DrawPuck(olc::PixelGameEngine* p) {
	p->FillCircle(position.x, position.y, radius,color);
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

void Paddle::InitPaddle(float x, float y, float inR, float outR, olc::Pixel inCol, olc::Pixel outCol) {
	pos.x = x;
	pos.y = y;
	innerR = inR;
	outerR = outR;
	innerCol = inCol;
	outerCol = outCol;
}

void Paddle::DrawPaddle(olc::PixelGameEngine* p) {
	p->FillCircle(pos.x, pos.y, outerR, outerCol);
	p->FillCircle(pos.x, pos.y, innerR, innerCol);
}

olc::vf2d Paddle::GetVelocity() {
	olc::vf2d v = pos - lastPos;
	return v;
}

void Paddle::Move(const Field& f) {
	float nextX, nextY;
	nextX = pos.x;
	nextY = pos.y;

	if (nextX - outerR < f.innerX || nextX + outerR>f.innerX + f.width) {
		pos.x = -v.x;
	}
	if (nextY - outerR < f.innerY || nextY + outerR > f.innerY + f.height) {
		pos.y = -v.y;
	}
	pos.x += v.x;
	pos.y += v.y;
}