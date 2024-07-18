#include "SDL.h"
#include "SDL_net.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
	if (0 != SDLNet_Init()) {
		//初始化失败
		return 0;
	}
	atexit(SDLNet_Quit);//注册一个程序终止时执行的退出函数

	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, 16);
	cout << "Local Host: " << SDLNet_ResolveIP(&ip) << endl;
	return 0;
}