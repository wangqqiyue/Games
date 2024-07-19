#include <iostream>
#include <thread>
#include "SDL.h"
#include "SDL_net.h"

using std::cin;
using std::cout;
using std::endl;
using std::thread;

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


	//创建线程,用于接受其他P2P节点的连接请求

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
		cout << "输入消息:";
		memset(buf, 0, 512);
		cin >> buf;
		len = strlen(buf);
		SDLNet_TCP_Send(client_sock, buf, len);
	}
	SDLNet_TCP_Close(client_sock);
	SDLNet_Quit();

}