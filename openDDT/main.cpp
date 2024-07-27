#include <iostream>
#include "Game.h"
#include "NetworkManager.h"
using std::cout;
using std::endl;
//系统信号钩子
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// CTRL-CLOSE: 关闭窗口时存储当前信息到外部文件,再退出系统 
	case CTRL_CLOSE_EVENT:
		cout << "SDLNet_Quit." << endl;
		SDLNet_TCP_Send(TheNetworkManager::m_socket, "exit", 4);
		SDLNet_TCP_Close(TheNetworkManager::m_socket);
		SDLNet_Quit();
		return TRUE;

	default:
		return FALSE;
	}
}

int main(int argc, char* args[])
{
	SetConsoleCtrlHandler(CtrlHandler, TRUE);
	std::cout << "game init attempt...\n";
	if (TheGame::Instance()->init("Chapter 1", 100, 100, 800, 600, false))
	{
		std::cout << "game init success!\n";
		cout << TheGame::Instance()->running() << endl;
		while (TheGame::Instance()->running())
		{
			TheGame::Instance()->handleEvents();
			TheGame::Instance()->update();
			TheGame::Instance()->render();

			SDL_Delay(10);
		}
	}
	else
	{
		std::cout << "game init failure - " << SDL_GetError() << "\n";
		return -1;
	}

	std::cout << "game closing...\n";
	TheGame::Instance()->clean();

	return 0;
}


