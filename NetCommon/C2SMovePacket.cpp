#include "pch.h"
#include "C2SMovePacket.h"

void C2SMovePacket::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());

	MovementInput = JSONDocument["MovementInput"].GetInt();
	
}

std::string C2SMovePacket::ToString()
{
	JSONDocument.SetObject();
	JSONDocument.AddMember("MovementInput", MovementInput, JSONDocument.GetAllocator());

	rapidjson::StringBuffer Buffer;
	rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
	JSONDocument.Accept(Writer);

	return Buffer.GetString();
}

int C2SMovePacket::Length()
{
	return (int)ToString().length();
}
