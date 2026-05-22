#pragma once
#include "pch.h"
#include "C2S_Login.h"
#include "S2C_Login.h"
#include "S2C_Spawn.h"
#include "S2C_Destroy.h"


extern int RecvAll(SOCKET ReceiverSocket, char* Data, int Size);

extern int SendAll(SOCKET ReceiverSocket, const char* Data, int Size);

extern void DisconnectSocket(SOCKET DisconnectedSocket, fd_set* Sockets);




