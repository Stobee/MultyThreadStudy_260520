#pragma once
#include "pch.h"
#pragma pack(push, 1)
struct PacketHeader
{
	unsigned short Size;
	unsigned char Type;
	
};
#pragma pack(pop)