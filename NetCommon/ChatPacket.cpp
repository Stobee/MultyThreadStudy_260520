#include "pch.h"
#include "ChatPacket.h"
#include <sstream>

void ChatPacket::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());

	UserID = JSONDocument["UserID"].GetString();
	Message = JSONDocument["Message"].GetString();
	Gold = JSONDocument["Gold"].GetInt();
}

std::string ChatPacket::ToString()
{
    //JSONDocument를 문자열 변환 요청
    JSONDocument.SetObject();
    JSONDocument.AddMember("UserID", UserID, JSONDocument.GetAllocator());
    JSONDocument.AddMember("Message", Message, JSONDocument.GetAllocator());
    JSONDocument.AddMember("Gold", Gold, JSONDocument.GetAllocator());

    rapidjson::StringBuffer Buffer;
    rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
    JSONDocument.Accept(Writer);

    return Buffer.GetString();
}

int ChatPacket::Length()
{
    return (int)ToString().length();
}
