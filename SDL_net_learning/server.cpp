#include <iostream>
#include "SDL.h"
#include "SDL_net.h"

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
	IPaddress ip;
	TCPsocket server_sock, client_sock;
	if (0 != SDLNet_Init()) {
		return -1;
	}
	atexit(SDLNet_Quit);
	SDLNet_ResolveHost(&ip, nullptr, 12345);
	server_sock = SDLNet_TCP_Open(&ip);
	while (true) {
		if (client_sock = SDLNet_TCP_Accept(server_sock)) {
			IPaddress *client_ip = SDLNet_TCP_GetPeerAddress(client_sock);
			unsigned char* ip = (unsigned char *)client_ip->host;
			cout << "开始接受来自客户端" << 
				ip[0] << "." << ip[1] << "." << ip[2] << "." << ip[3] << 
				"的消息" << endl;

			char buf[512];
			int len = 0;
			bool quit = false;
			while (!quit) {
				len = SDLNet_TCP_Recv(client_sock, buf, 512);
				cout << "消息：" << buf << endl;
			}
		}
	}
	SDLNet_TCP_Close(server_sock);
	SDLNet_Quit();
}