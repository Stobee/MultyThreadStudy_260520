#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <WinSock2.h>
#include <iostream>
#include "NetUtills.h"


using namespace rapidjson;

#pragma comment(lib, "NetCommon")
#pragma comment(lib, "ws2_32")


// 버퍼 재사용
char Buffer[1024] = { 0, };

int main()
{
	std::cout << "Server" << std::endl;

	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_port = htons(35000);

	bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	// 최대 크기
	listen(ListenSocket, SOMAXCONN);

	

	// Blocking, Synchronous(TimeOut) >> 다른 os와 호환성이 좋음
	TIMEVAL TimeOut;
	TimeOut.tv_sec = 0;
	TimeOut.tv_usec = 500000;

	fd_set ReadSockets;
	fd_set CopyReadSockets;

	FD_ZERO(&ReadSockets); // set 초기화
	FD_SET(ListenSocket, &ReadSockets); // set에 소켓 추가

	while (true)
	{
		CopyReadSockets = ReadSockets;

		// 0.5초 마다 Blocking함
		int ChangeCount = select(0, &CopyReadSockets, 0, 0, &TimeOut);

		if (ChangeCount <= 0)
		{

			continue;
		}

		for (int i = 0; i < (int)ReadSockets.fd_count; i++)
		{	
			// 비교하는 내부함수
			if (FD_ISSET(ReadSockets.fd_array[i], &CopyReadSockets))
			{	
				// 리슨 소켓이라면 accept함
				if (ReadSockets.fd_array[i] == ListenSocket)
				{
					SOCKADDR_IN ClientSockAddr;
					memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
					int ClientSockLength = sizeof(ClientSockAddr);

					// Blocking, Synchronous
					SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockLength);

					std::cout << "Connect Client " << inet_ntoa(ClientSockAddr.sin_addr) << std::endl;

					FD_SET(ClientSocket, &ReadSockets);
				}
				else
				{
					// 데이터 받는 작업. 

					//header
					unsigned short PacketSize = 0;
					int RecvBytes = recv(ReadSockets.fd_array[i], (char*)&PacketSize, sizeof(PacketSize), MSG_WAITALL);
					if (RecvBytes <= 0)
					{
						std::cout << "header recv fail " << std::endl;
						DisconnectSocket(ReadSockets.fd_array[i], &ReadSockets);
						continue;
					}

					PacketSize = ntohs(PacketSize);

					//data
					memset(Buffer, 0, sizeof(Buffer));
					int RecvBytes = recv(ReadSockets.fd_array[i], Buffer, sizeof(Buffer), 0);
					if (RecvBytes <= 0)
					{
						std::cout << "data recv fail " << std::endl;
						DisconnectSocket(ReadSockets.fd_array[i], &ReadSockets);
						continue;
					}
					else
					{
						SOCKADDR_IN ClientSockAddr;
						memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
						int ClientSockLength = sizeof(ClientSockAddr);

						getpeername(ReadSockets.fd_array[i], (SOCKADDR*)&ClientSockAddr, &ClientSockLength);

						std::cout << "client(" << inet_ntoa(ClientSockAddr.sin_addr);
						std::cout << ")" << Buffer << " send" << std::endl;
						//모든 접속한 유저한테 전달

						for (int j = 0; j < (int)ReadSockets.fd_count; ++j)
						{
							//자기꺼는 그냥 찍고 안 받으면 안되요?
							//클라이언트에서는 처리 안함.
							if (ReadSockets.fd_array[j] != ListenSocket)
							{
								PacketSize = (unsigned short)strlen(Buffer);
								PacketSize = htons(PacketSize);

								//header
								int SentBytes = SendAll(ReadSockets.fd_array[j], (char*)&PacketSize, 2);
								if (SentBytes <= 0)
								{
									std::cout << "header send fail." << std::endl;
									DisconnectSocket(ReadSockets.fd_array[j], &ReadSockets);
								}

								//Data
								SentBytes = SendAll(ReadSockets.fd_array[j], Buffer, ntohs(PacketSize));
								if (SentBytes <= 0)
								{
									std::cout << "Data send fail." << std::endl;
									DisconnectSocket(ReadSockets.fd_array[j], &ReadSockets);
								}
							}
						}
					}
				}
			}
		}
	}

	
	closesocket(ListenSocket);
	WSACleanup();

	return 0;
}