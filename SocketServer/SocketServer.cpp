#include <iostream>
#include "SocketServer.h"
#include "Utils.h"
#include "AnswersServicer.h"

using namespace std;



CSocketServer::CSocketServer(PCSTR szDesfualtPort)
    :m_szDesfualtPort(szDesfualtPort),
    m_DEFAULT_BUFLEN(512)
{
    m_isInitializtaionProcessOK = TRUE;
    m_listenSocket = INVALID_SOCKET;
    m_addrinfoResult = NULL;
}

CSocketServer::~CSocketServer()
{
    WSACleanup();
    freeaddrinfo(m_addrinfoResult);
}

void CSocketServer::runForever()
{
    SOCKET ClientSocket = INVALID_SOCKET;
    if (!m_isInitializtaionProcessOK)
    {
        return;
    }

    fnInitializeWinsock();
    fnCreateServerSocket();
    fnBindSocket();
    fnListenForClient();
   
    while (TRUE)
    {

        // Accept a client socket
        ClientSocket = accept(m_listenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(m_listenSocket);
            return;
        }
        else {
            handleSingleClient(ClientSocket); //this should be in a thread
        }
    }
}


void setHints(struct addrinfo& hints)
{
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
}

void CSocketServer::fnCreateServerSocket()
{ 
    struct addrinfo * ptr = NULL, hints;

    if (!m_isInitializtaionProcessOK)
    {
        return;
    }

    setHints(hints);

    // Resolve the local address and port to be used by the server
    INT iResult = getaddrinfo(NULL, m_szDesfualtPort, &hints, &m_addrinfoResult);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        m_isInitializtaionProcessOK = FALSE;
        return;
    }

    m_listenSocket = socket(m_addrinfoResult->ai_family, m_addrinfoResult->ai_socktype, m_addrinfoResult->ai_protocol);
    if (m_listenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(m_addrinfoResult);
        m_isInitializtaionProcessOK = FALSE;
        return;
    }
}

void CSocketServer::fnBindSocket()
{
    if (!m_isInitializtaionProcessOK)
    {
        return;
    }

    // Setup the TCP listening socket
    INT iResult = bind(m_listenSocket, m_addrinfoResult->ai_addr, (int)m_addrinfoResult->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(m_addrinfoResult);
        closesocket(m_listenSocket);
        return;
    }

}

void CSocketServer::fnListenForClient()
{
    if (!m_isInitializtaionProcessOK)
    {
        return;
    }

    if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error: %ld\n", WSAGetLastError());
        closesocket(m_listenSocket);
        m_isInitializtaionProcessOK = FALSE;
        return;
    }

}

void CSocketServer::handleSingleClient(SOCKET ClientSocket)const noexcept
{
    //do recive
    PSTR answer = NULL;
    AnswerServicer knowTheAnswer;

    PCHAR recvbuf = (PCHAR)Allocations::AllocBuffer(m_DEFAULT_BUFLEN);
    if (NULL == recvbuf)
    {
        return;
    }

    INT iResult, iSendResult;
    INT recvbuflen = m_DEFAULT_BUFLEN;

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            Answer ans = knowTheAnswer.corespondingAnswer(recvbuf, recvbuflen);
            iSendResult = send(ClientSocket, ans.aBuff, ans.cbSize, 0);
            Allocations::FreeBuffer(ans.aBuff);

            if (iSendResult == SOCKET_ERROR) {
                closesocket(ClientSocket);
                Allocations::FreeBuffer(recvbuf);
                return;
            }
        }

        else {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            Allocations::FreeBuffer(recvbuf);
            return;
        }

        Sleep(10);
    } while (iResult > 0);
    Allocations::FreeBuffer(recvbuf);
}

void CSocketServer::fnInitializeWinsock()
{
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        m_isInitializtaionProcessOK = FALSE;
        return;
    }

}
