#include "IceHockey.h"

void IceHockey::GameReset() {
	PlaySound(whistle_sound_file, NULL, SND_FILENAME | SND_ASYNC);
	puck.InitPuck(field, olc::MAGENTA, this);
	paddle.InitPaddle(field, LEFT, olc::RED, olc::DARK_RED, this);
	AiPaddle.InitPaddle(field, RIGHT, olc::BLUE, olc::DARK_BLUE, this);
}

bool IceHockey::PuckInGoal() {
	float x = puck.position.x;
	float y = puck.position.y;
	float r = puck.radius;
	float goalY = field.goalLeft.y;
	float goalWidth = field.goalWidth;
	if ( y- r >= goalY && y+r <= goalY + goalWidth) {
		if (x + r <= field.innerX || x - r >= field.innerX + field.width) {
			return true;
		}
	}
	return false;
}

void IceHockey::CollisionResponse(Paddle& paddle,float fElapsedTime) {
	
	olc::vf2d vPaddle = paddle.v;
	olc::vf2d vRelative = puck.velocity - vPaddle;
	olc::vf2d vDis = puck.position - paddle.pos;
	olc::vf2d vNormal = vDis.norm();
	float vRn = vRelative.dot(vNormal);
	float dis = vDis.mag();
	float sumR = paddle.outerR + puck.radius;
	//距离小于两者半径和，且相对移动速度为正
	if ( dis < sumR && vRn<0.0f) {
		//如果靠的太近,需要调整撞击参数

		float j = -2.1f * vRn/ vNormal.dot(vNormal);
		j /= (1.0f / paddle.mass + 1.0f / puck.mass);
		puck.velocity += j * vNormal * 1.0f / puck.mass;
		paddle.v -= j * vNormal * 1.0f / paddle.mass;
		
		PlaySound(NULL, 0, 0);//先停止其他声音
		PlaySound(bound_sound_file, NULL, SND_FILENAME | SND_ASYNC);
		if (dis < sumR /4.0f) {
			paddle.pos -= vDis;
		}
	}
	
}

void IceHockey::MouseOperate() {
	olc::vi2d mPos=GetMousePos();
	if (!GetMouse(0).bHeld) {
		holdPaddle = false;
	}
	if (!holdPaddle) {
		if ((mPos, paddle.pos).mag() <= paddle.outerR && GetMouse(0).bHeld) {
			holdPaddle = true;
		}
	}
	
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
	paddle.v = paddle.pos - paddle.lastPos;
	
}

void IceHockey::Rendering() {
	//先绘制场地
	field.DrawField();
	//再绘制冰球
	puck.DrawPuck();
	//绘制边框
	field.DrawBarrier();
	//绘制球拍
	paddle.DrawPaddle();
	AiPaddle.DrawPaddle();
}

void IceHockey::AiResponse(float fElapsedTime) {
	olc::vf2d posGoal = { field.goalRight.x + field.goalDepth,field.goalRight.y + field.goalWidth / 2.0f };
	olc::vf2d posPlayer = paddle.pos;
	olc::vf2d nMove;

	if (puck.velocity.mag()>=AiPaddle.speedEasy&&(posGoal - puck.position).dot(puck.velocity) > 0.001f) {

		olc::vf2d posCenter = (posGoal + puck.position) / 2.0f;
		nMove = (posCenter - AiPaddle.pos).norm();
	}
	else {
		//进攻
		nMove = (puck.position - AiPaddle.pos).norm();
	}

	AiPaddle.v = AiPaddle.speedEasy * nMove;

	AiPaddle.Move();
	if (AiPaddle.pos.x <= ScreenWidth() / 2.0f + AiPaddle.outerR) {
		AiPaddle.pos.x = ScreenWidth() / 2.0f + AiPaddle.outerR;
	}
	if (AiPaddle.pos.x >= field.innerX+field.width - AiPaddle.outerR) {
		AiPaddle.pos.x = field.innerX + field.width - AiPaddle.outerR;
	}
	if (AiPaddle.pos.y >= field.innerY + field.height - paddle.outerR) {
		AiPaddle.pos.y = field.innerY + field.height - AiPaddle.outerR;
	}
	if (AiPaddle.pos.y <= field.innerY + paddle.outerR) {
		AiPaddle.pos.y = field.innerY + AiPaddle.outerR;
	}
}

void Field::InitField(float w, float h, float gw, float gp, float b,olc::PixelGameEngine* p) {
	this->p = p;
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
void Field::DrawField() {


	//绘制内圈
	p->FillRect(innerX, innerY, width, height);

	//绘制中线
	p->DrawLine(p->ScreenWidth() / 2.0f, innerY, p->ScreenWidth() / 2.0f, innerY + height, borderColor);

	//绘制中央圆
	p->DrawCircle(p->ScreenWidth() / 2.0f, p->ScreenHeight() / 2.0f, height/ heightRadiusRatio,olc::BLACK);

	//绘制球门
	p->FillRect(goalLeft.x, goalLeft.y, goalDepth, goalWidth, olc::GREEN);
	p->FillRect(goalRight.x, goalRight.y, goalDepth, goalWidth, olc::GREEN);


}

void Field::DrawBarrier() {
	p->FillRect(outterX, outterY, border, height + border * 2.0f, borderColor);
	p->FillRect(outterX, outterY, width + border * 2.0f,border, borderColor);
	p->FillRect(outterX+width+border, outterY, border, height + border * 2.0f, borderColor);
	p->FillRect(outterX, outterY+height+border, width + border * 2.0f, border, borderColor);
}

void Puck::InitPuck(const Field& f,olc::Pixel col,olc::PixelGameEngine *p) {
	this->p = p;
	this->f = f;
	position.x = f.innerX+f.width/2.0f;
	position.y = f.innerY+f.height/2.0f;
	velocity = { 0,0 };
	radius = f.goalWidth/goalPuckRatio;
	color = col;
}


void Puck::DrawPuck() {
	float x = position.x;
	float y = position.y;
	if (radius > 0)
	{
		int x0 = 0;
		int y0 = radius;
		int d = 3 - 2 * radius;

		//定义一个lambda函数drawline
		auto drawline = [&](int sx, int ex, int y)
		{
			for (int x = sx; x <= ex; x++)
				p->Draw(x, y, color);
		};

		while (y0 >= x0)
		{
			drawline(x - y0, x + y0, y - x0);
			if (x0 > 0)	drawline(x - y0, x + y0, y + x0);

			if (d < 0)
				d += 4 * x0++ + 6;
			else
			{
				if (x0 != y0)
				{
					drawline(x - x0, x + x0, y - y0);
					drawline(x - x0, x + x0, y + y0);
				}
				d += 4 * (x0++ - y0--) + 10;
			}
		}
	}
	else
		p->Draw(x, y, color);
	
	
}

void Puck::Move() {
	bool bound = true;
	p->DrawRect(position.x - radius, position.y - radius, radius * 2, radius * 2, olc::RED);

	if (position.y - radius >= f.goalLeft.y && position.y + radius <= f.goalLeft.y + f.goalWidth) {
		bound = false;
	}else if (position.x< f.innerX + radius) {
		position.x = f.innerX + radius;
		velocity.x = -velocity.x;
	}else if (position.x  > f.innerX + f.width- radius) {
		position.x = f.innerX + f.width - radius;
		velocity.x = -velocity.x;
	}else if (position.y  < f.innerY+ radius) {
		position.y = f.innerY + radius;
		velocity.y = -velocity.y;
	}else if ( position.y > f.innerY + f.height- radius) {
		position.y = f.innerY + f.height - radius;
		velocity.y = -velocity.y;	
	}else {
		bound = false;
	}

	if (bound) {
		PlaySound(bound_sound_file, NULL, SND_FILENAME | SND_ASYNC);
	}
	

	position.x += velocity.x;
	position.y += velocity.y;
	velocity *= (1.0f-f.friction);

}

void Paddle::InitPaddle(const Field& f,Side side, olc::Pixel inCol, olc::Pixel outCol, olc::PixelGameEngine* p) {
	this->p = p;
	this->f = f;
	v = { 0.0f,0.0f };
	innerR = f.goalWidth / goalPaddleRatio / 1.5f;
	outerR = f.goalWidth / goalPaddleRatio;
	if (LEFT == side) {
		pos.x = f.innerX + outerR;
	}
	else {
		pos.x = f.innerX +f.width - outerR;
	}
	pos.y = f.innerY + f.height / 2.0f;

	innerCol = inCol;
	outerCol = outCol;
}

void Paddle::DrawPaddle() {
	p->FillCircle(pos.x, pos.y, outerR, outerCol);
	p->FillCircle(pos.x, pos.y, innerR, innerCol);
}

void Paddle::Move() {
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