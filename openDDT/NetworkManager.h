#pragma once
#include "SDL.h"
#include "SDL_net.h"
#include "Global.h"
#include "Game.h"
#include <thread>
#include <Windows.h>
using std::thread;

class NetworkManager
{
public:
	//����ģʽ
	static NetworkManager* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new NetworkManager();
			return s_pInstance;
		}
		return s_pInstance;
	}
	bool send(const char* msg);
	bool createRecvThread();
	static TCPsocket m_socket;
private:
	static NetworkManager* s_pInstance;
	NetworkManager();
	~NetworkManager();
	bool connect();
	static void doRecv();

	
	const char* m_server_ip = "116.198.36.246";
	int m_server_port = 8888;
};

typedef  NetworkManager TheNetworkManager;


