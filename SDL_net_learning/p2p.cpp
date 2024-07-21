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
		//��ʼ��ʧ��
		return 0;
	}
	atexit(SDLNet_Quit);//ע��һ��������ֹʱִ�е��˳�����

	SDLNet_ResolveHost(&server_ip, nullptr, 12345);
	server_sock = SDLNet_TCP_Open(&server_ip);
	//�����߳�,���ڽ�������P2P�ڵ����������
	thread server_thread(serverHandler, server_sock);
	server_thread.detach();


	//��ָ��P2P�ڵ㷢������
	// �ȴ��û������ȡĿ�Ľڵ��IP��port
	cout << "������Ŀ�Ľڵ�ip��port" << endl;
	cout << "ip:";
	cin >> dstip;
	cout << "port:";
	cin >> dstport;

	//����Ѿ����ӹ��ˣ�ֱ�ӷ���������Ϣ
	//������״����ӣ�������
	SDLNet_ResolveHost(&client_ip, dstip, dstport);
	client_sock = SDLNet_TCP_Open(&client_ip);
	while (true) {
		cout << endl << "������Ϣ:";
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

	printf("����ͻ���%d.%d.%d.%d:%d ��������\n",
		client_ip_str[0], client_ip_str[1], client_ip_str[2], client_ip_str[3], port);
	while (!quit) {

		len = SDLNet_TCP_Recv(client_sock, buf, 512);
		buf[len] = 0;
		printf("����%d.%d.%d.%d:%d: ",
			client_ip_str[0], client_ip_str[1], client_ip_str[2], client_ip_str[3], port);
		cout << buf << endl;
		if (0 == strcmp(buf, "exit")) {
			printf("%d.%d.%d.%d:%d ���˳�\n",
				client_ip_str[0], client_ip_str[1], client_ip_str[2], client_ip_str[3], port);
			quit = true;
		}
	}
	SDLNet_TCP_Close(client_sock);
}
