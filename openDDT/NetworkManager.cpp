#include "NetworkManager.h"


NetworkManager* NetworkManager::s_pInstance = nullptr;
TCPsocket NetworkManager::m_socket=nullptr;



NetworkManager::NetworkManager() {
	if (0 != SDLNet_Init()) {
		//初始化失败
		cout << "SDLNet_Init failed." << endl;
		return;
	}
	connect();
}

NetworkManager::~NetworkManager() {
	SDLNet_Quit();
}


bool NetworkManager::connect() {
	IPaddress ip;
	SDLNet_ResolveHost(&ip, m_server_ip, m_server_port);
	m_socket = SDLNet_TCP_Open(&ip);
	if (!m_socket) {
		cout << "connect failed." << endl;
		return false;
	}
	createRecvThread();
	return true;
}


void NetworkManager::doRecv() {
	char *buffer = new char[BUFSIZ];
	while (true)
	{
		memset(buffer, 0, BUFSIZ);
		int len = SDLNet_TCP_Recv(m_socket, buffer, BUFSIZ);
		if (len > 0)
		{
			TheGame::Instance()->recvMsg(buffer,len);
		}
		else
		{
			break;
		}
	}
	delete[] buffer;
	SDLNet_TCP_Close(m_socket);
}
bool NetworkManager::createRecvThread() {
	thread th(NetworkManager::doRecv);//线程处理
	th.detach();
	return true;
}

bool NetworkManager::send(const char *msg) {
	int len = SDLNet_TCP_Send(m_socket, msg, strlen(msg));
	return len != 0;
}
