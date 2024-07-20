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
	while (true) {
		cout << "������:";
		memset(buf, 0, 512);
		gets_s(buf, 512);
		int len = strlen(buf);
		SDLNet_TCP_Send(socket, buf, len);
		if (0 == strcmp(buf, "exit")) {
			break;
		}
	}
	SDLNet_TCP_Close(socket);
	SDLNet_Quit();

	return 0;
}