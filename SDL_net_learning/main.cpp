#include "SDL.h"
#include "SDL_net.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
	if (0 != SDLNet_Init()) {
		//��ʼ��ʧ��
		return 0;
	}
	atexit(SDLNet_Quit);//ע��һ��������ֹʱִ�е��˳�����

	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, 16);
	cout << "Local Host: " << SDLNet_ResolveIP(&ip) << endl;
	return 0;
}