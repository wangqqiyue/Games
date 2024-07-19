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
		//��ʼ��ʧ��
		return 0;
	}
	atexit(SDLNet_Quit);//ע��һ��������ֹʱִ�е��˳�����

	
	SDLNet_ResolveHost(&ip, NULL, 12345);
	cout << "Local Host: " << SDLNet_ResolveIP(&ip) << endl;
	socket = SDLNet_TCP_Open(&ip);
	char buf[512] = { 0 };
	while (true) {
		cout << "������Ҫ���͵�����:";
		cin >> buf;
		int len = strlen(buf);
		SDLNet_TCP_Send(socket, buf, len);
	}
	SDLNet_TCP_Close(socket);
	SDLNet_Quit();

	return 0;
}