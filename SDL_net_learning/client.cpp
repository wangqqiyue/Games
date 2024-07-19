#include "SDL.h"
#include "SDL_net.h"
#include <iostream>

using std::cout;
using std::endl;
using std::cin;

int main(int argc, char* argv[]) {
	IPaddress ip;
	TCPsocket socket;

	if (0 != SDLNet_Init()) {
		//初始化失败
		return 0;
	}
	atexit(SDLNet_Quit);//注册一个程序终止时执行的退出函数

	const char* dstip = "127.0.0.1";
	SDLNet_ResolveHost(&ip, dstip, 12345);
	
	socket = SDLNet_TCP_Open(&ip);
	char buf[512] = { 0 };
	while (true) {
		cout << "请输入:";
		memset(buf, 0, 512);
		gets_s(buf, 512);
		int len = strlen(buf);
		SDLNet_TCP_Send(socket, buf, len);
	}
	SDLNet_TCP_Close(socket);
	SDLNet_Quit();

	return 0;
}