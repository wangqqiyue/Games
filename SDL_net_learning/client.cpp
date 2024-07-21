#include "SDL.h"
#include "SDL_net.h"
#include <iostream>
#include <thread>

using std::cout;
using std::endl;
using std::cin;
using std::thread;
bool g_quit = false;

//接收数据处理线程
void dohandler(TCPsocket sd)
{
	char buffer[512] = { 0 };
	while (!g_quit)
	{
		memset(buffer, 0, 512);
		int len = SDLNet_TCP_Recv(sd, buffer, 512);
		if (len > 0)
		{
			cout << buffer << endl;
			if (0 == strcmp(buffer, "Goodbye")) {
				g_quit = true;
				break;
			}
		}
		else
		{
			break;
		}
	}
}

int main(int argc, char* argv[]) {
	IPaddress ip;
	TCPsocket socket;

	if (0 != SDLNet_Init()) {
		//初始化失败
		return 0;
	}
	atexit(SDLNet_Quit);//注册一个程序终止时执行的退出函数

	const char* dstip = "192.168.3.128";//116.198.37.2  192.168.3.128
	SDLNet_ResolveHost(&ip, dstip, 8888);
	
	socket = SDLNet_TCP_Open(&ip);
	char buf[512] = { 0 };
	
	if (socket) {
		cout << "已连接到服务器." << endl;
		cout << "请输入你的消息，以exit退出服务器" << endl;
	}
	else {
		cout << "无法连接到服务器" << endl;
	}
	thread th(dohandler, socket);//线程处理
	th.detach();

	while (!g_quit) {
		cout << "请输入:";
		memset(buf, 0, 512);
		gets_s(buf, 512);
		int len = strlen(buf);
		SDLNet_TCP_Send(socket, buf, len);

		if (0 == strcmp(buf, "exit")) {
			g_quit = true;
			break;
		}
	}
	SDLNet_TCP_Close(socket);
	SDLNet_Quit();

	return 0;
}