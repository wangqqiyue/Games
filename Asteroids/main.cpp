#ifndef INCLUDE_ASTEROID
#define INCLUDE_ASTEROID
#include "Asteroid.h"
#endif 

int main() {
	Asteroid asteroid;
	

	if (asteroid.Construct(600, 400, 1, 1)) {
		// ��ȡ��ǰ����ڵľ��
		HWND hWnd = GetForegroundWindow();

		// ����ǰ���������Ϊ����
		SetForegroundWindow(hWnd);
		asteroid.Start();
	}
}