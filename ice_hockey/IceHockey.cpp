#include "IceHockey.h"

bool BoundBarrier(olc::vf2d& pos,olc::vf2d* v, float r, olc::vf2d barrier,  float width, float height) {
	bool bound = false;
	if (pos.x < barrier.x + r) {//左边界
		pos.x = barrier.x + r;
		if (v) {
			v->x = -v->x;
		}
		bound = true;
	}
	else if (pos.x > barrier.x+width-r) {//右边界
		pos.x = barrier.x + width - r;
		if (v) {
			v->x = -v->x;
		}
		bound = true;
	}
	else if (pos.y < barrier.y+r) {//上边界
		pos.y = barrier.y + r;
		if (v) {
			v->y = -v->y;
		}
		bound = true;
	}
	else if (pos.y > barrier.y +height- r) {//下边界
		pos.y = barrier.y + height - r;
		if (v) {
			v->y = -v->y;
		}
		bound = true;
	}
	return bound;
}

void IceHockey::DrawWin(const Paddle& p1, const Paddle& p2) {
	std::string win = "GOAL!";
	float x, y;
	for (auto& paddle : { p1,p2 }) {
		if (LEFT == paddle.side && paddle.win) {
			//在左侧中央位置绘制GOAL！字符串
			x = ((field.innerX) + (field.innerX + field.width / 2.0f) - 2 * win.length() * 8) / 2.0f;
			y = field.innerY + field.height / 2.0f;

		}
		else if (RIGHT == paddle.side && paddle.win) {
			//在右侧中央位置绘制 GOAL! 字符串
			x = ((field.innerX + field.width) + (field.innerX + field.width / 2.0f) - 2 * win.length() * 8) / 2.0f;
			y = field.innerY + field.height / 2.0f;
		}
	}
	
	DrawString(x, y, win, olc::BLACK, 2);
}

void IceHockey::GameReset() {
	puck.InitPuck(field, olc::MAGENTA, this);
	player1.InitPaddle(field, LEFT, olc::RED, olc::DARK_RED, this);
	//ai1.InitPaddle(field, LEFT, olc::RED, olc::DARK_RED, this,&ai2);
	ai2.InitPaddle(field, RIGHT, olc::BLUE, olc::DARK_BLUE, this,&ai1);
}

bool IceHockey::PuckInGoal() {
	float x = puck.position.x;
	float r = puck.radius;
	
	//如果未判断过胜负,则进行判断
	if (!judged) {
		//左侧进球，则右侧胜利
		if (x + r <= field.innerX ) {
			ai2.score++;
			ai2.win = true;
			player1.win = false;
			PlaySound(lose_sound_file, NULL, SND_FILENAME | SND_ASYNC);
			judged = true;
			return true;
		}
		//右侧进球，则左侧胜利
		else if (x - r >= field.innerX + field.width) {
			//ai1.score++;
			player1.score++;
			player1.win = true;
			ai2.win = false;
			PlaySound(win_sound_file,NULL,SND_FILENAME|SND_ASYNC);
			judged = true;
			return true;
		}

	}
	return false;
}

void IceHockey::CollisionResponse(Paddle& paddle, float fElapsedTime) {

	olc::vf2d vPaddle = paddle.v;
	olc::vf2d vRelative = puck.velocity - vPaddle;
	olc::vf2d vDis = puck.position - paddle.pos;
	olc::vf2d vNormal = vDis.norm();//碰撞垂向量
	float vRn = vRelative.dot(vNormal);
	float dis = vDis.mag();
	float sumR = paddle.outerR + puck.radius;
	//距离小于两者半径和，且相对移动速度为正
	if (dis < sumR && vRn < 0.0f) {

		float j = -2.1f * vRn / vNormal.dot(vNormal);
		j /= (1.0f / paddle.mass + 1.0f / puck.mass);
		puck.velocity += j * vNormal * 1.0f / puck.mass;
		paddle.v -= j * vNormal * 1.0f / paddle.mass;

		PlaySound(NULL, 0, 0);//先停止其他声音
		PlaySound(bound_sound_file, NULL, SND_FILENAME | SND_ASYNC);

		//调整位置
		paddle.pos -= (sumR-dis)*vDis.norm();

		if (puck.velocity.mag() > 100.0f) {
			puck.velocity /= 2.0f;
		}
	}

}

void IceHockey::MouseOperate(Paddle& paddle) {
	olc::vi2d mPos = GetMousePos();
	if (!GetMouse(0).bHeld) {
		holdPaddle = false;
	}
	if (!holdPaddle) {
		if ((mPos-paddle.pos).mag() <= paddle.outerR && GetMouse(0).bHeld) {
			holdPaddle = true;
		//	cout << "holdPaddle=" << holdPaddle << endl;
		}
		
	}
	if (true) {
		paddle.lastPos = paddle.pos;
		paddle.pos.x = mPos.x;
		paddle.pos.y = mPos.y;
		if (paddle.pos.x >= ScreenWidth() / 2.0f - paddle.outerR) {
			paddle.pos.x = ScreenWidth() / 2.0f - paddle.outerR;
		}
		if (paddle.pos.x <= field.innerX + paddle.outerR) {
			paddle.pos.x = field.innerX + paddle.outerR;
		}
		if (paddle.pos.y >= field.innerY + field.height - paddle.outerR) {
			paddle.pos.y = field.innerY + field.height - paddle.outerR;
		}
		if (paddle.pos.y <= field.innerY + paddle.outerR) {
			paddle.pos.y = field.innerY + paddle.outerR;
		}
		paddle.v = paddle.pos - paddle.lastPos;
		while (paddle.v.mag() > SPEED_MAX) {
			paddle.v /= 2.0F;
		}
	}
	
	//cout << "player.v.mag = " << paddle.v.mag() << endl;
}

void IceHockey::DrawScore(int s1,int s2) {
	std::string str;
	str += std::to_string(s1);
	str += ":";
	str += std::to_string(s2);
	DrawString((ScreenWidth() - str.length() * 4 * 8) / 2.0f, 65, str, olc::BLACK, 4);
}

void IceHockey::Rendering() {
	//绘制背景,太耗时了
	// Sprites live in RAM and are accessed and manipulated by the CPU. 
	//DrawSprite(0, 0, bgSprite.get());
	//A decal is a sprite that lives on the GPU
	// The GPU will draw the decal on top of whatever was drawn by the CPU first. 
	//SetDrawTarget(bgSprite.get());
	SetDecalMode(olc::DecalMode::MULTIPLICATIVE);
	DrawDecal({ 10,10 }, bgDecal.get());
	DrawDecal({ ScreenWidth()-ringSprite.get()->width-10.0f ,5.0f }, ringDecal.get());
	DrawDecal({ (ScreenWidth() - logoSprite.get()->width) / 2.0f ,(ScreenHeight() - logoSprite.get()->height) / 2.0f }, logoDecal.get());
	DrawDecal({ ScreenWidth()/2.0f - chinaSprite.get()->width - 4.0f ,5.0f }, chinaDecal.get());
	DrawDecal({ ScreenWidth()/2.0f + 4.0f ,5.0f }, franceDecal.get());

	SetDrawTarget(nullptr);
	//绘制比分
	//DrawScore(ai1.score,ai2.score);
	DrawScore(player1.score,ai2.score);
	//先绘制场地
	field.DrawField();
	//再绘制冰球
	puck.DrawPuck();
	//绘制边框
	field.DrawBarrier();
	//绘制球拍
	player1.DrawPaddle();
	//ai1.DrawPaddle();
	ai2.DrawPaddle();
}


void IceHockey::AiResponseStrong(AiPaddle& paddle,float fElapsedTime) {
	olc::vf2d nMove = { 0.0f,0.0f };
	float disX = paddle.pos.x - puck.position.x;
	//默认方向是朝向球的方向
	nMove = (puck.position - paddle.pos).norm();

	//进攻策略,当球未越过球拍时采用
	if ((paddle.side == LEFT && disX <0) || (paddle.side==RIGHT && disX>0)) {

		if ((puck.position- paddle.pos).norm().dot((paddle.posEnemyGoal - paddle.pos).norm()) > 0.7f) {
			//如果球拍到球的和敌方球门的夹角较小,则大力进攻
			nMove = (puck.position - paddle.pos).norm();
		}else if ((puck.position-paddle.pos).mag()/1.5f<puck.radius+paddle.outerR && ((puck.position - paddle.pos).norm()).dot((paddle.enemy->pos - paddle.pos).norm()) > 0.5f) {
			//如果球和敌人的夹角较小,则侧面突击
			nMove = (paddle.enemy->pos - puck.position).polar().norm();
			DrawLine(paddle.pos+ (paddle.enemy->pos - puck.position).polar().norm()*field.height, paddle.pos, olc::GREEN);
		}

	}
	//防守策略,当球已越过球拍时采用
	else if (puck.velocity.mag() > paddle.speedEasy) {
		olc::vf2d pCenter = (puck.position + paddle.posGoal) / 2.0f;
		nMove = (pCenter - paddle.pos).norm();
	}

	paddle.v = paddle.speedEasy * nMove;

	paddle.Move(fElapsedTime);
	if (LEFT == paddle.side) {
		BoundBarrier(paddle.pos, NULL, paddle.outerR, { field.innerX,field.innerY }, field.width / 2.0f, field.height);
	}
	else {
		BoundBarrier(paddle.pos, NULL, paddle.outerR, { field.innerX + field.width / 2.0f,field.innerY }, field.width / 2.0f, field.height);
	}

}
void IceHockey::AiResponse(AiPaddle& paddle,float fElapsedTime) {
	olc::vf2d nMove;

	//冰球速度大于球拍速度,且冰球速度方向朝向球门,则防守
	if (puck.velocity.mag()>=paddle.speedEasy&&(paddle.posGoal - puck.position).dot(puck.velocity) > 0.001f) {

		olc::vf2d posCenter = (paddle.posGoal + puck.position) / 2.0f;
		nMove = (posCenter - paddle.pos).norm();
	}
	else {
		//进攻
		//速度方向指向冰球
		nMove = (puck.position - paddle.pos).norm();
	}

	paddle.v = paddle.speedEasy * nMove;

	paddle.Move(fElapsedTime);
	if (LEFT == paddle.side) {
		BoundBarrier(paddle.pos, NULL, paddle.outerR, {field.innerX,field.innerY},field.width/2.0f,field.height);
	}
	else {
		BoundBarrier(paddle.pos, NULL, paddle.outerR, { field.innerX+field.width/2.0f,field.innerY }, field.width / 2.0f, field.height);
	}
	
}

void Field::DrawEllipse(float a, float b, olc::Pixel c) {
	int x, y;
	float d1, d2;
	x = 0;
	y = b;
	p->Draw(x, y, c);
	d1 = b * b - a * a * b + a * a / 4;//b^2-a^2b+a^2/4
	/*Region 1*/
	while (a * a * (y - 0.5f) > b * b * (x + 1)) {//a^2(y-1/2) > b^2(x+1)
		if (d1 < 0) {
			d1 += b * b * (2 * x + 3);//b^2(2x+3)
		}
		else {
			d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);//b^2(2x+3)+a^2(-2y+2)
			y--;
		}
		x++;
		p->Draw(x, y, c);
	}
	//d2 = b^2(x+1/2)^2 + a^2(y-1)^2 -a^2b^2
	d2 = b * b * (x + 0.5f) * (x + 0.5f) + a * a * (y - 1) * (y - 1) - a * a * b * b;
	/*Region 2*/
	while (y > 0) {
		if (d2 < 0) {
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);//b^2(2x+2) + a^2(-2y+3)
			x++;
		}
		else {
			d2 += a * a * (-2 * y + 3);//a^2(-2y+3)
		}
		y--;
		p->Draw(x, y, c);
	}
}

void Field::InitField(float w, float h, float gw, float b,olc::PixelGameEngine* p) {
	this->p = p;
	width = w;
	height = h;
	goalWidth = gw;
	border = b;

	innerX = (p->ScreenWidth() - width) / 2.0f;
	innerY = (p->ScreenHeight() - height) / 2.0f;

	outterX = innerX - border;
	outterY = innerY - border;

	goalLeft.x = innerX;
	goalLeft.y = (p->ScreenHeight() - goalWidth) / 2.0f;

	goalRight.x = (p->ScreenWidth() + width) / 2.0f;
	goalRight.y = goalLeft.y;
}
void Field::DrawSpot(float x,float y,float r,olc::Pixel c,bool drawCentral) {
	p->DrawCircle(x, y, r, c);
	p->DrawCircle(x, y, r - 1, c);
	if (drawCentral) {
		p->FillCircle(x, y, 3, c);
	}
}

void Field::DrawZoneLine(float x1, float y1, float x2,float y2, olc::Pixel c) {
	p->DrawLine(x1, y1, x2,y2, c);
	p->DrawLine(x1+1, y1, x2+1, y2, c);
	p->DrawLine(x1-1, y1, x2-1, y2, c);
}
void Field::DrawField() {
	DrawEllipse(100, 100, olc::GREEN);
	DrawEllipse(700, 500, olc::GREEN);

	//绘制内圈
	// Fill 函数很耗时,尽量少用
	//p->FillRect(innerX, innerY, width, height);

	//绘制中线
	DrawZoneLine(p->ScreenWidth() / 2.0f, innerY, p->ScreenWidth() / 2.0f, innerY + height, olc::RED);

	//绘制中央圆
	float centralR = height / heightRadiusRatio*1.5;
	float centralX = p->ScreenWidth() / 2.0f;
	float centralY = p->ScreenHeight() / 2.0f;
	DrawSpot(centralX, centralY, centralR, olc::BLUE);
	

	//绘制两侧4个圆
	float sideR = height / heightRadiusRatio;
	DrawSpot(innerX + 2 * sideR, innerY + 2 * sideR,sideR,olc::RED);
	DrawSpot(innerX + width- 2 * sideR, innerY + 2 * sideR, sideR, olc::RED);
	DrawSpot(innerX + 2 * sideR, innerY +height- 2 * sideR, sideR, olc::RED);
	DrawSpot(innerX + width-2 * sideR, innerY +height- 2 * sideR, sideR, olc::RED);
	
	//绘制防守线
	float defenseX = innerX + width / 4.0f;
	DrawZoneLine(defenseX, innerY, defenseX, innerY + height, olc::BLUE);
	DrawZoneLine(defenseX + width / 2.0f, innerY, defenseX + width / 2.0f, innerY + height, olc::BLUE);

	//绘制球门
	DrawSpot(goalLeft.x, goalLeft.y + goalWidth / 2.0f, goalWidth/2.0f, olc::GREEN,false);
	DrawSpot(goalRight.x, goalRight.y + goalWidth / 2.0f, goalWidth / 2.0f, olc::GREEN,false);
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
	// 设置随机数种子
	srand(time(0));

	// 生成一个0到99之间的随机数
	int random_number = rand() % 100;
	if (random_number > 50) {
		position.y = f.innerY + radius;
		velocity = { 0,1.0f };
	}
	else {
		position.y = f.innerY +f.height- radius;
		velocity = { 0,-1.0f };
	}
	
	
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

void Puck::Move(float fElapsedTime) {
	bool bound = false;
	//p->DrawRect(position.x - radius, position.y - radius, radius * 2, radius * 2, olc::RED);

	if (position.y - radius >= f.goalLeft.y && position.y + radius <= f.goalLeft.y + f.goalWidth) {
		//如果冰球上下边沿都在在球门范围内,则不必检测碰撞
		bound = false;
	}
	else if(position.y >= f.goalLeft.y && position.y <= f.goalLeft.y + f.goalWidth) {
		//如果冰球的球心在球门范围内,检测碰撞点
		olc::vf2d leftUp, leftDown, rightUp, rightDown;//待检测的4个碰撞点
		olc::vf2d vRelative={0.0f,0.0f};//相对速度
		olc::vf2d vN;//碰撞垂向量
		leftUp =  f.goalLeft;
		leftDown = { leftUp.x,leftUp.y + f.goalWidth };
		rightUp = f.goalRight;
		rightDown = { rightUp.x,rightUp.y + f.goalWidth };
		if ((position - leftUp).mag() < radius) {
			vN = (position - leftUp).norm();
			vRelative = velocity.dot(vN) * vN;
		}
		else if ((position - leftDown).mag() < radius) {
			vN = (position - leftDown).norm();
			vRelative = velocity.dot(vN) * vN;
		}
		else if ((position - rightUp).mag() < radius) {
			vN = (position - rightUp).norm();
			vRelative = velocity.dot(vN) * vN;
		}
		else if ((position - rightDown).mag() < radius) {
			vN = (position - rightDown).norm();
			vRelative = velocity.dot(vN) * vN;
		}
		if (vRelative.mag() > 0.01f) {
			bound = true;
			velocity += -2.0f * vRelative;
		}
	}
	else {
		bound = BoundBarrier(position, &velocity, radius, { f.innerX,f.innerY }, f.width, f.height);
	}

	if (bound) {
		PlaySound(NULL, 0, 0);//先停止其他所有声音,再播放当前音效
		PlaySound(bound_sound_file, NULL, SND_FILENAME | SND_ASYNC);
	}
	while (velocity.mag() > SPEED_MAX) {
		velocity /= 2.0f;
	}

	float ratio = fElapsedTime * p->GetFPS();
	//cout << "ratio=" << ratio<<endl;
	position.x += velocity.x*ratio;
	position.y += velocity.y*ratio;
	velocity *= (1.0f-f.friction);

}

void Paddle::InitPaddle(const Field& f,Side side, olc::Pixel inCol, olc::Pixel outCol, olc::PixelGameEngine* p) {
	this->p = p;
	this->f = f;
	v = { 0.0f,0.0f };
	innerR = f.goalWidth / goalPaddleRatio / 1.5f;
	outerR = f.goalWidth / goalPaddleRatio;
	this->side = side;
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

void Paddle::Move(float fElapsedTime) {
	static float  ratioMax = 0;
	float nextX, nextY;
	nextX = pos.x;
	nextY = pos.y;

	if (nextX - outerR < f.innerX || nextX + outerR>f.innerX + f.width) {
		pos.x = -v.x;
	}
	if (nextY - outerR < f.innerY || nextY + outerR > f.innerY + f.height) {
		pos.y = -v.y;
	}
	float ratio = fElapsedTime * p->GetFPS();
	while (v.mag() < speedEasy) {
		v *= 2.0f;
	}

	while (v.mag() > SPEED_MAX) {
		v / 2.0f;
	}

	pos.x += v.x* ratio;
	pos.y += v.y* ratio;
	if (ratio > ratioMax) {
		ratioMax = ratio;
		//cout << "max ratio = " << ratioMax << endl;
	}
	
}

void AiPaddle::InitPaddle(const Field& f, Side side, olc::Pixel inCol, olc::Pixel outCol, olc::PixelGameEngine* p,const Paddle* enemy) {
	this->p = p;
	this->f = f;
	this->enemy = enemy;
	v = { 0.0f,0.0f };
	innerR = f.goalWidth / goalPaddleRatio / 1.5f;
	outerR = f.goalWidth / goalPaddleRatio;
	this->side = side;
	if (LEFT == side) {
		pos.x = f.innerX + outerR;
		posGoal = { f.goalLeft.x , f.goalRight.y + f.goalWidth / 2.0f };
		posEnemyGoal = { f.goalRight.x , f.goalRight.y + f.goalWidth / 2.0f };
	}
	else {
		pos.x = f.innerX + f.width - outerR;
		posGoal = { f.goalRight.x , f.goalRight.y + f.goalWidth / 2.0f };
		posEnemyGoal = { f.goalLeft.x , f.goalRight.y + f.goalWidth / 2.0f };
	}
	pos.y = f.innerY + f.height / 2.0f;

	innerCol = inCol;
	outerCol = outCol;
}