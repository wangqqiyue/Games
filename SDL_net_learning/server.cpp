#include <iostream>
#include <thread>
#include "SDL.h"
#include "SDL_net.h"

using std::cin;
using std::cout;
using std::endl;
using std::thread;

void doHandler(TCPsocket client_sock) {
	IPaddress* client_ip = SDLNet_TCP_GetPeerAddress(client_sock);
	unsigned char* client_ip_str = (unsigned char*)&client_ip->host;
	int port= client_ip->port;
	char buf[512] = { 0 };
	int len = 0;
	bool quit = false;
	
	printf("已与客户端%d.%d.%d.%d:%d 建立连接\n",
		client_ip_str[0], client_ip_str[1], client_ip_str[2], client_ip_str[3],port);
	while (!quit) {

		len = SDLNet_TCP_Recv(client_sock, buf, 512);
		buf[len] = 0;
		printf("来自%d.%d.%d.%d:%d: ",
			client_ip_str[0], client_ip_str[1], client_ip_str[2], client_ip_str[3],port);
		cout << buf << endl;
		if (0 == strcmp(buf, "exit")) {
			printf("%d.%d.%d.%d:%d 已退出\n",
				client_ip_str[0], client_ip_str[1], client_ip_str[2], client_ip_str[3],port);
			quit = true;
		}
	}
	SDLNet_TCP_Close(client_sock);
}

int main(int argc, char* argv[]) {
	IPaddress ip;
	TCPsocket server_sock, client_sock;
	if (0 != SDLNet_Init()) {
		return -1;
	}
	atexit(SDLNet_Quit);
	SDLNet_ResolveHost(&ip, nullptr, 12345);
	server_sock = SDLNet_TCP_Open(&ip);
	cout << "等待客户端连接中...." << endl;
	while (true) {
		if (client_sock = SDLNet_TCP_Accept(server_sock)) {
			thread t(doHandler, client_sock);
			t.detach();
		}
	}
	SDLNet_TCP_Close(server_sock);
	SDLNet_Quit();
}