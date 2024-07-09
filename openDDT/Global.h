#pragma once
enum Direction {
	LEFT = 0,
	RIGHT,
};

enum State {
	Idle=0,
	GoingLeft,
	GoingRight,
	GettingAngle,
	GettingForce,
	Shooting,
	End,
};