#ifndef INCLUDE_ASTEROID
#define INCLUDE_ASTEROID
#include "Asteroid.h"
#endif 

int main() {
	Asteroid asteroid;
	

	if (asteroid.Construct(300, 200, 2, 2)) {
		// 获取当前活动窗口的句柄
		HWND hWnd = GetForegroundWindow();

		// 将当前活动窗口设置为焦点
		SetForegroundWindow(hWnd);
		asteroid.Start();
	}
}