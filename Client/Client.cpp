#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <WinSock2.h>
#include <iostream>
#include <process.h>
#include "ChatPacket.h"
#include "NetUtills.h"

using namespace rapidjson;

#pragma comment(lib, "NetCommon")
#pragma comment(lib, "ws2_32")

// 버퍼 재사용
char SendBuffer[1024] = { 0, };
char RecvBuffer[1024] = { 0, };
bool IsRecvThreadRunning = true;
bool IsSendThreadRunning = true;
// WINAPI == __StdCall
// void* 보이드 포인터. 자료의 형태가 정해지지 않음.
unsigned WINAPI RecvThread(void* Socket)
{
	SOCKET ServerSocket = *(SOCKET*)Socket;
	while (IsRecvThreadRunning)
	{
		unsigned short PacketSize = 0;

		//header
		int RecvBytes = recv(ServerSocket, (char*)&PacketSize, sizeof(PacketSize), MSG_WAITALL);
		if (RecvBytes <= 0)
		{
			std::cout << "recv fail " << std::endl;
			break;
		}

		PacketSize = ntohs(PacketSize);

		memset(RecvBuffer, 0, sizeof(RecvBuffer));
		//data JSON
		RecvBytes = recv(ServerSocket, RecvBuffer, PacketSize, MSG_WAITALL);
		if (RecvBytes <= 0)
		{
			std::cout << "recv fail " << std::endl;
			break;
		}

		ChatPacket Data;

		Data.Parse(RecvBuffer);

		std::cout << Data.UserID << " : " << Data.Message << " " << Data.Gold << std::endl;
	}

	return 0;
}

unsigned WINAPI SendThread(void* Socket)
{
	SOCKET ServerSocket = *(SOCKET*)Socket;
	while (IsSendThreadRunning)
	{
		std::cin.getline(SendBuffer, sizeof(SendBuffer));

		ChatPacket Data;
		Data.UserID = "junios";
		Data.Message = SendBuffer;
		Data.Gold = 1000;
		std::string JSONString = Data.ToString();

		unsigned short PacketSize = (unsigned short)JSONString.length();
		PacketSize = htons(PacketSize);

		//header
		int SentBytes = SendAll(ServerSocket, (char*)&PacketSize, 2);
		if (SentBytes <= 0)
		{
			std::cout << "header send fail." << std::endl;
			break;
		}

		//Data
		SentBytes = SendAll(ServerSocket, JSONString.c_str(), ntohs(PacketSize));
		if (SentBytes <= 0)
		{
			std::cout << "data send fail." << std::endl;
			break;
		}

	}
	return 0;
}

int main()
{
	std::cout << "Client" << std::endl;

	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));

	ServerSockAddr.sin_addr.s_addr = inet_addr("192.168.0.95");
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_port = htons(35000);

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	std::cout << "Server Connect" << std::endl;

	// 스레드 사용, nonBlocking, asynchronus
	HANDLE ThreadHandles[2] = { 0, };
	ThreadHandles[0] = (HANDLE)_beginthreadex(0, 0, RecvThread, &ServerSocket, 0, 0);
	ThreadHandles[1] = (HANDLE)_beginthreadex(0, 0, SendThread, &ServerSocket, 0, 0);

	// Blocking
	WaitForMultipleObjects(2, ThreadHandles, FALSE, INFINITE);


	closesocket(ServerSocket);

	IsSendThreadRunning = false;
	IsRecvThreadRunning = false;

	
	if (ThreadHandles[0] != NULL && ThreadHandles[0] != INVALID_HANDLE_VALUE)
	{
		CloseHandle(ThreadHandles[0]);
		ThreadHandles[0] = NULL; // 댕글링 핸들 방지
	}
	if (ThreadHandles[1] != NULL && ThreadHandles[1] != INVALID_HANDLE_VALUE)
	{
		CloseHandle(ThreadHandles[1]);
		ThreadHandles[1] = NULL; 
	}
	WSACleanup();
	return 0;

}