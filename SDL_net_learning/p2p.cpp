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
		//��ʼ��ʧ��
		return 0;
	}
	atexit(SDLNet_Quit);//ע��һ��������ֹʱִ�е��˳�����


	//�����߳�,���ڽ�������P2P�ڵ����������

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
		cout << "������Ϣ:";
		memset(buf, 0, 512);
		cin >> buf;
		len = strlen(buf);
		SDLNet_TCP_Send(client_sock, buf, len);
	}
	SDLNet_TCP_Close(client_sock);
	SDLNet_Quit();

}