#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")


class CSocketServer {
public:
	CSocketServer(PCSTR szDesfualtPort);
	~CSocketServer();

	void runForever();

protected:	
	//1.Initialize Winsock.
	void fnInitializeWinsock();
	//2.Create a socket.
	void fnCreateServerSocket();
	//3.Bind the socket.
	void fnBindSocket();
	//4.Listen on the socket for a client.
	void fnListenForClient();
		/*

	7.Disconnect.*/
	void handleSingleClient(SOCKET ClientSocket)const noexcept;


private:
	BOOL m_isInitializtaionProcessOK;
	PCSTR m_szDesfualtPort;
	SOCKET m_listenSocket;
	struct addrinfo* m_addrinfoResult;
	const INT m_DEFAULT_BUFLEN;
};
