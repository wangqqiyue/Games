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

	//������Ȧ
	p->FillRect(outterX, outterY, width + border * 2.0f, height + border * 2.0f, borderColor);

	//������Ȧ
	p->FillRect(innerX, innerY, width, height);

	//��������
	p->DrawLine(p->ScreenWidth() / 2.0f, innerY, p->ScreenWidth() / 2.0f, innerY + height, borderColor);

	//��������Բ
	p->DrawCircle(p->ScreenWidth() / 2.0f, p->ScreenHeight() / 2.0f, radius,olc::BLACK);

	//��������
	p->FillRect(goalLeft.x, goalLeft.y, goalDepth, goalWidth, olc::GREEN);
	p->FillRect(goalRight.x, goalRight.y, goalDepth, goalWidth, olc::GREEN);


}