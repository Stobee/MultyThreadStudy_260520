#include "pch.h"
#include "MovePacket.h"

void S2CMovePacket::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());

	
}

std::string S2CMovePacket::ToString()
{
	return std::string();
}
