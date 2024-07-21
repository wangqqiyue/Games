#include <iostream>
#include <thread>
#include "SDL.h"
#include "SDL_net.h"

using std::cin;
using std::cout;
using std::endl;
using std::thread;


void doHandler(TCPsocket client_sock);

void serverHandler(TCPsocket server_sock) {
	TCPsocket client_sock;
	while (true) {
		if (client_sock = SDLNet_TCP_Accept(server_sock)) {
			thread t(doHandler, client_sock);
			t.detach();
		}
	}
}

int main(int arg, char* argv[]) {
	TCPsocket server_sock, client_sock;
	IPaddress server_ip, client_ip;
	char dstip[30];
	int dstport;
	char buf[512];
	int len;

	if (0 != SDLNet_Init()) {
		//初始化失败
		return 0;
	}
	atexit(SDLNet_Quit);//注册一个程序终止时执行的退出函数

	SDLNet_ResolveHost(&server_ip, nullptr, 12345);
	server_sock = SDLNet_TCP_Open(&server_ip);
	//创建线程,用于接受其他P2P节点的连接请求
	thread server_thread(serverHandler, server_sock);
	server_thread.detach();


	//向指定P2P节点发送请求
	// 先从用户输入获取目的节点的IP和port
	cout << "请输入目的节点ip和port" << endl;
	cout << "ip:";
	cin >> dstip;
	cout << "port:";
	cin >> dstport;

	//如果已经连接过了，直接发送请求消息
	//如果是首次连接，先连接
	SDLNet_ResolveHost(&client_ip, dstip, dstport);
	client_sock = SDLNet_TCP_Open(&client_ip);
	while (true) {
		cout << endl << "输入消息:";
		memset(buf, 0, 512);
		gets_s(buf, 512);
		len = strlen(buf);
		SDLNet_TCP_Send(client_sock, buf, len);
	}
	SDLNet_TCP_Close(client_sock);
	SDLNet_Quit();

}

void doHandler(TCPsocket client_sock) {
	IPaddress* client_ip = SDLNet_TCP_GetPeerAddress(client_sock);
	unsigned char* client_ip_str = (unsigned char*)&client_ip->host;
	int port = client_ip->port;
	char buf[512] = { 0 };
	int len = 0;
	bool quit = false;

	printf("已与客户端%d.%d.%d.%d:%d 建立连接\n",
		client_ip_str[0], client_ip_str[1], client_ip_str[2], client_ip_str[3], port);
	while (!quit) {

		len = SDLNet_TCP_Recv(client_sock, buf, 512);
		buf[len] = 0;
		printf("来自%d.%d.%d.%d:%d: ",
			client_ip_str[0], client_ip_str[1], client_ip_str[2], client_ip_str[3], port);
		cout << buf << endl;
		if (0 == strcmp(buf, "exit")) {
			printf("%d.%d.%d.%d:%d 已退出\n",
				client_ip_str[0], client_ip_str[1], client_ip_str[2], client_ip_str[3], port);
			quit = true;
		}
	}
	SDLNet_TCP_Close(client_sock);
}
