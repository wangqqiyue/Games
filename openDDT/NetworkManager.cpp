#include "NetworkManager.h"

NetworkManager* NetworkManager::s_pInstance = nullptr;

NetworkManager::NetworkManager() {
	if (0 != SDLNet_Init()) {
		//≥ı ºªØ ß∞‹
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
		return false;
	}
	return true;
}