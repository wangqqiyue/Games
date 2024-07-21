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

	// ��ȡ����̨��Ļ��������Ϣ
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		printf("Failed to get console screen buffer info.\n");
		return 1;
	}
	

	// �������һ�е�λ��
	lastLinePos.X = 0;
	lastLinePos.Y = csbi.dwSize.Y - 1;
	// �ƶ���굽���һ��
	SetConsoleCursorPosition(hConsole, lastLinePos);

	// ��յ�ǰ��
	FillConsoleOutputCharacterA(hConsole, ' ', csbi.dwSize.X, lastLinePos, &written);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, csbi.dwSize.X, lastLinePos, &written);

	// �ٴ��ƶ���굽���һ�п�ʼ
	SetConsoleCursorPosition(hConsole, lastLinePos);
}

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

int main(int argc, char* argv[]) {
	IPaddress ip;
	TCPsocket socket;

	if (0 != SDLNet_Init()) {
		//��ʼ��ʧ��
		return 0;
	}
	atexit(SDLNet_Quit);//ע��һ��������ֹʱִ�е��˳�����

	const char* dstip = "116.198.37.2";//116.198.37.2  192.168.3.128
	SDLNet_ResolveHost(&ip, dstip, 8888);
	
	socket = SDLNet_TCP_Open(&ip);
	char buf[512] = { 0 };
	
	if (socket) {
		cout << "�����ӵ�������." << endl;
		cout << "�����������Ϣ����exit�˳�������" << endl;
	}
	else {
		cout << "�޷����ӵ�������" << endl;
	}
	thread th(dohandler, socket);//�̴߳���
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