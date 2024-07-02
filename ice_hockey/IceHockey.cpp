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
void IceHockey::KeyOperation() {
	if (GetKey(olc::UP).bReleased) {
		if (SPEED_MAX < 100) {
			SPEED_MAX++;
			puck.SPEED_MAX = SPEED_MAX;
		}
	}
	else if (GetKey(olc::DOWN).bReleased) {
		if (SPEED_MAX > 1) {
			SPEED_MAX--;
			puck.SPEED_MAX = SPEED_MAX;
		}
	}

	if (GetKey(olc::RIGHT).bReleased) {
		if (maxFPS < 100) {
			maxFPS++;
			SetMaxFPS(maxFPS);
		}
	}
	else if (GetKey(olc::LEFT).bReleased) {
		if (maxFPS > 1) {
			maxFPS--;
			SetMaxFPS(maxFPS);
		}
	}

}

void IceHockey::DrawFPS() {
	std::string fpsStr = "FPS:";
	fpsStr += std::to_string(maxFPS);
	fpsStr += "(press Left/Right to change.)";
	float x, y;
	x = (ScreenWidth() - fpsStr.length() * 8 * 2) / 2.0f;
	y = field.innerY + field.height + 8 * 6;

	DrawString(x, y, fpsStr, olc::BLACK, 2);
}

void IceHockey::DrawSpeed() {
	std::string speedStr = "SPEED:";
	speedStr += std::to_string(SPEED_MAX);
	speedStr += "(press Up/Down to change.)";
	float x, y;
	x = (ScreenWidth() - speedStr.length() * 8 * 2) / 2.0f;
	y = field.innerY + field.height + 8 * 2;

	DrawString(x, y, speedStr, olc::BLACK, 2);
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
	
	if (aiPlay) {
		ai1.InitPaddle(field, LEFT, olc::RED, olc::DARK_RED, this, &ai2);
		player1 = ai1;
	}
	else {
		player1.InitPaddle(field, LEFT, olc::RED, olc::DARK_RED, this);
	}
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

void IceHockey::CollisionResponse(Paddle& paddle) {

	olc::vf2d vPaddle = paddle.v;
	olc::vf2d vRelative = puck.velocity - vPaddle;
	olc::vf2d vDis = puck.position - paddle.pos;
	olc::vf2d vNormal = vDis.norm();//碰撞垂向量
	float vRn = vRelative.dot(vNormal);
	float dis = vDis.mag();
	float sumR = paddle.outerR + puck.radius;
	//距离小于两者半径和，且相对移动速度为正
	if (dis < sumR && vRn<0.0f) {

		float j = -2.0f * vRn / vNormal.dot(vNormal);
		j /= (1.0f / paddle.mass + 1.0f / puck.mass);
		puck.velocity += j * vNormal * 1.0f / puck.mass;
		paddle.v -= j * vNormal * 1.0f / paddle.mass;

		PlaySound(NULL, 0, 0);//先停止其他声音
		PlaySound(bound_sound_file, NULL, SND_FILENAME | SND_ASYNC);

		//调整位置
		paddle.pos -= (sumR-dis)*vDis.norm();

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
	}
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
	if (aiPlay) {
		ai1.DrawPaddle();
	}
	else {
		player1.DrawPaddle();
	}
	ai2.DrawPaddle();

	DrawSpeed();
	DrawFPS();
}


void IceHockey::AiResponseStrong(AiPaddle& paddle) {
	paddle.speedEasy = SPEED_MAX / 10;
	paddle.speedNormal = SPEED_MAX / 5;
	paddle.speedHard = SPEED_MAX/2;
	olc::vf2d nMove = { 0.0f,0.0f };
	olc::vf2d pCenter= { ScreenWidth() / 2.0f,ScreenHeight() / 2.0f };
	float disX = paddle.pos.x - puck.position.x;
	//默认方向是朝向球的方向
	nMove = (puck.position - paddle.pos).norm();
	srand(time(NULL));
	//如果球在对方半场
	if ((paddle.side == LEFT && puck.position.x -puck.radius > ScreenWidth()/2.0f) || (paddle.side==RIGHT  && puck.position.x+puck.radius < ScreenWidth() / 2.0f)) {
		//防守策略
		olc::vf2d pIntercept = (puck.position + paddle.posGoal ) /2.0f;
		if ((pIntercept - paddle.pos).mag() < 2*paddle.outerR) {
			return;
		}
		nMove = (pIntercept-paddle.pos).norm();
		nMove *= {0.9f + 0.2f * rand() / RAND_MAX, 0.9f + 0.2f * rand() / RAND_MAX};
		paddle.v = paddle.speedEasy * nMove;
		
		return;
	}

	//球在己方半场
	
	//进攻策略,当球未越过球拍时采用
	if ((paddle.side == LEFT && disX <= 0) || (paddle.side == RIGHT && disX >= 0)) {
		//如果球拍到球的和敌方球门的夹角较小,则大力进攻
		if ((puck.position - paddle.pos).norm().dot((paddle.posEnemyGoal - paddle.pos).norm()) > 0.7f) {
			if ((puck.position - paddle.pos).mag() <=  1.2f*(paddle.outerR + puck.radius)) {
				nMove = (paddle.posEnemyGoal - paddle.pos).norm();
				nMove *= 2.0f;
			}
			else {
				nMove = (puck.position - paddle.pos).norm();
			}
		}
		//绕道球后面
		else  {
			nMove = (puck.position - paddle.posEnemyGoal).norm() + (puck.position - paddle.pos).norm();
		}
		
		nMove *= {0.9f + 0.2f * rand() / RAND_MAX, 0.9f + 0.2f * rand() / RAND_MAX};
		paddle.v = paddle.speedNormal * nMove;
		//cout << "I'm attacking." << endl;
		return;
	}

	//防守策略,当球已越过球拍时采用
	olc::vf2d pIntercept = paddle.posGoal;//拦截点位置
	//cout << "I'm defensing." << endl;
	//如果球拍击球会导致球进入自己球门,则迂回绕开
	if (puck.velocity.mag()>paddle.speedEasy) {
		//球拍去拦截球
		nMove = (pIntercept - paddle.pos).norm();
	}
	else {
		nMove = (puck.position - paddle.pos).norm();
	}
	nMove *= {0.6f+0.8f*rand()/RAND_MAX, 0.6f+0.8f* rand() / RAND_MAX};
	paddle.v = paddle.speedEasy * nMove;
	return;
}
void IceHockey::AiResponse(AiPaddle& paddle) {
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

	paddle.Move();
	if (LEFT == paddle.side) {
		BoundBarrier(paddle.pos, NULL, paddle.outerR, {field.innerX,field.innerY},field.width/2.0f,field.height);
	}
	else {
		BoundBarrier(paddle.pos, NULL, paddle.outerR, { field.innerX+field.width/2.0f,field.innerY }, field.width / 2.0f, field.height);
	}
	
}

//start提供x坐标,end 提供y坐标,d代表绘制的弧线在start-end连线的什么方向
void Field::DrawArc(olc::vf2d start , olc::vf2d end, olc::Pixel c, Direction d) {
	olc::vf2d center;
	olc::vi2d rotate;
	int x, y;
	int a, b;
	float d1, d2;

	a = abs(start.x - end.x);
	b = abs(start.y - end.y);
	center.x = start.x;
	center.y = end.y;
	x = 0;
	y = b;
	switch (d) {
	case NW:
		rotate.x = -1;
		rotate.y = -1;
	
		break;
	case NE:
		rotate.x = 1;
		rotate.y = -1;
		break;
	case SW:
		rotate.x = -1;
		rotate.y = 1;
		break;
	case SE:
		rotate.x = 1;
		rotate.y = 1;
		break;
	}
	p->Draw(center.x + rotate.x*x, center.y + rotate.y*y, c);


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
		p->Draw(center.x + rotate.x * x, center.y + rotate.y * y, c);
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
		p->Draw(center.x + rotate.x * x, center.y + rotate.y * y, c);
	}
}

void Field::InitField(float w, float h, float gw, float b, olc::PixelGameEngine* p) {
	this->p = p;
	width = w;
	height = h;
	goalWidth = gw;
	border = b;

	innerX = (p->ScreenWidth() - width) / 2.0f;
	innerY = (p->ScreenHeight() - height) / 2.0f;

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
	//左
	for (int i = 0; i < border; i++) {
		p->DrawLine(innerX-i, innerY, innerX-i, height + innerY, olc::RED);
	}
	

	//右
	for (int i = 0; i < border; i++) {
		p->DrawLine(innerX+i + width, innerY, innerX+i + width, height + innerY, olc::RED);
	}

	//上
	for (int i = 0; i < border; i++) {
		p->DrawLine(innerX, innerY-i, innerX + width, innerY-i, borderColor);
	}

	//下
	for (int i = 0; i < border; i++) {
		p->DrawLine(innerX, innerY + height+i, innerX + width, innerY + height+i, borderColor);
	}

	//左上角弧度
	for (int i = 0; i < border; i++) {
		for (int j = 0; j < border; j++) {
			DrawArc({ innerX - i,innerY - j }, { innerX - i - width / 10.f,innerY - j + height / 10.0f }, borderColor, NW);
		}
	}

	//左侧二道墙
	for (int i = 0; i < border; i++) {
		p->DrawLine(innerX -i- width / 10, innerY + height / 10, innerX -i- width / 10, innerY + height * 9 / 10, borderColor);
	}

	//左下角弧度
	for (int i = 0; i < border; i++) {
		for (int j = 0; j < border; j++) {
			DrawArc({ innerX - i,innerY + height + j }, { innerX - i - width / 10.f,innerY + j + height * 9.0f / 10.0f }, borderColor, SW);
		}
	}

	//右上角弧度
	for (int i = 0; i < border; i++) {
		for (int j = 0; j < border; j++) {
			DrawArc({ innerX + i + width,innerY - j }, { innerX + i + width * 11 / 10.f,innerY - j + height / 10.0f }, borderColor, NE);
		}
	}

	//右侧二道墙
	for (int i = 0; i < border; i++) {
		p->DrawLine(innerX+i + width * 11 / 10, innerY + height / 10, innerX+i + width * 11 / 10, innerY + height * 9 / 10, borderColor);
	}
	//右下角弧度
	for (int i = 0; i < border; i++) {
		for (int j = 0; j < border; j++) {
			DrawArc({ innerX + i + width,innerY + j + height }, { innerX + i + width * 11 / 10.f,innerY + j + height * 9.0f / 10.0f }, borderColor, SE);
		}
	}
}

void Puck::InitPuck(const Field& f,olc::Pixel col, olc::PixelGameEngine*p) {
	this->p = p;
	this->f = f;
	position.x = f.innerX+f.width/2.0f;
	// 设置随机数种子
	srand(time(0));

	// 生成一个0到99之间的随机数
	int random_number = rand() % 100;
	if (random_number %2) {
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

void Puck::Move() {
	bool bound = false;

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

	position.x += velocity.x;
	position.y += velocity.y;
	
	velocity *= (1.0f-f.friction);//固定的摩擦阻力
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
	speedEasy = SPEED_MAX / 10;
	speedNormal = SPEED_MAX / 2;
	speedHard = SPEED_MAX ;
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
	while (v.mag() < speedEasy) {
		v *= 2.0f;
	}

	pos.x += v.x;
	pos.y += v.y;

	if (LEFT == side) {
		BoundBarrier(pos, NULL, outerR, { f.innerX,f.innerY }, f.width / 2.0f, f.height);
	}
	else {
		BoundBarrier(pos, NULL, outerR, { f.innerX + f.width / 2.0f,f.innerY }, f.width / 2.0f, f.height);
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