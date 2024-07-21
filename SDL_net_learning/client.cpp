#include "SDL.h"
#include "SDL_net.h"
#include <iostream>
#include <thread>
#include <Windows.h>
#include <signal.h>

using std::cout;
using std::endl;
using std::cin;
using std::thread;
bool g_quit = false;


//接收数据处理线程
void dohandler(TCPsocket sd)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// 获取控制台屏幕缓冲区信息
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		printf("Failed to get console screen buffer info.\n");
		return;
	}
	COORD  originalPos = csbi.dwCursorPosition;
	char buffer[512] = { 0 };
	while (!g_quit)
	{
		memset(buffer, 0, 512);
		int len = SDLNet_TCP_Recv(sd, buffer, 512);
		if (len > 0)
		{
			// 恢复到原始位置
			SetConsoleCursorPosition(hConsole, originalPos);
			cout << buffer << endl;
			GetConsoleScreenBufferInfo(hConsole, &csbi);
			originalPos = csbi.dwCursorPosition;
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
TCPsocket g_socket = NULL;

//系统信号钩子
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// CTRL-CLOSE: 关闭窗口时存储当前信息到外部文件,再退出系统 
	case CTRL_CLOSE_EVENT:
		SDLNet_TCP_Send(g_socket, "exit", 4);
		SDLNet_TCP_Close(g_socket);
		SDLNet_Quit();
		return TRUE;

	default:
		return FALSE;
	}
}

int main(int argc, char* argv[]) {
	IPaddress ip;
	SetConsoleCtrlHandler(CtrlHandler, TRUE);

	if (0 != SDLNet_Init()) {
		//初始化失败
		return 0;
	}


	const char* dstip = "116.198.37.2";//116.198.37.2  192.168.3.128
	SDLNet_ResolveHost(&ip, dstip, 8888);
	
	g_socket = SDLNet_TCP_Open(&ip);
	char buf[512] = { 0 };
	
	if (g_socket) {
		cout << "已连接到服务器." << endl;
		cout << "请输入你的消息，以exit退出服务器" << endl;
	}
	else {
		cout << "无法连接到服务器" << endl;
	}
	thread th(dohandler, g_socket);//线程处理
	th.detach();

	Sleep(3000);
	while (!g_quit) {
		//clearLastLine();

		memset(buf, 0, 512);
		gets_s(buf, 512);
		int len = strlen(buf);
		SDLNet_TCP_Send(g_socket, buf, len);

		if (0 == strcmp(buf, "exit")) {
			g_quit = true;
			break;
		}
	}
	SDLNet_TCP_Close(g_socket);
	SDLNet_Quit();

	return 0;
}