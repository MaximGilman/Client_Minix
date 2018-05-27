
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <signal.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define SIGUSR1 10


DWORD WINAPI fool(LPVOID lpParam)
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int iResult;
	
	SOCKET ClientSocket = INVALID_SOCKET;
	ClientSocket = (SOCKET) lpParam;
	
	while( iResult = recv(ClientSocket, recvbuf, recvbuflen, 0) )
	{
		if(recvbuf[0] == '*')
		{
			send(ClientSocket, SIGUSR1, iResult, 0);
			break;
		}
		
		recvbuf[0]+=1;
		send(ClientSocket, recvbuf, iResult, 0);		
	}
	shutdown(ClientSocket, SD_SEND);
}


int __cdecl main(void)
{
	WSADATA wsaData;
	int iResult;
	
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	
	int iSendResult;
	
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	listen(ListenSocket, SOMAXCONN);
	
	while(1)
	{
		ClientSocket = accept(ListenSocket, NULL, NULL);
		CreateThread(NULL,0,fool, (LPVOID)ClientSocket, 0, NULL);
	}
	closesocket(ListenSocket);
	WSACleanup();
	
	return 0;
}

