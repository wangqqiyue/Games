#pragma once
#include "SDL.h"
#include "SDL_net.h"
#include "Global.h"
class NetworkManager
{
public:
	//µ¥ÀýÄ£Ê½
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
private:
	static NetworkManager* s_pInstance;
	NetworkManager();
	~NetworkManager();
	bool connect();

	TCPsocket m_socket = nullptr;
	const char* m_server_ip = "116.198.37.2";
	int m_server_port = 8888;
};

typedef  NetworkManager TheNetworkManager;
