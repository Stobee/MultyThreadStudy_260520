#pragma once
#include "pch.h"

enum class EPacketType : unsigned char
{
	C2S_Login = 1,
	S2C_Login,
	S2C_Spawn,
	S2C_Destroy,
	C2SMovePacket,
	S2CMovePacket,
	ChatPacket,
	Max
};
#pragma pack(push, 1)
struct PacketHeader
{
	unsigned short Size;
	unsigned char Type;
	
	void MakeHeader(int InPacketSize, EPacketType InPacketType)
	{
		Size = InPacketSize;
		Type = static_cast<unsigned char>(InPacketType);
	}

	void NetworkToHost()
	{
		
	}
};
#pragma pack(pop)