#include "SDL.h"
#include "SDL_net.h"
#include <iostream>
#include <thread>
#include <Windows.h>

using std::cout;
using std::endl;
using std::cin;
using std::thread;
bool g_quit = false;

int clearLastLine() {
	HANDLE hConsole;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD written;
	COORD lastLinePos;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) {
		printf("Failed to get console handle.\n");
		return 1;
	}

	// 获取控制台屏幕缓冲区信息
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		printf("Failed to get console screen buffer info.\n");
		return 1;
	}
	

	// 计算最后一行的位置
	lastLinePos.X = 0;
	lastLinePos.Y = csbi.dwSize.Y - 1;
	// 移动光标到最后一行
	SetConsoleCursorPosition(hConsole, lastLinePos);

	// 清空当前行
	FillConsoleOutputCharacterA(hConsole, ' ', csbi.dwSize.X, lastLinePos, &written);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, csbi.dwSize.X, lastLinePos, &written);

	// 再次移动光标到最后一行开始
	SetConsoleCursorPosition(hConsole, lastLinePos);
}

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

int main(int argc, char* argv[]) {
	IPaddress ip;
	TCPsocket socket;

	if (0 != SDLNet_Init()) {
		//初始化失败
		return 0;
	}
	atexit(SDLNet_Quit);//注册一个程序终止时执行的退出函数

	const char* dstip = "116.198.37.2";//116.198.37.2  192.168.3.128
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

	Sleep(3000);
	while (!g_quit) {
		//clearLastLine();

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