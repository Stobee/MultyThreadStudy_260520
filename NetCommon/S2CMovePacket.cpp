#include "pch.h"
#include "S2CMovePacket.h"

void S2CMovePacket::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());

	UserNum = JSONDocument["UserNum"].GetInt();
	UserX = JSONDocument["UserX"].GetInt();
	UserY = JSONDocument["UserY"].GetInt();
}

std::string S2CMovePacket::ToString()
{
	JSONDocument.SetObject();
	JSONDocument.AddMember("UserNum", UserNum, JSONDocument.GetAllocator());
	JSONDocument.AddMember("UserX", UserX, JSONDocument.GetAllocator());
	JSONDocument.AddMember("UserY", UserY, JSONDocument.GetAllocator());

	rapidjson::StringBuffer Buffer;
	rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
	JSONDocument.Accept(Writer);

	return Buffer.GetString();
}

int S2CMovePacket::Length()
{
	return (int)ToString().length();
}
