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


//�������ݴ����߳�
void dohandler(TCPsocket sd)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// ��ȡ����̨��Ļ��������Ϣ
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
			// �ָ���ԭʼλ��
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

//ϵͳ�źŹ���
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// CTRL-CLOSE: �رմ���ʱ�洢��ǰ��Ϣ���ⲿ�ļ�,���˳�ϵͳ 
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
		//��ʼ��ʧ��
		return 0;
	}


	const char* dstip = "116.198.37.2";//116.198.37.2  192.168.3.128
	SDLNet_ResolveHost(&ip, dstip, 8888);
	
	g_socket = SDLNet_TCP_Open(&ip);
	char buf[512] = { 0 };
	
	if (g_socket) {
		cout << "�����ӵ�������." << endl;
		cout << "�����������Ϣ����exit�˳�������" << endl;
	}
	else {
		cout << "�޷����ӵ�������" << endl;
	}
	thread th(dohandler, g_socket);//�̴߳���
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